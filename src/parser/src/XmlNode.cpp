
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


#include <boost/algorithm/string.hpp>


#include "XmlNode.h"
#include "XmlElement.h"
#include "XmlDocument.h"
#include "XmlText.h"
#include "XmlDeclaration.h"
#include "XmlComment.h"
#include "XmlException.h"
#include "rawxml_position.h"

using namespace std;

namespace txml {


   /*

         A Document can contain: Element  (container or leaf)
                           Comment (leaf)
                           Declaration( leaf )

         An Element can contain: Element (container or leaf)
                           Text  (leaf)
                           Attributes (list)
                           Comment (leaf)

         A Decleration contains: Attributes

         ----
         // - Elements start with a letter or underscore, but xml is reserved.
         // - Comments: <!--
         // - Declaration: <?xml
        */

   XmlNode* XmlNode::identify( rawxml_position& pos ) {

      pos.skip();

      string next_ = pos.next( string( "<" ).size() );

      if( next_ != "<" ) {
         // no '<'
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_unknown_node,
                             msg_unknown_node + ": " +
                             pos.next25() );
         //         return 0;
      }

      vector8_t::const_iterator ptr_end_tag = pos.find( static_cast<string>( ">" ) ); //">"
      string const& content = pos.next( ptr_end_tag + 1 );
      return identifyNode( content );
   }


   namespace {
      int IsAlpha( uint8_t ch ) {
         if( ch < 127 ) {
            return ::isalpha( ch );
         } else {
            return 1;   // fallback to EN code, unicode is too large for checking
         }
      }

   }

   bool isElement( string const& s ) { // size is > 2
      if( IsAlpha( s[1] ) || s[1] == '_' ) {
         return true;
      }

      return false;
   }
   bool isComment( string const& s ) { // size is > 2
      // <!-- und -->
      if( boost::starts_with( s, comment_start )
            && boost::ends_with( s, comment_end ) ) {
         return true;
      }

      return false;
   }
   bool isDeclaration( string const& s ) { // size is > 2
      // "<?xml und ?>"
      if( boost::starts_with( s, declaration_start )
            && boost::ends_with( s,   declaration_end ) ) {
         return true;
      }

      return false;
   }

   XmlNode* XmlNode::identifyNode( string const& temp ) {
      if( temp.size() < 2 ) { // only <> in element
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_unknown_node, msg_unknown_node + ": " + temp );
         //         return 0;
      }

      if( isElement( temp ) ) {  // is alpha or underscore
         XmlNode* node =  XmlElement::create();
         node->parent( this );
         return node;
      }

      if( isComment( temp ) ) {  // <!-- und -->
         XmlNode* node = XmlComment::create();
         node->parent( this );
         return node;
      }

      if( isDeclaration( temp ) )  {   // "<?xml und ?>"
         XmlNode* node = XmlDeclaration::create();
         node->parent( this );
         return node;
      }

      throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                          enum_unknown_node, msg_unknown_node + ": " + temp );
   }


   XmlNode::XmlNode( NodeType type_ ) :
      _lookuppath(),
      _type( type_ ),
      _firstChild(),
      _lastChild(),
      _previous(),
      _next(),
      _parent(),
      _node_value(),
      _rawxml() {}


   XmlNode::~XmlNode() {
      XmlNode* node = firstChild();

      while( node != 0 ) {
         XmlNode* temp = node;
         node = node->_next;
         //tAllocatedPtr a = AllocatedPtrList.to_ptr_instanz(temp, tAllocatedPtr::tNode);
         //AllocatedPtrList.Delete(a.Id());

         delete temp; // do_delete
      }
   }

   void XmlNode::clear() {
      XmlNode* node = firstChild();

      while( node ) {
         XmlNode* temp = node;
         node = node->_next;
         delete temp;
      }

      _firstChild = nullptr;
      _lastChild = nullptr;
   }

   const std::string XmlNode::value() const {
      return _node_value;
   }
   void XmlNode::value( const std::string& _value )   {
      _node_value = _value;
   }

   const XmlNode* XmlNode::firstChild() const {
      return _firstChild;
   }
   XmlNode* XmlNode::firstChild() {
      return _firstChild;
   }



   XmlNode* XmlNode::linkEndChild( XmlNode* node ) {
      assert( node->parent() == 0 || node->parent() == this );
      assert( node->getDocument() == nullptr || node->getDocument() == this->getDocument() );

      if( node->type() == XmlNode::RL_XML_DOCUMENT ) {
         delete node; // do_delete
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_document_top_only, msg_document_top_only );
      }

      node->parent( this );

      if( _lastChild ) {
         _lastChild->_next = node;
      } else {
         _firstChild = node;   // it was an empty list.
      }

      node->_previous = _lastChild;
      node->_next = nullptr;

      _lastChild = node;
      return node;
   }


   XmlNode* XmlNode::insertBeforeChild( XmlNode* child, const XmlNode& addThis ) {
      if( child == nullptr ) {
         return nullptr;
      }

      XmlNode* parent_ = child->parent();

      if( parent_ != this ) {
         return nullptr;
      }

      if( addThis.type() == XmlNode::RL_XML_DOCUMENT ) {
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_document_top_only, msg_document_top_only );
      }

      XmlNode* node = addThis.clone();

      if( !node ) {
         return nullptr;
      }

      node->parent( this );

      node->_next = child;
      node->_previous = child->_previous;

      if( child->_previous ) {
         child->_previous->_next = node;
      } else {
         assert( _firstChild == child );
         _firstChild = node;
      }

      child->_previous = node;
      return node;
   }




   const XmlNode* XmlNode::iterateChildren( const XmlNode* previous ) const {
      if( !previous ) {
         return firstChild();
      } else {
         assert( previous->parent() == this );
         return previous->_next;
      }
   }
   XmlNode* XmlNode::iterateChildren( XmlNode* previous ) {
      if( !previous ) {
         return firstChild();
      } else {
         assert( previous->parent() == this );
         return previous->_next;
      }
   }

   XmlNode* XmlNode::lastChild()  {
      return _lastChild;
   }
   const XmlNode* XmlNode::lastChild()const {
      return _lastChild;
   }


   const XmlElement* XmlNode::lastChildElement( const string& value_ ) const {
      const XmlNode* node;

      for( node = _lastChild; node != nullptr; node = node->_previous ) {
         if( node->value() == value_ && dynamic_cast<XmlElement const*>( node ) ) {
            return dynamic_cast<XmlElement const*>( node );
         }
      }

      return 0;
   }
   XmlElement* XmlNode::lastChildElement( const string& value_ ) {

      XmlNode* node;

      for( node = lastChild(); node != nullptr; node = node->_previous ) {
         if( node->value() == value_ && dynamic_cast<XmlElement const*>( node ) ) {
            return dynamic_cast<XmlElement*>( node );
         }
      }

      return 0;
   }




   const XmlNode* XmlNode::next() const {
      return _next;
   }


   XmlNode::NodeType XmlNode::type() const  {
      return _type;
   }

   void         XmlNode::parent( XmlNode* p ) {
      _parent = p;
   }


   XmlDocument const* XmlNode::getDocument()  {
      for( XmlNode const* node = this; node != nullptr; node = node->parent() ) {
         XmlDocument const* doc = dynamic_cast<XmlDocument const*>( node ) ;

         if( doc != nullptr ) {
            return doc;
         }
      }

      return nullptr;
   }


}

//EOF
