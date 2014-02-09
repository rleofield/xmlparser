/* --------------------------------------------------------------------------
Copyright 2012 by Richard Albrecht
richard.albrecht@rleofield.de
www.rleofield.de

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------------------------------------
*/

#include <boost/filesystem.hpp>

#include "stringhelper.h"
#include "t_filename.h"


using std::string;

using rlf_hstring::index_right;
using rlf_hstring::last_char;


namespace rlf_filefn {


   // http://www.boost.org/doc/libs/1_55_0/libs/filesystem/doc/reference.html#Path-decomposition-table

   namespace fn_intern {

      const string dot = ".";

      string slash() {
         static string slash = string();

         if( slash.size() == 0 ) {
            boost::filesystem::path p = boost::filesystem::current_path();
            slash = p.root_directory().string();
            assert( slash.size() == 1 );
         }

         return  slash;
      }
      char char_slash() {
         string sl = slash();
         assert( sl.size() == 1 );
         return  sl[0];
      }

      std::string remove_dot_at_front( std::string const& file ) {
         if( file.size() == 0 ) {
            return string();
         }

         size_t pos =  file.rfind( "." );

         if( pos == 0 ) {
            return file.substr( pos + 1 );;
         }

         return file;
      }
      std::string correct_slash_at_end( std::string const& path ) {
         std::string temp = path;

         if( temp.size() == 0 ) {
            return string();
         }

         // correct slash at end
         string s = fn_intern::slash();

         if( last_char( temp ) != char_slash() ) {
            temp +=  s;
         }

         return temp;
      }

      bool file_exists_( boost::filesystem::path const& p ) {
         if( !boost::filesystem::is_regular_file( p ) ) {
            return false;
         }

         boost::filesystem::file_status s = status( p );

         if( boost::filesystem::exists( s ) ) {
            return true;
         }

         //#include "filehelper.h"

         return false;
      }
      bool path_exists_( boost::filesystem::path const& p ) {
         boost::filesystem::file_status s = status( p );

         if( boost::filesystem::is_directory( s ) ) {
            return true;
         }

         return false;
      }

      bool file_exists( string const& fn ) {
         boost::filesystem::path p( fn );
         return file_exists_( p );
      }
      bool path_exists( string const& path ) {
         boost::filesystem::path p( path );
         return path_exists_( p );
      }
      bool is_folder( string const& path ) {
         boost::filesystem::path p( path );
         return boost::filesystem::is_directory( p );
      }

      struct t_bfe {
         t_bfe(): path(), base(), ext() {}
         string path;
         string base;
         string ext;
      };
      void splitpath( string const& in, t_bfe& bfe_ ) {
         string temp = in; // + "abc.t";
         string cb;
         string ce;
         string cp;
         boost::filesystem::path p( temp );

         if( boost::filesystem::exists( p ) ) {
            if( boost::filesystem::is_regular_file( p ) ) {
               cb = p.filename().string();
               ce = p.extension().string();
               cp = p.parent_path().string();

               if( cb == fn_intern::dot ) {
                  cb = string();
               }

               if( ce.size() > 0 ) {
                  size_t pos = cb.rfind( ce );

                  if( pos != string::npos ) {
                     cb = rlf_hstring::clip_at_pos( cb, pos );
                  }

                  ce = fn_intern::remove_dot_at_front( ce );
               }
            }

            if( boost::filesystem::is_directory( p ) ) {
               cp = in;
            }

            cp = fn_intern::correct_slash_at_end( cp );
            bfe_.base = cb;
            bfe_.ext = ce;
            bfe_.path = cp;
            return;
         }

         // doesn't exist
         cb = p.filename().string();
         ce = p.extension().string();
         cp = p.parent_path().string();

         if( cb == fn_intern::dot ) {
            cb = string();
         }

         if( ce.size() > 0 ) {
            size_t pos = cb.rfind( ce );

            if( pos != string::npos ) {
               cb = rlf_hstring::clip_at_pos( cb, pos );
            }

            ce = fn_intern::remove_dot_at_front( ce );
         }

         cp = fn_intern::correct_slash_at_end( cp );
         bfe_.base = cb;
         bfe_.ext = ce;
         bfe_.path = cp;


      }

   } // end of ns fn_intern



