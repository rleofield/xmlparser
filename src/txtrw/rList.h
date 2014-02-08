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

#include "helper.h"

using namespace helper_read_write_file;

namespace rlf_txtrw {
   namespace err {
      const std::string msg_file_not_exists = "File doesn't exist: '" + marker + "'";
      const std::string msg_read_file = " Couldn't read file '" + marker + "'";

      inline std::string read_file( std::string const& s0 ) {
         return replace( msg_read_file, s0 );
      }
      inline std::string file_not_exists( std::string const& s0 ) {
         return replace( msg_file_not_exists, s0 );
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
   class t_text_read  {
         t_text_read& operator= ( const t_text_read& in );
         t_text_read( const t_text_read& in );


      public:

         t_text_read() {}
         ~t_text_read() {}

         void operator()( const std::string& filename, std::list<std::string> & lines )  {

            if( !file_exists( filename ) ) {
               std::string s = err::file_not_exists( filename );
               throw bad_text_read( s );
            }

            std::ifstream fp( filename.c_str() );

            if( fp.bad() ) {
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
         void operator()( const std::string& filename, std::string& str )  {

            if( !file_exists( filename ) ) {
               std::string s = err::file_not_exists( filename );
               throw bad_text_read( s );
            }

            std::ifstream fp( filename.c_str() );

            if( fp.bad() ) {
               std::string s = err::read_file( filename );
               throw bad_text_read( s );
            }

            while( !fp.eof() ) {
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
   // converts the output list to one string, with linebreaks
   inline std::string toString( const std::list<std::string>& lines ) {
      const std::string sep = "\n";
      std::string s;
      for (auto temp : lines ) {
         if( !s.empty() ) {
            s += sep;
            s += temp;
         } else {
            s += temp;
         }
      }
      return s;
   }


}// end of namespace

#endif

//EOF

