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



#include "xml_utl.h"
#include "keyentries.h"
#include "xml_node.h"
#include "xml_fs.h"
#include "xml_exception.h"
#include "xml_interface.h"

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

   string path_element::left_bracket = "[";
   string path_element::right_bracket = "]";
   string path_element::double_colon = ":";

   path_element::path_element( string const& val ): _element(), _attr(), _childcount( 0 ), _value(), _node( 0 ) {

      string index;
      string element;
      size_t bracket_left  = val.find( left_bracket );
      size_t bracket_right = val.find( right_bracket );

      if( bracket_left  != string::npos
            &&
            bracket_right != string::npos
            &&
            bracket_left < bracket_right ) {
         size_t l = bracket_right - bracket_left - 1;
         index = val.substr( bracket_left + 1, l );
         element = val.substr( 0, bracket_left );
      }

      vector<string> v = rlf_hstring::split( val, double_colon );

      if( !v.empty() ) {
         if( bracket_left != string::npos ) {
            _element =  element;
            _childcount = xmlinterface::to_int( index );
         } else {
            _element =  v[0];
         }

         if( v.size() > 1 ) {
            string const& v1 = v[1];
            // an attribute has at least one alphachar
            bool b1 = hasnumbersonly( v1 );

            if( b1 ) {
               _childcount = xmlinterface::to_int( v1 );
            } else {
               _attr = v1;
            }

            if( v.size() > 2 ) {
               string const& v2 = v[2];
               // an attribute has at least one alphachar
               bool b2 = hasnumbersonly( v2 );

               if( b2 ) {
                  _childcount = xmlinterface::to_int( v2 );
               } else {
                  _attr = v2;
               }
            }
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

   // compares elements only
   bool path_element::operator==( path_element const& pe )const {
      if( _element == pe._element ) {
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

   string const& path_element::attr()const {
      return _attr;
   }
   void path_element::attr( string const& a ) {
      _attr = a;
   }
   bool path_element::is_attr()const {
      return !_attr.empty();
   }
   //   string const& keyentry::value()const {
   //      return _value;
   //   }
   void path_element::value( string const& v ) {
      _value = v;
   }
   string path_element::Element()const {
      return _element;
   }
   void path_element::Element( string element ) {
      _element = element;
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
      _attr = "";
   }

   string path_element::to_string()const {
      if( _childcount > 0 ) {
         string chcount = rlf_hstring::trim( rlf_hstring::toString( _childcount ) );

         if( _attr.empty() ) {
            return _element + left_bracket + chcount + right_bracket;
         }

         return _element + left_bracket + chcount + right_bracket + double_colon + _attr;
      }

      if( _attr.empty() ) {
         return _element;
      }

      return _element + double_colon + _attr;
   }

   path::path( string const& key ): _keyentries() {

      vector<string> v0 = rlf_hstring::split( key, "." );

      for( size_t i = 0; i < v0.size(); i++ ) {
         path_element ke( v0[i] );
         _keyentries.push_back( ke );
      }


   }

   path& path::operator=( string const& key ) {
      _keyentries.clear();
      vector<string> v0 = rlf_hstring::split( key, "." );

      for( size_t i = 0; i < v0.size(); i++ ) {
         path_element ke( v0[i] );
         _keyentries.push_back( ke );
      }

      return *this;
   }
   path::path(): _keyentries() {}

   bool path::operator>( path const& ke )const {
      return _keyentries.size() > ke._keyentries.size();
   }

   bool path::operator==( path const& k )const {
      return _keyentries == k._keyentries;
   }



   path& path::operator=( path const& ke ) {
      if( this != &ke ) {
         _keyentries = ke._keyentries;
      }

      return *this;
   }



   bool path::empty()const {
      return _keyentries.empty();
   }
   void path::addEmpty() {
      _keyentries.push_back( path_element() );
   }
   void path::add( path_element const& ke ) {
      _keyentries.push_back( ke );
   }
   void path::insert_front( path_element const& ke ) {
      _keyentries.insert( _keyentries.begin(),  ke );
   }

   size_t path::size()const {
      return _keyentries.size();
   }





   void path::remove_last() {
      _keyentries.erase( _keyentries.end() - 1 );
   }


   bool path::compareByElement( path const& v1 )const {
      if( _keyentries.size() != v1._keyentries.size() ) {
         return false;
      }

      for( size_t i = 0; i < _keyentries.size(); i++ ) {
         if( !( _keyentries[i] == v1._keyentries[i] ) ) {
            return false;
         }
      }

      return true;
   }
   bool path::compareByChildCount( path const& v1 ) const {
      if( _keyentries.size() != v1._keyentries.size() ) {
         return false;
      }

      for( size_t i = 0; i < _keyentries.size(); i++ ) {
         if( !( _keyentries[i].childcount() == v1._keyentries[i].childcount() ) ) {
            return false;
         }
      }

      return true;
   }



   namespace {
   }

   namespace {
      struct add {
         string s;
         string const& sep;
         add( string const& sep_ ): s(), sep( sep_ ) {}
         void operator()( string const& str ) {
            if( s.empty() ) {
               s += str;
               return;
            }

            s += sep;
            s += str;
         }
      };

      string merge( vector<string>  const& v, string const& sep ) {
         if( v.empty() ) {
            return string();
         }

         if( v.size() == 1 ) {
            return v[0];
         }

         return for_each( v.begin(), v.end(), add( sep ) ).s;
      }

      struct pattern2vstring {
         vector<string>  v;
         pattern2vstring(): v() {}
         void operator()( path_element const& pe ) {
            v.push_back( pe );
         }
      };

   }
   string to_string( vector<path_element> const& pev ) {
      vector<string> v = for_each( pev.begin(), pev.end(), pattern2vstring() ).v;
      return merge( v, "." );

   }
   string  path::to_string()const {
      vector<string> vs = for_each( _keyentries.begin(), _keyentries.end(), pattern2vstring() ).v;
      return merge( vs, "." );
   }



   path_element const& path::operator[]( size_t i )const {
      if( _keyentries.empty() ) {
         throw Xml_exception(
            eEx::pathlist, msg_list_is_empty );
      }

      return _keyentries[i];
   }

   path_element& path::operator[]( size_t i ) {
      if( _keyentries.empty() ) {
         throw Xml_exception( eEx::pathlist, msg_list_is_empty );
      }

      return _keyentries[i];
   }

   path_element const& path::last()const {
      if( _keyentries.empty() ) {
         throw Xml_exception( eEx::pathlist, msg_list_is_empty );
      }

      return _keyentries[_keyentries.size() - 1];
   }
   path_element& path::last() {
      if( _keyentries.empty() ) {
         throw Xml_exception( eEx::pathlist, msg_list_is_empty );
      }

      return _keyentries[_keyentries.size() - 1];
   }
   xml_node const* path::parentOfLast()const {
      if( _keyentries.empty() ) {
         throw Xml_exception( eEx::visitor, msg_keylist_in_visitor_has_length_zero );
      }

      // parent is document
      if( _keyentries.size() == 1 ) {
         xml_node const*  n = _keyentries.begin()->node();
         return n->parent();
      }

      path_element const& ke =   _keyentries[_keyentries.size() - 2];
      return  ke.node();
   }


   void path::toPatterns( string const& key ) {
      vector<string> vs = rlf_hstring::split( key, "." );
      struct string_to_keyentry {
         vector<path_element> v;
         string_to_keyentry(): v() {}
         void operator()( string const& s ) {
            v.push_back( path_element( s ) );
         }
      };

      _keyentries = for_each( vs.begin(), vs.end(), string_to_keyentry() ).v;

   }


}



//EOF
