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

#ifndef RL_XML_LOCATOR_H
#define RL_XML_LOCATOR_H


#include "XmlElement.h"
#include "XmlVisitor.h"


namespace txml {


   /** Locate element value in tree
   */
   class XmlLocator : public XmlVisitor {
   public:
      XmlLocator( std::string const& key );

      virtual bool enter( const XmlElement& );
      virtual bool visitExit( const XmlElement& );
      virtual bool visit( const XmlText& text );

      std::string const& value()  const    {
         return _value;
      }
      std::string const& attr()  const    {
         return _attr;
      }
      void attr( std::string const& a ) {
         _attr = a;
      }
      void setElementAttr( std::string const& value );
      bool isAttr()const ;

      XmlElement* elementfound()const {
         return _elementfound;
      }
      bool accepted() {
         return _elementfound != nullptr;
      }

      keyentries lookupkeys;
      keyentries remainder;

   private:
      XmlElement* _elementfound;
      std::string _value;
      std::string _attr;
      XmlLocator( XmlLocator const& );
      void operator=( XmlLocator const& );

   };

} // end of namespace txml


#endif

//EOF


