/*
www.sourceforge.net/projects/tinyxml
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:
#include "xml_exception.h"

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

#ifndef RLF_INTERFACE_H
#define RLF_INTERFACE_H

#include <string>



/*

Txml kann einen XML File parsen und Werte ändern.
Der Zugriff auf ein einzelnes XML Element erfolgt mit Keys, die Xpath angelehnt sind,
aber viel einfacher als Xpath zu handhaben sind.


*/


namespace txml {
   class tXmlInterfaceImpl;
   class xml_document;
}


namespace xmlinterface {


   class tXmlInterface {

      tXmlInterface( tXmlInterface const& );
      void operator=( tXmlInterface const& );

      txml::tXmlInterfaceImpl* impl;

   public:
      tXmlInterface();
      ~tXmlInterface();

      // create a root xml structure in  memory
      void create_root( std::string const& name, std::string const& root );

      // create an element with default
      void create( std::string const& key, std::string const& value );

      // create an attribute with default, the element must exist
      void attr( std::string const& key, std::string const& value );



      // get text value of an element, and get as int or get as double
      std::string get_string( std::string const& key )const ;
      int get_int( std::string const& key )const ;
      double get_double( std::string const& key )const ;


      // write text value if an element
      // key must point to an element
      void set_string( std::string const& key, std::string const& value );
      void set_int( std::string const& key, int value );
      void set_double( std::string const& key, double value );


      // write an attribute
      // key must point to an element
      void set_attribute( std::string const& key, std::string const& value );
      void set_int_attribute( std::string const& key, int value );
      void set_double_attribute( std::string const& key, double value );

      // set comment before element marked by key
      // doc or fiile must parsed before
      void set_comment( std::string const& key, std::string const& comment );

      // parse xml file, filename is stored internally
      void parse( std::string const& fn );

      // parse document to internal structure
      void rebuild( txml::xml_document const& doc );

      // write to file
      void save( std::string const& fn );
      // write to file, uses internal name
      void save();



      // list of all keys, with childnumber, für elements and attributes
      void print_keys( std::string const& fn )const;

      // keys and values
      void print_keys_and_values( std::string const& fn, size_t w = 60 )const;

      // dto. als C++ source code to generate a new file with defaults
      // parameter:
      //     fn_keys - keyfile for the generated C++ code
      //     fn_out  - XML File which will be generated after executing the code

      void print_creator( std::string const& fn_keys, std::string const& fn_out )const;



      txml::xml_document* document()const;

   };


   void show_alloc_list();
   std::string allocliststring();

   // small helpers
   int to_int( std::string const& s );
   double to_double( std::string const& s );
   std::string int_to_string( int val );
   std::string double_to_string( double val );


   std::string replace_all( std::string const& ins, const std::string& pattern, const std::string& replace ) ;
   size_t index( std::string const& s, std::string const& pattern, size_t pos ) ;
   std::string trim( std::string const& str, char ch = ' ' ) ;


   std::string fillup( std::string const& in, char ch, size_t n ) ;
   std::string to_string( double val ) ;
   std::string to_string( int val ) ;
   int to_int( std::string const& s );
   double to_double( std::string const& s, std::locale l ) ;
   double to_double( std::string const& s ) ;


} // end of ns


#endif //

// EOF
