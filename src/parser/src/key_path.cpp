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
#include "path.h"
#include "xml_node.h"
#include "xml_fs.h"
#include "xml_exception.h"
#include "xml_interface.h"
#include "xml_element.h"
#include "xml_text.h"
#include "element_locator.h"


#include "stringhelper.h"

using namespace std;

namespace txml {



   path::path( string const& key ): _keyentries() {
      vector<string> elements = rlf_hstring::split( key, path_element::element_separator );

      for( auto const & entry : elements ) {
         _keyentries.push_back( entry );
      }
   }
   void path::from_string( string const& key ) {
      _keyentries.clear();
      vector<string> elements = rlf_hstring::split( key, path_element::element_separator );

      for( auto const & entry : elements ) {
         _keyentries.push_back( entry );
      }
   }


   path& path::operator=( string const& key ) {
      from_string( key );
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

   bool path::points_to_attr()const {
      path_element const& el = last();
      return el.points_to_attr();
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


   std::string path::toElementText( xml_document const& _doc )const {

      path key_no_attr = *this;

      if( key_no_attr.points_to_attr() ) {
         path_element& pel = key_no_attr.last();
         pel.attr( path_attr() );
      }// debug

      string pathadd = key_no_attr;

      if( pathadd == "domain.features.apic" ) {
         pathadd = key_no_attr;
      }

      element_locator locator( key_no_attr );

      _doc.accept( &locator );

      xml_element* elem = locator.elementfound();

      if( elem != nullptr ) {
         xml_node const* ch = elem->first_child();

         if( ch == nullptr ) {
            return "nullptr";
         }

         xml_text const* p1 = dynamic_cast<xml_text const*>( ch );
         string txt = p1->value();
         return txt;

      }


      return "";

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

      string merge( vector<string>  const& v, string const& sep ) {
         if( v.empty() ) {
            return string();
         }

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

         if( v.size() == 1 ) {
            return v[0];
         }

         return for_each( v.begin(), v.end(), add( sep ) ).s;
      }


   }
   //   string to_string( vector<path_element> const& pev ) {
   //      vector<string> v = for_each( pev.begin(), pev.end(), pattern2vstring() ).v;
   //      return merge( v, "." );

   //   }
   string  path::to_string()const {
      struct pattern2vstring {
         vector<string>  v;
         pattern2vstring(): v() {}
         void operator()( path_element const& pe ) {
            v.push_back( pe );
         }
      };
      vector<string> vs = for_each( _keyentries.begin(), _keyentries.end(), pattern2vstring() ).v;
      return merge( vs, path_element::element_separator );
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




}



//EOF
