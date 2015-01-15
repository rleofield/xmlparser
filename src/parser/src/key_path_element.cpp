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


#include <vector>

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <typeinfo>
#include <algorithm>
#include <boost/lexical_cast.hpp>



#include "xml_utl.h"
#include "keyentries.h"
#include "xml_node.h"
#include "xml_fs.h"
#include "xml_exception.h"
#include "xml_interface.h"
#include "xml_element.h"

#include "stringhelper.h"

using namespace std;

namespace txml {





   namespace {
      // get numbers
      bool hasnumbersonly( string const& str ) {
         if( str.empty() ) {
            return false;
         }

         for( auto ch : str ) {
            if( !isdigit( ch ) ) {
               return false;
            }
         }

         // has only digits
         return true;
      }
   }

   const string path_element::element_count_left_bracket = "[";
   const string path_element::element_count_right_bracket = "]";
   const string path_element::element_attr_separator = ":";
   const string path_element::element_separator = ".";


   // element[count]:attr
   path_element::path_element( string const& val ): _element(), _attr(), _childcount( 0 ), _value(), _node( nullptr ) {

      string index;
      string selement;
      size_t bracket_left_pos  = val.find( element_count_left_bracket );
      size_t bracket_right = val.find( element_count_right_bracket );


      // find elem, if path_element has brackets
      if( bracket_left_pos  != string::npos
            &&
            bracket_right != string::npos
            &&
            bracket_left_pos < bracket_right ) {
         size_t l = bracket_right - bracket_left_pos - 1;
         index = val.substr( bracket_left_pos + 1, l );
         selement = val.substr( 0, bracket_left_pos );
      }

      // v[0] = elem
      // v[1] = attr
      vector<string> v = rlf_hstring::split( val, element_attr_separator );

      if( v.size() == 0 ) {
         return;
      }

      if( bracket_left_pos != string::npos ) {
         _element =  selement;
         _childcount = xmlinterface::to_int( index );
      } else {
         _element =  v[0];
      }

      if( v.size() == 1 ) {
         return;
      }

      string  attr_  = v[1];
      // an attribute has at least one alphachar
      // then it is childcount
      bool b1 = hasnumbersonly( attr_ );

      if( b1 ) {
         _childcount = xmlinterface::to_int( attr_ );
      } else {
         _attr = path_attr(attr_);
      }

      // never happens, future use
      if( v.size() > 2 ) {
         attr_ = v[2];
         // an attribute has at least one alphachar
         // then it is childcount
         bool b2 = hasnumbersonly( attr_ );

         if( b2 ) {
            _childcount = xmlinterface::to_int( attr_ );
         } else {
            _attr = attr_;
         }
      }


   }


   path_element::path_element():
      _element(),
      _attr(),
      _childcount( 0 ),
      _value(),
      _node( nullptr ) {}

   path_element::path_element( path_element const& ke ):
      _element( ke._element ),
      _attr( ke._attr ),
      _childcount( ke._childcount ),
      _value( ke._value ),
      _node( ke._node ) {}

   path_element& path_element::operator=( path_element const& ke ) {
      if( this != &ke ) {
         _element = ke._element;
         _attr = ke._attr;
         _childcount = ke._childcount;
         _value = ke._value;
         _node = ke._node;
      }

      return *this;
   }

   string path_element::toValue()const {
      xml_element const* n =  dynamic_cast<xml_element const*>( _node );
      string val;

      if( n != nullptr ) {
         val =  n->unencoded_text();
         string a = attr();

         if( !a.empty() ) {
            val = n->attribute( a );
         }
      }

      return val;
   }

   // compares elements and attribute
   bool path_element::operator==( path_element const& pe )const {
      if( _element == pe._element && _attr == pe._attr && _childcount == pe._childcount) {
         return true;
      }

      return false;
   }

   int path_element::childcount()const {
      return _childcount;
   }
   void path_element::childcount( int chc ) {
      _childcount = chc;
   }

   path_attr path_element::attr()const {
      return _attr;
   }
   void path_element::attr( path_attr const& a ) {
      _attr = a;
   }
   bool path_element::points_to_attr()const {
      return !_attr.empty();
   }
   void path_element::value( string const& v ) {
      _value = v;
   }
   string path_element::elementname()const {
      return _element;
   }
   void path_element::elementname( string element_ ) {
      _element = element_;
   }
   xml_node const* path_element::node()const {
      return _node;
   }
   xml_node* path_element::node() {
      return _node;
   }
   void path_element::node( xml_node* n ) {
      _node = n;
   }
   // for default inserter
   void path_element::removeAttr() {
      _attr = path_attr();
   }

   // childcount > 0 : element[count]:attr
   // childcount = 0 :
   //   element:attr
   string path_element::to_string()const {
      if( _childcount > 0 ) {
         string chcount = boost::lexical_cast<string>( _childcount );
         string element1 = _element + element_count_left_bracket + chcount + element_count_right_bracket;
         if( _attr.empty() ) {
            // elem [ch_count]
            return element1;
         }

         // elem[ch_count]:attr
         return element1 + element_attr_separator + _attr.name();
      }

      // elem
      if( _attr.empty() ) {
         return _element;
      }

      // elem:attr
      return _element + element_attr_separator + _attr.name();
   }



}



//EOF

