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


//#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/regex.hpp>
#include <memory>

#include "filetreewalk.h"
#include "stringhelper.h"


using std::string;
using std::vector;
using namespace boost::filesystem;
using rlf_filefn::t_filename;
using rlf_hstring::split;
using rlf_hstring::index;

namespace rlf_ftw {
   namespace {
      bool path_exists_( boost::filesystem::path const& p ) {
         boost::filesystem::file_status s = status( p );

         if( boost::filesystem::is_directory( s ) ) {
            return true;
         }

         return false;
      }

      bool path_exists( string const& path ) {
         boost::filesystem::path p( path );
         return path_exists_( p );
      }
      bool is_directory( string const& path ) {
         boost::filesystem::path p( path );
         return boost::filesystem::is_directory( p );
      }
   }

   const string delimiters = ";, ";

   class ftw_impl {
   public:
      std::vector <t_filename > _files;
      std::vector <t_filename > _folders;
      std::vector<std::string> _folder_exclude;
      std::vector<std::string> _file_include;
      t_filename _path;

      ~ftw_impl() {
         std::cout << std::endl;
      }
      ftw_impl() : _files(), _folders(), _folder_exclude(), _file_include(), _path() {}
      ftw_impl( std::string const& p ) : _files(), _folders(), _folder_exclude(), _file_include(), _path( p ) {}
      std::vector <t_filename > const& files()const {
         return _files;
      }
      std::vector <t_filename > const& folders()const {
         return _folders;
      }


   };



   namespace {
      bool exclude_folder( string const& n, vector<string> const& list ) {
         // false, wenn file verwendet wird
         if( list.size() == 0 ) {
            return false;
         }

         for( auto s : list ) {
            if( s.size() == 0 ) {
               return false;    // not excluded
            }

            if( s == "*" ) { // excluded
               return true;
            }

            if( boost::ends_with( n, s ) ) {
               return true; // excluded
            }

            if( boost::contains( n, s ) ) {
               return true; // excluded
            }
         }

         return false; // not excluded
      }

      bool include_file( string const& n, vector<string> const& list ) {
         // true, wenn file verwendet wird
         if( list.size() == 0 ) {
            return true;
         }

         for( auto s : list ) {
            if( s.size() == 0 ) { // included
               return true;
            }

            if( s == "*" ) { // included
               return true;
            }

            if( boost::ends_with( n, s ) ) {
               return true; // included
            }
         }

         return false;
      }
   }


   void ftw::scan_folders() {

      if( !path_exists( _impl->_path.path() ) ) {
         throw bad_ftw( "folder doesn't exist: " + _impl->_path.path() );
      }

      if( !is_directory( _impl->_path.path() ) ) {
         throw bad_ftw( "input is not a folder: " + _impl->_path.path() );
      }

      //boost::regex pattern("*"); // list all files starting with a
      boost::filesystem::path  current_dir( _impl->_path.path() ); //

      for( recursive_directory_iterator iter( current_dir ), end;
            iter != end;
            ++iter ) {

         boost::filesystem::path p = iter->path();

         if( boost::filesystem::is_directory( p ) ) {
            std::string dname = p.string();
            size_t i = index( dname, ".tmp" );

            if( i != string::npos ) {
               i = 0;
            }

            bool b = exclude_folder( dname, _impl->_folder_exclude );

            if( b == false ) {
               t_filename fn;
               fn.path( dname );
               _impl->_folders.push_back( fn );
            }
         }

         if( boost::filesystem::is_regular_file( p ) ) {
            std::string name = p.string();
            t_filename fn = t_filename( name );
            string dir = fn.path();
            size_t i = index( dir, ".tmp" );

            if( i != string::npos ) {
               i = 0;
            }

            bool bdir = exclude_folder( dir, _impl->_folder_exclude );

            if( bdir == false ) {
               bool b_include = include_file( name, _impl->_file_include );

               if( b_include == true ) {
                  _impl->_files.push_back( name );
               }
            }

            //    if (regex_match(name, pattern))
            //      std::cout << iter->path() << "\n";
         }
      }
   }
   ftw::ftw():
      _impl( new ftw_impl() ) {
   }
   ftw::~ftw() {}

   ftw::ftw( std::string const& p ) : _impl( new ftw_impl( p ) ) {}


   void ftw::path( std::string const& p ) {
      if( _impl->_path.path() != p ) {
         _impl->_path = p;
         _impl->_files.clear();
         _impl->_folders.clear();
      }
   }
   void ftw::set_include_files( tIncludeFiles const& i ) {
      _impl->_file_include = split( i.last, delimiters );
   }

   void ftw::set_exclude_folders( tExcludeFolders const& i ) {
      _impl->_folder_exclude = split( i.contains, delimiters );
   }

   std::vector <t_filename > const& ftw::files()const {
      return _impl->files();
   }
   std::vector <t_filename > const& ftw::folders()const {
      return _impl->folders();
   }

}

// EOF


