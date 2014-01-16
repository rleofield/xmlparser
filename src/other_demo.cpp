
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

#include <iostream>

// demos
#include "xml_interface.h"
#include "logger.h"
#include "XmlException.h"



using namespace std;


namespace examples {


   std::string xmlTest = "CC.xml";

   const std::string outxml = "out.xml";
   const std::string outkeys = "keys.txt";
   const std::string outcode = "code.txt";









   void xml_path_to_string_const() {

      // make instance
      xmlinterface::tXmlInterface t;

      xmlTest = "CC.xml";
      // parse first
      t.parse( xmlTest );

      // write list of internal keys to file
      t.print_keys( outkeys );

      // write this list with small formatting
      t.print_keys_and_values( outkeys, 120 );

      // write the list as string consts for direct use in Sourcecode
      //  t.PrintKeysAsConstStdStrings( outcode );

      LOGT_A_INFO( "" );

   }

   void get_element_attribute_values() {

      // make inatance
      xmlinterface::tXmlInterface t;
      xmlTest = "CC.xml";

      // parse first
      try {
         t.parse( xmlTest );
      } catch( txml::XmlException& ex ) {
         std::string w = ex.What();
         LOGT_INFO( ex.what() );

      }

      string element1;

      try {
         // test reading an element, with default and unknown key
         std::string msg;

         // simulate a wrong key.
         string key1 = ""; // ecpected:  element: '/usr/bin/kvm'

         const std::string CutControl_SW_DebugMode__typpe = "CutControl.SW.DebugmMode";  //   attribute:  = 'testvalue_type'

         key1 = CutControl_SW_DebugMode__typpe;

         try {
            element1 = t.get_string( key1, "xen" );
         } catch( txml::XmlException& e ) {
            msg = e.What();
            LOGT_INFO( msg );

         }

         t.save( "out1.xml" );
         // result:
         // a new element node ist inserted with value 'xen'.
         LOGT_INFO( key1 + ",  element: " + element1 );
      } catch( txml::XmlException& ex ) {
         std::string w = ex.What();
         LOGT_INFO( ex.what() );

      }


      // reading an attribute, notice the : in the key as separator
      string attribute3;

      try {
         // get attribute
         string key3 = "domain.devices.interface.mac:address"; // -> attribute: '52:54:00:6c:47:37'
         attribute3 = t.get_string( key3 );
         // resukt:
         // value of attribute = '52:54:00:6c:47:37'
         LOGT_INFO( key3 + ",  attr: " + attribute3 );
      } catch( txml::XmlException& ex ) {
         std::string w = ex.What();
         LOGT_INFO( ex.what() );

      }

      string attribute4;

      try {
         string key4 = "domain.devices.console.target:typ"; // error, attribute doesn't exist
         attribute4 = t.get_string( key4 );

         if( attribute4.empty() ) {
            LOGT_INFO( key4 + ",  nicht gefunden" );
         }

         LOGT_INFO( key4 + ",  attr: " + attribute4 );

      } catch( txml::XmlException& ex ) {
         std::string w = ex.What();
         LOGT_INFO( ex.what() );

      }


      LOGT_INFO( "" );
   }





   void set_element_attribute_values() {

      // make instance
      xmlinterface::tXmlInterface t;

      xmlTest = "test.xml";

      // parse first
      t.parse( xmlTest );

      try {
         t.print_keys( "keys.txt" );

         string key1 = "domain.devices.interface.mac:address";
         string value = "52:54:00:6c:77:77 test test jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjttttt";
         t.set_attribute( key1, value );

         // save to disk
         t.save( "changed.xml" );

         // parse again for control
         t.parse( "changed.xml" );

         // get control attribute
         string temp = t.get_string( key1 );

         LOGT_INFO( key1 + " ,  " + temp );

      } catch( txml::XmlException& ex ) {
         std::string w = ex.What();
         LOGT_INFO( ex.what() );

      }

      try {

         // set a string element value
         string key2 = "domain.memory";
         string value =  "838608";
         t.set_string( key2, value );

         // test the same with a double value
         t.set_double( key2, 1234.567 );

         // read and test
         string temp = t.get_string( key2 );

         LOGT_INFO( key2 + " ,  " + temp );

      } catch( txml::XmlException& ex ) {
         std::string w = ex.What();
         LOGT_INFO( ex.what() );

      }

      // save to disk
      t.save( outxml );
      LOGT_INFO( "" );


   }



} // end of ns examples


