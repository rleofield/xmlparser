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


#ifndef RLF_T_TEXT_WRITE_H
#define RLF_T_TEXT_WRITE_H


/*! \file wList.h
 *  \brief class t_write_ascii, writes a text file
 *  \author Richard Albrecht
 */



#include <fstream>
#include <list>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/cstdint.hpp>



namespace rlf_txtrw {

   const std::string marker = "%s";

   namespace rlf_txtrw_helper{

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
            return FindAndReplace( msg, marker, s0 );
         }
         return msg;
      }




      const std::string msg_file_exists = "File exists: '" + marker + "'";
      const std::string msg_write_file = "Couldn't write file: '" + marker + "'";
      const std::string msg_open_file = "Couldn't open file for write: '" + marker + "'";
      const std::string msg_text_empty = "Text has no lines: '" + marker + "'";

      inline std::string write_file( std::string const& s0 ) {
         return replace( msg_write_file, s0 );
      }
      inline std::string file_exists( std::string const& s0 ) {
         return replace( msg_file_exists, s0 );
      }
      inline std::string file_open( std::string const& s0 ) {
         return replace( msg_open_file, s0 );
      }
      inline std::string text_empty( std::string const& s0 ) {
         return replace( msg_text_empty, s0 );
      }
      inline bool file_exists( boost::filesystem::path const& p ) {
         if( !boost::filesystem::is_regular_file( p ) ) {
            return false;
         }
         boost::filesystem::file_status s = status( p );
         if( boost::filesystem::exists( s ) ) {
            return true;
         }
         return false;
      }

      } // end of ns



   /*! \class t_write_ascii_list
       *  \brief writes text file from list
       *
       * a textfile is stored in a <b>string</b> list<br>
       */
   class t_write_text  {
         t_write_text( const t_write_text& in ) ;
         t_write_text& operator= ( const t_write_text& in );


         // list
         void operator()( const std::string& file, std::list<std::string> const& lines, bool overwrite = false ) {

            if( ! overwrite ) {
               if( rlf_txtrw_helper::file_exists( boost::filesystem::path(file) ) ) {
                  throw std::runtime_error( rlf_txtrw_helper::file_exists( file ) );
               }
            }

            if( lines.size() > 0 ) {
               std::ofstream fp( file.c_str() );

               if( !fp.is_open() ) {
                  throw std::runtime_error( rlf_txtrw_helper::file_open( file ) );
               }

               try {
                  for_each( lines.begin(), lines.end(), writer( fp ) );
               } catch( std::runtime_error& ex ) {
                  throw std::runtime_error( rlf_txtrw_helper::write_file( file + ex.what() ) );
               }
            }
         }

         // vector
         void operator()( const std::string& file, std::vector<std::string> const& lines, bool overwrite = false ) {

            if( ! overwrite ) {
               if( rlf_txtrw_helper::file_exists( boost::filesystem::path(file) ) ) {
                  throw std::runtime_error( rlf_txtrw_helper::file_exists( file ) );
               }
            }

            if( lines.size() > 0 ) {
               std::ofstream fp( file.c_str() );

               if( !fp.is_open() ) {
                  throw std::runtime_error( rlf_txtrw_helper::file_open( file ) );
               }

               try {
                  for_each( lines.begin(), lines.end(), writer( fp ) );
               } catch( std::runtime_error& ex ) {
                  throw std::runtime_error( rlf_txtrw_helper::write_file( file + ex.what() ) );
               }
            }
         }

         // string
         void operator()( const std::string& file, std::string const& str, bool overwrite = false ) {

            if( ! overwrite ) {
               if( rlf_txtrw_helper::file_exists( boost::filesystem::path(file) ) ) {
                  throw std::runtime_error( rlf_txtrw_helper::file_exists( file ) );
               }
            }

            if( str.size() > 0 ) {
               std::ofstream fp( file.c_str() );

               if( !fp.is_open() ) {
                  throw std::runtime_error( rlf_txtrw_helper::file_open( file ) );
               }
               if( fp.bad() ) {
                  throw std::runtime_error( "" );
               }
               try {
                  writer w( fp );
                  w( str );
               } catch( std::runtime_error& ex ) {
                  throw std::runtime_error( rlf_txtrw_helper::write_file( file + ex.what() ) );
               }
            }
         }
         class writer {
               std::ofstream& _fp;
            public:
               writer( std::ofstream& fp ): _fp( fp ) {}
               void operator()( std::string const& s ) {
                  _fp << s << std::endl;
                  if( _fp.bad() ) {
                     throw std::runtime_error( "" );
                  }
               }
         };

      public:
         t_write_text(const std::string& file, std::list<std::string> const& lines, bool overwrite = false){
            operator()( file, lines, overwrite );
         }
         t_write_text(const std::string& file, std::vector<std::string> const& lines, bool overwrite = false){
            operator()( file, lines, overwrite );
         }
         t_write_text(const std::string& file, std::string const& lines, bool overwrite = false){
            operator()( file, lines, overwrite );
         }
         ~t_write_text() {}

   };


   }// end of ns text_write

#endif

//EOF

