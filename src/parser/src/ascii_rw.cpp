/*
www.sourceforge.net/projects/tinyxml
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.


Modified by Richard Albrecht:
- adapted, using more C++
- locator 'XmlLocator' for simple searching/changing the xml document
- code simplified, most of the comments removed, code is self explaning
- class 'rawxml_position' for moving through a vector<char>,
- logger (can be used separately)
- some help methods for strings (can be used separately)
- check of new/delete (code is a small howto overload new/delete example)
- an example, how to use  the parser in your code is in 'main.cpp'

www.rleofield.de

Linux User Group Ottobrunn / München Südost (LOMSO)
www.lug-ottobrunn.de
*/


#include <vector>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <typeinfo>



#include "xml_utl.h"
#include "xml_fs.h"
#include "ascii_rw.h"


using namespace std;


namespace ascii {

   namespace {
      class writer {
         std::ofstream& _fp;
      public:
         writer( std::ofstream& fp ): _fp( fp ) {}
         void operator()( std::string const& s ) {
            _fp << s.c_str() << std::endl;

            if( _fp.bad() ) {
               // -- replace this line with your error message
               throw BadAsciiWrite( "file  is bad" );
            }
         }
      };
      class writerout {

      public:
         writerout( ) {}
         void operator()( std::string const& s ) {
            std::cout << s.c_str() << std::endl;
         }
      };
   }

   void tReadText::operator()( vector<string> &l, std::string const& file )  {
      list<string> li;
      operator()( li, file );
      l.assign( li.begin(), li.end() );
   }
   void tReadText::operator()( list<string> &l, std::string const& file )  {
      if( !txml::fs::file_exists( file ) ) {
         throw BadAsciiRead( std::string( "file does not exist: " ) + file );
      }

      std::ifstream fp( file.c_str() );

      if( fp.bad() ) {
         std::string s = "file couldn't be read: ";
         s += file;
         throw BadAsciiRead( s );
      }

      while( !fp.eof() ) {
         std::string temp;
         std::getline( fp, temp );

         if( !fp.fail() ) {
            l.push_back( temp );
         } else {
            if( !fp.eof() ) {
               throw BadAsciiRead( std::string( "unexpected EOF in file: " ) + file );
            }
         }
      }
   }


   size_t tWriteText::operator()( vector<std::string> const& lines , const std::string& file ) {
      list<string> l( lines.begin(), lines.end() );
      operator()( l, file );
      return l.size();
   }


   size_t tWriteText::operator()( std::list<std::string> const& lines , const std::string& file ) {
      if( lines.size() > 0 ) {
         std::ofstream fp( file.c_str() );

         if( fp.bad() ) {
            throw BadAsciiWrite( "file: " + file + " is bad found." );
         }

         for_each( lines.begin(), lines.end(), writer( fp ) );
      }

      return lines.size();
   }


}




//EOF

