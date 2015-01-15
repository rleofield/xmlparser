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

#ifndef RLF_XML_ELEMENT_H
#define RLF_XML_ELEMENT_H

#include <string>
#include <vector>

#include "rawxml_position.h"
#include "xml_node.h"
#include "xml_attribute.h"
#include "xml_document.h"


namespace txml {


   class xml_element : public xml_node {

      explicit xml_element( const std::string& _value );
      xml_element( const xml_element& );
      xml_element& operator=( const xml_element& base );
      static void operator delete( void* );
      static void* operator new( size_t, t_lfm const& lfm );

      std::string parse_attributes( std::string const& temp );
      std::vector<xml_attribute> _attributes;

   public:
      static xml_element* create( t_lfm const& lfmcIn, const std::string& value_ = std::string() );
      static xml_element* create( const std::string& value_ = std::string() );
      virtual ~xml_element();

      // add empty attribute
      std::string attribute( const std::string& name ) const;
      std::string attribute_by_path( const path& key ) const;
      std::vector<xml_attribute> const& Attributes() const;

      // change or set an attribute
      void attribute( const std::string& name, const std::string& _value );

      const std::string unencoded_text() const;
      void text( std::string const& s );
      xml_text const* text() const ;

      bool isClosed()const ;
      virtual void parse( raw_buffer& pos );
      virtual v_ret accept( visitor_base* visitor ) const;

   };



} // end of namespace txml



#endif

//EOF


