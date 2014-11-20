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


#include <string>
#include <vector>

#include "xml_utl.h"
#include "xml_locator.h"

#include "stringhelper.h"

using namespace std;

namespace txml {


   xml_locator::xml_locator( string const& key ):
      xml_visitor(),
      lookupkeys( key ),
      remainder(),
      _elementfound( nullptr ),
      _value(),
      _attr() {
   }


   bool xml_locator::enter( const xml_element& element ) {
      if( accepted() ) {
         return false;
      }

      if( lookupkeys.empty() ) {
         return false;
      }

      string keys_element = element.lookupPathString();
      string strLookupkeys = lookupkeys.to_string();

      // search depth reached
      if( element.lookuppath().size() == lookupkeys.size() ) {
         if( strLookupkeys !=  keys_element ) {
            _value = element.getText();
            _elementfound = nullptr;
            //return false;
         }
      }

      if( strLookupkeys ==  keys_element ) {
         _value = element.getText();
         xml_node const* n = element.lookuppath().last().node();
         _elementfound = const_cast<xml_element*>( dynamic_cast<xml_element const*>( n ) );
         return false;
      }

      string lookupkey = lookupkeys.to_string();
      string a = element.attributeByKey( lookupkey );

      if( !a.empty() ) {
         this->attr( a );
         xml_node const* n = element.lookuppath().last().node();
         _elementfound = const_cast<xml_element*>( dynamic_cast<xml_element const*>( n ) );
         return false;
      }


      return true;
   }


   bool xml_locator::visitExit( const xml_element& ) {
      if( accepted() ) {
         return false;
      }

      return true;
   }


   bool xml_locator::visit( const xml_text& ) {
      if( accepted() ) {
         return false;
      }

      return true;
   }

   void xml_locator::setElementAttr( string const& value_ ) {
      if( _elementfound == nullptr ) {
         return ;
      }

      keyentry const& last = lookupkeys.last();
      _elementfound->setAttribute( last.Attr(), value_ );
   }

   bool xml_locator::isAttr()const {
      if( lookupkeys.empty() ) {
         return false;
      }

      keyentry const& last = lookupkeys.last();
      return !last.Attr().empty();
   }


}

//EOF
