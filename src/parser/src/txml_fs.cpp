
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
- locator 'xml_locator' for simple searching/changing the xml document
- code simplified, most of the comments removed, code is self explaning
- class 'rawxml_position' for moving through a vector<char>,
- logger (can be used separately)
- some help methods for strings (can be used separately)
- check of new/delete (code is a small howto overload new/delete example)
- an example, how to use  the parser in your code is in 'main.cpp'

www.rleofield.de

Linux User Group Ottobrunn / M�nchen S�dost (LOMSO)
www.lug-ottobrunn.de
*/


#include <vector>

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <typeinfo>



#include "xml_fs.h"

#include <boost/filesystem.hpp>


using namespace std;


namespace txml {


   namespace fs {
      namespace {
         bool file_exists( boost::filesystem::path const& p ) {
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

      bool file_exists( string const& fn ) {
         boost::filesystem::path p( fn );
         return file_exists( p );
      }
      string extension( string const& fn ) {
         boost::filesystem::path p( fn );
         string ext = p.extension().string();
         return ext;
      }

      void to_upper_in_place( string& s ) {
         string::iterator start = s.begin();

         while( start != s.end() ) {
            *start = ( char )::toupper( *start );
            ++start;
         }
      }


      void to_lower_in_place( string& s ) {
         string::iterator start = s.begin();

         while( start != s.end() ) {
            *start = ( char )::tolower( *start );
            ++start;
         }
      }

      string to_lower( string const& s ) {
         string temp = s;
         to_lower_in_place( temp );
         return temp;
      }

   }


}

//EOF
