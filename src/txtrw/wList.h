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


#ifndef T_TEXT_WRITE
#define T_TEXT_WRITE


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



//#include "helper.h"

//using namespace helper_read_write_file;

namespace rlf_txtrw {



   namespace whelper {
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
            return FindAndReplace( msg, whelper::marker, s0 );
         }

         return msg;

      }




   }


   namespace err {

      const std::string msg_file_exists = "File exists: '" + whelper::marker + "'";
      const std::string msg_write_file = "Couldn't write file: '" + whelper::marker + "'";
      const std::string msg_open_file = "Couldn't open file for write: '" + whelper::marker + "'";
      const std::string msg_text_empty = "Text has no lines: '" + whelper::marker + "'";

      inline std::string write_file( std::string const& s0 ) {
         return whelper::replace( msg_write_file, s0 );
      }
      inline std::string file_exists( std::string const& s0 ) {
         return whelper::replace( msg_file_exists, s0 );
      }
      inline std::string file_open( std::string const& s0 ) {
         return whelper::replace( msg_open_file, s0 );
      }
      inline std::string text_empty( std::string const& s0 ) {
         return whelper::replace( msg_text_empty, s0 );
      }
      inline bool file_exists_( boost::filesystem::path const& p ) {
         if( !boost::filesystem::is_regular_file( p ) ) {
            return false;
         }

         boost::filesystem::file_status s = status( p );

         if( boost::filesystem::exists( s ) ) {
            return true;
         }

         return false;
      }

   }


   /*! bad_text_write,
      Exception, if text write fails
      \param [in] msg  error message
      */
   class bad_text_write: public std::runtime_error {
   public:
      bad_text_write( const std::string& msg )
         : std::runtime_error( msg ) { }
   };


   /*! \class t_write_ascii
       *  \brief writes text file
       *
       * a textfile is stored in a <b>string</b> list<br>
       */
   class t_write_ascii  {
      t_write_ascii( const t_write_ascii& in );
      t_write_ascii& operator= ( const t_write_ascii& in );

   public:
      t_write_ascii() {}
      ~t_write_ascii() {}
      void operator()( const std::string& file, std::list<std::string> const& lines, bool overwrite = false ) {

         if( ! overwrite ) {
            if( err::file_exists_( file ) ) {
               throw bad_text_write( err::file_exists( file ) );
            }
         }

         //         if( lines.size() == 0 ) {
         //            throw bad_text_write( err::text_empty( file ) );
         //         }

         if( lines.size() > 0 ) {
            std::ofstream fp( file.c_str() );

            if( !fp.is_open() ) {
               throw bad_text_write( err::file_open( file ) );
            }

            try {
               for_each( lines.begin(), lines.end(), writer( fp, file ) );
            } catch( bad_text_write& ex ) {
               throw bad_text_write( err::write_file( file + ex.what() ) );
            }
         }
      }
      void operator()( const std::string& file, std::string const& str, bool overwrite = false ) {

         if( ! overwrite ) {
            if( err::file_exists_( file ) ) {
               throw bad_text_write( err::file_exists( file ) );
            }
         }

         if( str.size() == 0 ) {
            throw bad_text_write( err::text_empty( file ) );
         }

         if( str.size() > 0 ) {
            std::ofstream fp( file.c_str() );

            if( !fp.is_open() ) {
               throw bad_text_write( err::file_open( file ) );
            }

            try {
               writer( fp, file )( str );
            } catch( bad_text_write& ex ) {
               throw bad_text_write( err::write_file( file + ex.what() ) );
            }
         }
      }
      class writer {
         std::ofstream& _fp;
         std::string const& _f;
      public:
         writer( std::ofstream& fp, std::string f ): _fp( fp ), _f( f ) {}
         void operator()( std::string const& s ) {
            _fp << s << std::endl;

            if( _fp.bad() ) {
               throw bad_text_write( "" );
            }
         }
      };


   };

}// end of ns text_write

#endif

//EOF

