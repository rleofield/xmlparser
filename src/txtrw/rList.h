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

#ifndef RLF_T_TEXT_READ_H
#define RLF_T_TEXT_READ_H


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
//      std::string clip( std::string const& s, size_t pos ) {
//         return s.substr( 0, pos ) ;
//      }
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





   /*! \class t_text_read
   *  \brief reads a text file
   *
   * a text file is stored in a <b>string</b> list<br>
   */
//   class t_text_read_list  {

//      std::string _filename;

//   public:

//      t_text_read_list( const std::string& filename ): _filename( filename ) {}
//      t_text_read_list& operator= ( const t_text_read_list& in ) = delete;
//      t_text_read_list( const t_text_read_list& in ) = delete;
//      ~t_text_read_list() {}

//      void operator()( const std::string& filename, std::list<std::string>& lines  )  {
//         _filename = filename;
//         operator()( lines );
//      }

//      void operator()( std::list<std::string>& lines )  {
//         if( !err::file_exists_r( _filename ) ) {
//            std::string s = err::file_not_exists( _filename );
//            throw bad_text_read( s );
//         }

//         std::ifstream fp( _filename.c_str() );

//         if( !fp.is_open() ) {
//            std::string s = err::read_file( _filename );
//            throw bad_text_read( s );
//         }

//         while( !fp.eof() ) {
//            std::string temp;
//            getline( fp, temp );

//            if( !fp.fail() ) {
//               // DOS & Windows: \r\n 0d0a (hex), 13,10 (decimal)
//               // Unix & Mac OS X: \n, 0a, 10
//               // Macintosh (OS 9): \r, 0d, 13
//               // replace windows /r to ;inux \n
//               if( temp.back() == 0xd ){
//                   temp = rhelper::clip(temp, temp.size()-1);
//               }

//               lines.push_back( temp );
//            } else {
//               if( !fp.eof() ) {
//                  std::string s = err::read_file( _filename );
//                  throw bad_text_read( s );
//               }
//            }
//         }
//      }


//      operator std::list<std::string> () {
//         std::list<std::string> lines ;
//         *this = lines;
//         return std::move(lines);
//      }

//   private:
//      t_text_read_list& operator=( std::list<std::string>& lines )  {
//         operator()(_filename,lines);
//         return *this;
//      }

//   };

   class t_text_read_string  {

      std::string _filename;

   public:

      //t_text_read_string() = default;
      t_text_read_string( const std::string& filename ): _filename( filename ) {}
      t_text_read_string& operator= ( const t_text_read_string& in ) = delete;
      t_text_read_string( const t_text_read_string& in ) = delete;
      ~t_text_read_string() {}
private:
      void operator()( const std::string& filename, std::string& str )  {
         _filename = filename;
         operator()( str );
      }
private:
      void operator()( std::string& str )  {

         if( !err::file_exists_r( _filename ) ) {
            std::string s = err::file_not_exists( _filename );
            throw std::runtime_error( s );
         }

         std::ifstream fp( _filename.c_str() );

         if( !fp.is_open() ) {
            std::string s = err::read_file( _filename );
            throw std::runtime_error( s );
         }

         while( fp.good() ) {
            std::string temp;
            getline( fp, temp );

            if( !fp.fail() ) {
               // DOS & Windows: \r\n 0D0A (hex), 13,10 (decimal)
               // Unix & Mac OS X: \n, 0A, 10
               // Macintosh (OS 9): \r, 0D, 13
               // replace windows /r to Linux \n
               if( temp.back() == 0x0d ){
                  temp.back() = 0x0a;
               }
               str.append( temp );
            } else {
               if( !fp.eof() ) {
                  std::string s = err::read_file( _filename );
                  throw std::runtime_error( s );
               }
            }
         }
      }
public:
      operator std::string () {
         std::string lines;
         *this = lines;
         return lines;
      }
   private:
      t_text_read_string& operator=( std::string& lines )  {
         operator()(_filename,lines);
         return *this;
      }


   };

   template< typename T >
   class text_read_t  {
      text_read_t& operator= ( const text_read_t& in );
      text_read_t( const text_read_t& in );


   public:

      text_read_t() = default;
      ~text_read_t() {}

      T operator()( const std::string& filename )  {
         if( !err::file_exists_r( filename ) ) {
            std::string s = err::file_not_exists( filename );
            throw std::runtime_error( s );
         }

         std::ifstream fp( filename.c_str() );

         if( !fp.is_open() ) {
            std::string s = err::read_file( filename );
            throw std::runtime_error( s );
         }

         T lines;
         while( !fp.eof() ) {
            std::string temp;
            getline( fp, temp );

            if( !fp.fail() ) {
               lines.push_back( temp );
            } else {
               if( !fp.eof() ) {
                  std::string s = err::read_file( filename );
                  throw std::runtime_error( s );
               }
            }
         }
         return lines;
      }

   };

typedef text_read_t< std::vector<std::string>> aaa;

   class t_text_read  {
      t_text_read& operator= ( const t_text_read& in );
      t_text_read( const t_text_read& in );


   public:

      t_text_read() = default;
      ~t_text_read() {}

      std::vector<std::string> operator()( const std::string& filename )  {
         if( !err::file_exists_r( filename ) ) {
            std::string s = err::file_not_exists( filename );
            throw std::runtime_error( s );
         }

         std::ifstream fp( filename.c_str() );

         if( !fp.is_open() ) {
            std::string s = err::read_file( filename );
            throw std::runtime_error( s );
         }

         std::vector<std::string> lines;
         while( !fp.eof() ) {
            std::string temp;
            getline( fp, temp );

            if( !fp.fail() ) {
               lines.push_back( temp );
            } else {
               if( !fp.eof() ) {
                  std::string s = err::read_file( filename );
                  throw std::runtime_error( s );
               }
            }
         }
         return lines;
      }

   };



}// end of namespace

#endif

//EOF

