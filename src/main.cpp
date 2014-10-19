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

Linux User Group Ottobrunn / München Südost (LOMSO)
www.lug-ottobrunn.de

*/

#include <iostream>

#include "tLog_Category_default.h"

// demos
#include "xml_interface.h"
#include "xml_exception.h"



#include "examples.h"

using namespace std;
using namespace rlf_tlog;

template <typename T>
void cpp11_value_initialization() {
   T t {}; // value-initialize
   int v {};
}



int main() {



   cout << "started" << endl;

   string logfile = "xmltest";
   bool b = logger().setLogfile( logfile );

   if( b == false ) {
      cout << "path for logging doesn't exist: '" << logfile <<  "'" << endl;
   }

   logger().setLogLevel( eLevel::LDEBUG, eCategory::_default );
   logger().setLogLevel( eLevel::LDEBUG, eCategory::Cat_A );



   LOGT_INFO( " Start1  " );

   try {

      examples::ubuntu32_xml_demo();
      // demo create xml file, please comment out unwanted tests


      // other demos, in demo.cpp

      // test of some string functions
      examples::string_convert();

      // print a nice xml format
      examples::pretty_print();

      examples::xml_path_to_string_const();

      // insert a comment before an element
      examples::add_comments();

      // read and set element values, attributes
      examples::get_element_attribute_values();
      examples::set_element_attribute_values();

   } catch( std::runtime_error& rt ) {
      LOGT_FATAL( string( "Ex rt: " ) + rt.what() );

   }

   xmlinterface::show_alloc_list();
   return 0;
}
