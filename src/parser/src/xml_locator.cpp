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
      visitor_base(),
      _path(),
      remainder(),
      _elementfound( nullptr ),
      _value(),
      _attr() {
      _path.from_string(key);
   }
   xml_locator::xml_locator( path const& key ):
      visitor_base(),
      _path(),
      remainder(),
      _elementfound( nullptr ),
      _value(),
      _attr() {
      _path.from_string(key);
   }


   v_ret xml_locator::enter( const xml_element& element ) {
      if( _elementfound != nullptr ) {
         return v_ret::eRet::STOP;
      }

      if( _path.empty() ) {
         return v_ret::eRet::STOP;
      }

      string keys_element = element.lookuppath();
      string strLookupkeys = _path;

      // search depth reached
      if( element.lookuppath().size() == _path.size() ) {
         if( strLookupkeys !=  keys_element ) {
            _value = element.unencoded_text();
            _elementfound = nullptr;
            //return v_ret::eType::STOP;
         }
      }

      if( strLookupkeys ==  keys_element ) {
         _value = element.unencoded_text();
         xml_node const* n = element.lookuppath().last().node();
         _elementfound = const_cast<xml_element*>( dynamic_cast<xml_element const*>( n ) );
         return v_ret::eRet::STOP;
      }

      //string lookupkey = lookupkeys;
      string a = element.attribute_by_path( _path );

      if( !a.empty() ) {
         this->attr( a );
         xml_node const* n = element.lookuppath().last().node();
         _elementfound = const_cast<xml_element*>( dynamic_cast<xml_element const*>( n ) );
         return v_ret::eRet::STOP;
      }


      return v_ret::eRet::RECURSE;
   }


   v_ret xml_locator::exit( const xml_element& ) {
      if( _elementfound != nullptr ) {
         return v_ret::eRet::STOP;
      }

      return v_ret::eRet::RECURSE;
   }


   v_ret xml_locator::visittext( const xml_text& ) {
      if( _elementfound != nullptr ) {
         return v_ret::eRet::STOP;
      }

      return v_ret::eRet::RECURSE;
   }

   void xml_locator::setElementAttr( string const& value_ ) {
      if( _elementfound == nullptr ) {
         return ;
      }

      path_element const& last = _path.last();
      _elementfound->attribute( last.attr(), value_ );
   }

   bool xml_locator::isAttr()const {
      if( _path.empty() ) {
         return false;
      }

      path_element const& last = _path.last();
      return !last.attr().empty();
   }


}

//EOF
