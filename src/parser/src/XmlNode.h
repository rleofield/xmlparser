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

#ifndef RL_XML_NODE_H
#define RL_XML_NODE_H


#include <string>



#include "keyentries.h"



namespace alloccheck  {
   class t_alloc_line_file_method;
}
using alloccheck::t_alloc_line_file_method;

namespace txml {
   class XmlVisitor;
   class XmlElement;
   class XmlDocument;
   class XmlComment;
   class XmlText;
   class XmlDeclaration;
}

namespace txml {



   class rawxml_position;

   class XmlNode {

      friend class XmlElement;
      friend class XmlDocument;

   public:

      virtual ~XmlNode();

      enum NodeType {
         RL_XML_NONE,
         RL_XML_DOCUMENT,
         RL_XML_ELEMENT,
         RL_XML_COMMENT,
         RL_XML_TEXT,
         RL_XML_DECLARATION,
         RL_XML_TYPECOUNT
      };


      /**
      in Document:   filename of the xml file
      in Element:    name of the element
      in Comment:    the comment text
      in Text:       the text string
      */
      const std::string value() const;
      void value( const std::string& _value );


      void clear();

      const XmlNode* firstChild()const;
      XmlNode* firstChild();

      //      const XmlNode* FirstChild( std::string const& value ) const;
      //      XmlNode* FirstChild( const std::string& _value );

      const XmlNode* lastChild()const;
      XmlNode* lastChild();

      //      const XmlNode* LastChild( const std::string& _value ) const;
      //      XmlNode* LastChild( const std::string& _value ) ;

      const XmlElement* lastChildElement( const std::string& _value ) const;
      XmlElement* lastChildElement( const std::string& _value ) ;

      const XmlNode* iterateChildren( const XmlNode* previous ) const;
      XmlNode* iterateChildren( XmlNode* previous ) ;

      //      const XmlNode* IterateChildren( const std::string& _value, const XmlNode* previous ) const  ;
      //      XmlNode* IterateChildren( const std::string& _value,  XmlNode* previous ) ;

      //std::vector<XmlNode*> childs();
      XmlNode* linkEndChild( XmlNode* addThis );

      XmlNode* insertBeforeChild( XmlNode* beforeThis, const XmlNode& addThis );

   public:

      const XmlNode* next() const;


      const XmlElement* firstChildElement( const std::string& _value ) const   ;
      XmlElement* firstChildElement( const std::string& _value )            ;

      NodeType type() const;


      XmlDocument const* getDocument() ;

      bool noChildren() const {
         return _firstChild == nullptr;
      }

      // return null, if not of the requested type.
      //const XmlDocument*    toXmlDocument()    const ;
      //      const XmlElement*     toXmlElement()     const ;
      //      XmlElement*           toXmlElement()     ;

      virtual XmlNode* clone() const = 0;

      virtual bool accept( XmlVisitor* visitor ) const = 0;


      XmlNode const*  parent( )const {
         return _parent;
      }
      XmlNode*   parent( ) {
         return _parent;
      }
      std::string lookupPathString()const {
         return _lookuppath.to_string();
      }
      keyentries const& lookuppath()const {
         return _lookuppath;
      }
      keyentries& lookuppath() {
         return _lookuppath;
      }

   protected:
      XmlNode( NodeType _type );

      XmlNode* identify( rawxml_position& pos );
      XmlNode* identifyNode( std::string const& str );
      void         parent( XmlNode* p ) ;

      virtual void parse( rawxml_position& ) = 0;

   private:

      // path to node
      keyentries _lookuppath ;

      NodeType     _type;

      XmlNode*     _firstChild;
      XmlNode*     _lastChild;

      XmlNode*      _previous;
      XmlNode*      _next;

      XmlNode*     _parent;

      std::string _node_value;
      std::string _rawxml;
      XmlNode( const XmlNode& );
      void operator=( const XmlNode& base );
   };


} // end of namespace txml



#endif

//EOF