   class fnimpl {

   public:
      fnimpl(): path(), base(), ext() {}
      fnimpl( fnimpl const& fi ): path( fi.path ), base( fi.base ), ext( fi.ext ) {}
      fnimpl( const std::string& path_, const std::string& base_, const std::string& extension_ )
         : path( path_ ), base( base_ ), ext( extension_ ) {


      }
      //   fnimpl( boost::filesystem::path const& p_ ):p(p_){}

      fnimpl& operator=( const fnimpl& s );

      bool operator==( const fnimpl& s )const {
         return ( path == s.path && base == s.base && ext == s.ext );
      }

      string path;
      string base;
      string ext;



   };

   fnimpl& fnimpl::operator=( const fnimpl& s ) {
      if( this != &s ) {
         path = s.path;
         base = s.base;
         ext = s.ext;
      }

      return *this;
   }


   t_filename::t_filename(): impl( new fnimpl() ) {}

   t_filename::t_filename( string const& f ): impl( new fnimpl() ) {
      fn_intern::t_bfe bfe;
      fn_intern::splitpath( f, bfe );
      impl->path = bfe.path;
      impl->base = bfe.base;
      impl->ext = bfe.ext;
   }

   t_filename::t_filename(
      const std::string& path_, const std::string& base_, const std::string& extension_ )
      : impl( new fnimpl( path_, base_, extension_ ) ) {}

   t_filename::t_filename( const t_filename& fn )
      : impl( new fnimpl( *( fn.impl ) ) ) {}

   t_filename::~t_filename() {
      delete impl;
   }

   t_filename& t_filename::operator=( const t_filename& s ) {
      if( this != &s ) {
         *impl  = *( s.impl );
      }

      return *this;
   }

   bool t_filename::operator==( const t_filename& s )const {
      return *impl == *( s.impl );
   }


   void t_filename::base( string const& base_ ) {
      impl->base = base_;
   }


   void t_filename::path( string const& path_ ) {
      string temp = fn_intern::correct_slash_at_end( path_ );
      impl->path = temp;
   }

   void t_filename::extension( const string& ext_ ) {
      impl->ext = ext_;
   }

   string t_filename::base()const {
      return impl->base;
   }

   string t_filename::path()const {
      return fn_intern::correct_slash_at_end( impl->path );
   }

   string t_filename::extension()const {
      return impl->ext;
   }

   void t_filename::fullname( const string& p, const string& f,  const string& e ) {
      path( p );
      base( f );
      extension( e );
   }



   string t_filename::filename()const {
      string ext = extension();

      if( ext.length() > 0 ) {
         return base() + fn_intern::dot + ext;
      }

      return base();
   }


   string t_filename::fullname()const {
      return path() + filename();
   }

   string t_filename::key()const {
      return filename();
   }

   bool t_filename::file_exists()const {
      string fn = fullname();
      bool temp =  fn_intern::file_exists( fn );
      return temp;
   }
   bool t_filename::path_exists()const {
      string fn = path();
      bool temp =  fn_intern::path_exists( fn );
      return temp;
   }
   bool t_filename::is_folder()const {
      string fn = path();
      bool b = fn_intern::is_folder( fn );

      if( b && extension().size() == 0 &&  base().size() == 0 ) {
         return true;
      }

      return false;
   }




   t_filename splitpath( string const& in ) {
      fn_intern::t_bfe bfe;
      fn_intern::splitpath( in, bfe );
      return t_filename( bfe.path, bfe.base, bfe.ext );
   }



} // end of namespace


//EOF

