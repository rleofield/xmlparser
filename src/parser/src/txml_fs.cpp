
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

Linux User Group Ottobrunn / M�nchen S�dost (LOMSO)
www.lug-ottobrunn.de
*/
#ifdef _WIN32
#pragma warning( disable : 4291 ) // Warning   no matching operator delete found; memory will not be freed if initialization throws an exception
#pragma warning( disable : 4800 ) //
#pragma warning( disable : 4996 ) // Warning   This function or variable may be unsafe. Consider using localtime_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning( disable : 4804 ) // Warning   unsafe use of type 'bool' in operation
#pragma warning( disable:4996 4100) // _CRT_SECURE_NO_WARNINGS
#endif

#include <vector>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

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
         std::string::iterator start = s.begin();

         while( start != s.end() ) {
            *start = ( char )::toupper( *start );
            ++start;
         }
      }


      void to_lower_in_place( string& s ) {
         std::string::iterator start = s.begin();

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