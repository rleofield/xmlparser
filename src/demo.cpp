
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


#include <tLog_Category_default.h>

// demos
#include "xml_interface.h"
#include "xml_exception.h"



using namespace std;
using namespace rlf_tlog;

namespace examples {

   const string xml_ubuntu32_Test = "ubuntu32.xml";
   const string xml_ubuntu32_TestPrint = "ubuntu32_print.xml";
   const string xml_ubuntu32_TestOut = "ubuntu32_test_out.xml";
   const string xml_ubuntu32_KeysOut = "ubuntu32_keys.txt";
   const string xml_ubuntu32_SourceOut = "ubuntu32_sourcecode.txt";
   const string xml_ubuntu32_DemoIn = "ubuntu32.xml";
   const string xml_ubuntu32_DemoOut = "ubuntu32_demo_out.xml";



   //#include "logger.h"
   /////////////////////////
   // demo string convert
   /////////////////////////

   // helper, to convert string results to int/double
   // helper, to convert double/int to string for xml input
   void string_convert() {


      // please use the debugger/logger to check the behavior

      // conversions,  static

      // to string
      string siabc = xmlinterface::int_to_string( -127777 );
      string sdabc = xmlinterface::double_to_string( -127.777 );
      LOGT_INFO( "t.IntToString( -127777 ) = " + siabc );
      LOGT_INFO( "t.DoubleToString( -127.777 ); = " + sdabc );

      // from string

      int iabc = xmlinterface::to_int( "-127777" );
      double dabc = xmlinterface::to_double( "-127777.888 " );
      LOGT_INFO( "t.str_to_int( \"  -127777 int variable as string\" ); = " + xmlinterface::to_string( iabc ) );
      LOGT_INFO( "t.str_to_double( \" -127777.888 \" ); = " + xmlinterface::to_string( dabc ) );



   }


   /////////////////////////
   // demo formatting a xml file
   /////////////////////////


   // format xml file
   void pretty_print() {



      // make instance
      xmlinterface::tXmlInterface t;

      // parse first the unformatted XML file
      t.parse( xml_ubuntu32_Test );

      // save the file nicely formatted
      t.save( xml_ubuntu32_DemoOut );

      // for the debugger break point
      LOGT_INFO( "" );
   }



   void xml_path_to_string_const() {

      // please comment out unwanted caes

      // make instance
      xmlinterface::tXmlInterface t;


      // parse first
      t.parse( xml_ubuntu32_Test );

      // write list of internal keys to file
      t.print_keys( xml_ubuntu32_KeysOut );

      // write this list with small formatting
      t.print_keys_and_values( xml_ubuntu32_KeysOut );

      // write the list as string consts for direct use in Sourcecode
      t.print_creator( xml_ubuntu32_KeysOut, xml_ubuntu32_DemoOut );

      LOGT_INFO( "" );

   }

   void add_comments() {

      // make instance
      xmlinterface::tXmlInterface t;

      // parse first
      t.parse( xml_ubuntu32_DemoIn );

      try {
         {
            // add a comment before element
            const string domain_devices = "domain.devices";  //   element:    = '';  //   element:    = 'devices'
            t.set_comment( domain_devices, "this is the device" );

            // save to disk
            t.save( xml_ubuntu32_DemoOut );
            LOGT_INFO( "" );
         }
         // set second comment
         {
            const string domain_devices_interface__1 = "domain.devices.interface[1]";  //   element:    = '860'
            t.set_comment( domain_devices_interface__1, "text at 2.position of element 'interface'" );
            t.save( xml_ubuntu32_DemoOut );
         }
      } catch( txml::xml_exception& ex ) {
         string w = ex.What();
         LOGT_INFO( ex.what() );

      }

      // save to disk
      LOGT_INFO( "" );

   }



   void get_element_attribute_values() {

      // make instance
      xmlinterface::tXmlInterface t;


      // parse first
      try {

         t.parse( xml_ubuntu32_DemoIn );

      } catch( txml::xml_exception& ex ) {
         string w = ex.What();
         LOGT_INFO( ex.what() );

      }

      string msg;
      string element1;

      try {
         //test read element;
         const string domain_on_reboot = "domain.on_reboot";  //   element:    = 'restart'
         element1 = t.get_string( domain_on_reboot );

      } catch( txml::xml_exception& ex ) {
         string w = ex.What();
         LOGT_INFO( ex.what() );

      }

      LOGT_INFO( "" );

      try {
         // test reading an element, with default and unknown key
         // the unknown node is inserted at end of node 'domain'
         const string domain_on_reboot_new = "domain.on_reboot_new";
         element1 = t.get_string( domain_on_reboot_new );
         t.set_string( domain_on_reboot_new, "00000domain" );

         // save changes
         t.save( xml_ubuntu32_DemoOut );

         LOGT_INFO( domain_on_reboot_new + ",  element: " + element1 );


      }

      catch( txml::xml_exception& ex ) {
         string w = ex.What();
         LOGT_INFO( ex.what() );

      }

      LOGT_INFO( "" );

      // reading an attribute, notice the : in the key as separator
      string attribute;

      try {
         // get attribute
         const string domain_clock__offset = "domain.clock:offset";
         attribute = t.get_string( domain_clock__offset );

         // value of attribute = 'utc'
         LOGT_INFO( domain_clock__offset + ",  attr: " + attribute );
      } catch( txml::xml_exception& ex ) {
         string w = ex.What();
         LOGT_INFO( ex.what() );

      }

      LOGT_INFO( "" );
   }





   void set_element_attribute_values() {

      // make instance
      xmlinterface::tXmlInterface t;

      string temp ;

      // parse first
      t.parse( xml_ubuntu32_DemoIn );

      try {
         // generate the keys
         t.print_keys( xml_ubuntu32_KeysOut );

         const string domain_clock__offset = "domain.clock:offset";
         string newValue = "set_element_attribute_values(), test, new test clock, seond try";
         t.set_attribute( domain_clock__offset, newValue );

         // save to disk
         t.save( xml_ubuntu32_DemoOut );

         // parse again for control
         t.parse( xml_ubuntu32_DemoOut );

         // get control attribute
         temp = t.get_string( domain_clock__offset );

         LOGT_INFO( domain_clock__offset + " ,  " + temp );

      } catch( txml::xml_exception& ex ) {
         string w = ex.What();
         LOGT_INFO( ex.what() );

      }

      LOGT_INFO( "" );


   }


} // end of ns examples


