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


#ifndef T_TEXT_READ_H
#define T_TEXT_READ_H


/*! \file rList.h
*  \brief class t_text_read, reads a text file to std::list
*  \author Richard Albrecht
*/



#include <fstream>
#include <sstream>
#include <iomanip>
#include <list>
#include <algorithm>
#include <stdexcept>
#include <boost/filesystem.hpp>


namespace rlf_txtrw {



   namespace rhelper {
      const std::string marker = "%s";
      inline std::string FindAndReplace( const std::string& source,
                                         const std::string& find,
                                         const std::string& replace ) {
         size_t i;
         size_t start = 0;
         std::string ret = source;

         for( ; ( i = ret.find( find, start ) ) != std::string::npos; ) {
            ret.replace( i, find.length(), replace );
            start = i + replace.length();
         }

         return ret;
      }

      inline std::string replace( std::string const& msg, std::string const& s0 = "" ) {

         if( s0.size() > 0 ) {
            return FindAndReplace( msg, rhelper::marker, s0 );
         }

         return msg;

      }





   }

   namespace err {
      const std::string msg_file_not_exists = "File doesn't exist: '" + rhelper::marker + "'";
      const std::string msg_read_file = " Couldn't read file '" + rhelper::marker + "'";

      inline std::string read_file( std::string const& s0 ) {
         return rhelper::replace( msg_read_file, s0 );
      }
      inline std::string file_not_exists( std::string const& s0 ) {
         return rhelper::replace( msg_file_not_exists, s0 );
      }
      inline bool file_exists_r( boost::filesystem::path const& p ) {
         if( !boost::filesystem::is_regular_file( p ) ) {
            return false;
         }

         boost::filesystem::file_status s = status( p );

         if( boost::filesystem::exists( s ) ) {
            return true;
         }

         return false;
      }

   } // end of ns err


   /*! bad_text_read
   \param [in] msg  Message
   */
   class bad_text_read: public std::runtime_error {
   public:
      bad_text_read( const std::string& msg )
         : std::runtime_error( msg ) { }
   };



   /*! \class t_text_read
   *  \brief reads a text file
   *
   * a text file is stored in a <b>string</b> list<br>
   */
   class t_text_read_list  {
      t_text_read_list& operator= ( const t_text_read_list& in );
      t_text_read_list( const t_text_read_list& in );


   public:

      t_text_read_list() {}
      ~t_text_read_list() {}

      void operator()( const std::string& filename, std::list<std::string>& lines )  {

         if( !err::file_exists_r( filename ) ) {
            std::string s = err::file_not_exists( filename );
            throw bad_text_read( s );
         }

         std::ifstream fp( filename.c_str() );

         if( !fp.is_open() ) {
            std::string s = err::read_file( filename );
            throw bad_text_read( s );
         }

         while( !fp.eof() ) {
            std::string temp;
            getline( fp, temp );

            if( !fp.fail() ) {
               lines.push_back( temp );
            } else {
               if( !fp.eof() ) {
                  std::string s = err::read_file( filename );
                  throw bad_text_read( s );
               }
            }
         }
      }

   };

   class t_text_read_string  {
      t_text_read_string& operator= ( const t_text_read_string& in );
      t_text_read_string( const t_text_read_string& in );


   public:

      t_text_read_string() {}
      ~t_text_read_string() {}

      void operator()( const std::string& filename, std::string& str )  {

         if( !err::file_exists_r( filename ) ) {
            std::string s = err::file_not_exists( filename );
            throw bad_text_read( s );
         }

         std::ifstream fp( filename.c_str() );

         if( !fp.is_open() ) {
            std::string s = err::read_file( filename );
            throw bad_text_read( s );
         }

         while( fp.good() ) {
            std::string temp;
            getline( fp, temp );

            if( !fp.fail() ) {
               str.append( temp );
            } else {
               if( !fp.eof() ) {
                  std::string s = err::read_file( filename );
                  throw bad_text_read( s );
               }
            }
         }
      }

   };


   class t_text_read  {
      t_text_read& operator= ( const t_text_read& in );
      t_text_read( const t_text_read& in );


   public:

      t_text_read() {}
      ~t_text_read() {}

      void operator()( const std::string& filename, std::vector<std::string>& lines )  {

         if( !err::file_exists_r( filename ) ) {
            std::string s = err::file_not_exists( filename );
            throw bad_text_read( s );
         }

         std::ifstream fp( filename.c_str() );

         if( !fp.is_open() ) {
            std::string s = err::read_file( filename );
            throw bad_text_read( s );
         }

         while( !fp.eof() ) {
            std::string temp;
            getline( fp, temp );

            if( !fp.fail() ) {
               lines.push_back( temp );
            } else {
               if( !fp.eof() ) {
                  std::string s = err::read_file( filename );
                  throw bad_text_read( s );
               }
            }
         }
      }

   };

   // converts the output list to one string, with linebreaks
   //   inline std::string toString( const std::list<std::string>& lines  ) {
   //      const std::string sep = "\n";
   //      std::string s;

   //      for( auto temp : lines ) {
   //         if( !s.empty() ) {
   //            s += sep;
   //            s += temp;
   //         } else {
   //            s += temp;
   //         }
   //      }

   //      return s;
   //   }
   //   inline std::string toString( const std::vector<std::string>& lines  ) {
   //      const std::string sep = "\n";
   //      std::string s;

   //      for( auto temp : lines ) {
   //         if( !s.empty() ) {
   //            s += sep;
   //            s += temp;
   //         } else {
   //            s += temp;
   //         }
   //      }

   //      return s;
   //   }


}// end of namespace

#endif

//EOF

