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

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <typeinfo>



#include "xml_utl.h"
#include "xml_fs.h"
#include "XmlException.h"
#include "XmlStrings_.h"
#include "win32.h"
#include "stringhelper.h"


using namespace std;

namespace txml_strings {





   size_t index( string const& s, string const& pattern, size_t pos ) {
      int inLength = static_cast<int>(pattern.length());
      int posInLength = static_cast<int>(pos + pattern.length());
      int l = static_cast<int>(s.length() + 1);

      if( s.length() > 0 && inLength > 0 && posInLength < l ) {
         size_t si = s.find( pattern, pos );
         return si;
      }

      return string::npos;
   }



   std::string to_string( size_t val ) {
      return to_string( ( int )val );
   }

   std::string to_string( int val ) {
      try {
         return rlf_hstring::trim( boost::lexical_cast<string>( val ) );
      } catch( boost::bad_lexical_cast& e ) {
         string msg = e.what();
         throw txml::XmlException( txml::t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_bad_lexical_cast, "msg: " + msg );
      }

   }

   std::string to_string( double val )  {
      try {
         return rlf_hstring::trim( boost::lexical_cast<string>( val ) );
      } catch( boost::bad_lexical_cast& e ) {
         string msg = e.what();
         throw txml::XmlException( txml::t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_bad_lexical_cast, "msg: " + msg );
      }

   }

   int to_int( std::string const& s )  {
      try {
         return boost::lexical_cast<int>( s );
      } catch( boost::bad_lexical_cast& e ) {
         string msg = e.what();
         throw txml::XmlException( txml::t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_bad_lexical_cast, "msg: " + msg );
      }

   }

   double to_double( std::string const& s )  {
      try {
         double b = boost::lexical_cast<double>( rlf_hstring::trim( s ) );
         return b;
      } catch( boost::bad_lexical_cast& e ) {
         string msg = e.what();
         throw txml::XmlException( txml::t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                   txml::enum_bad_lexical_cast, "string : " + s + ", msg: " + msg );
      }
   }

   // converts a hex string to size_t
   size_t hex_to_size_t( std::string const& s )  {
      size_t x;
      std::istringstream insx( s );
      insx >> std::hex;
      insx >> x;
      return x;
   }

} // end of namespace strings



