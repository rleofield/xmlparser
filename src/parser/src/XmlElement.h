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

#ifndef RL_XML_ELEMENT_H
#define RL_XML_ELEMENT_H

#include <string>
#include <vector>

#include "rawxml_position.h"
#include "XmlNode.h"
#include "XmlAttribute.h"



namespace txml {


   class XmlElement : public XmlNode {

      explicit XmlElement( const std::string& _value );
      XmlElement( const XmlElement& );
      XmlElement& operator=( const XmlElement& base );
      static void operator delete( void* );
      static void* operator new( size_t, t_alloc_line_file_method const& lfm );

      std::string getAttributes( vector8_t const& temp );

      std::vector<XmlAttribute> _attributes;
      vector8_t rawattributes;


   public:
      static XmlElement* create( t_alloc_line_file_method const& lfmcIn, const std::string& value_ = std::string() );
      static XmlElement* create( const std::string& value_ = std::string() );
      virtual ~XmlElement();

      std::string attribute( const std::string& name ) const;
      std::string attributeByKey( const std::string& key ) const;
      std::vector<XmlAttribute>::const_iterator AttributeObj( const std::string& name ) const;
      std::vector<XmlAttribute> const& Attributes() const;

      void setAttribute( const std::string& name, const std::string& _value );
      void removeAttribute( const std::string& name ) ;

      std::vector<XmlAttribute>::const_iterator firstAttribute() const;
      std::vector<XmlAttribute>::iterator firstAttribute();

      std::vector<XmlAttribute>::const_iterator lastAttribute()const;
      std::vector<XmlAttribute>::iterator lastAttribute();

      const std::string getText() const;
      void setText( std::string const& s );


      virtual XmlNode* clone() const;

      virtual void parse( rawxml_position& pos );
      virtual bool accept( XmlVisitor* visitor ) const;
   protected:
      void copy( XmlElement& target ) const;

   };



} // end of namespace txml



#endif

//EOF

