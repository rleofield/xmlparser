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

#ifndef RL_XML_ATTRIBUTE_H
#define RL_XML_ATTRIBUTE_H


#include "keyentries.h"


namespace txml {

   class rawxml_position;

   class XmlAttribute {

   public:
      XmlAttribute() :
         keys(), _name(), _value()   { }

      XmlAttribute( const std::string& name_, const std::string& value_ ) :
         keys(), _name( name_ ), _value( value_ ) {}

      std::string const& name()  const {
         return _name;
      }
      std::string const& value() const {
         return _value;
      }
      void name( const std::string& n )  {
         _name = n;
      }
      void value( const std::string& v )   {
         _value = v;
      }

      bool operator==( const XmlAttribute& a ) const {
         return a._name == _name;
      }
      bool operator<( const XmlAttribute& a )   const {
         return _name < a._name;
      }
      bool operator>( const XmlAttribute& a )  const {
         return _name > a._name;
      }

      void parseAttr( rawxml_position& pos );

      void print( std::string& str ) const;

      XmlAttribute( const XmlAttribute& attr ):
         keys( attr.keys ), _name( attr._name ), _value( attr._value ) {}

      XmlAttribute& operator=( const XmlAttribute& attr ) {
         if( this != &attr ) {
            _name = attr._name;
            _value = attr._value;
            keys = attr.keys;
         }

         return *this;
      }
      // compare only the name, for find()
      bool operator==( const XmlAttribute& attr ) {
         return _name == attr._name;
      }

      keyentries keys;
   private:

      std::string _name;
      std::string _value;
   };



} // end of namespace txml



#endif

//EOF


