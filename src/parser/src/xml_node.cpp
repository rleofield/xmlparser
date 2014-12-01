
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


#include <boost/algorithm/string.hpp>


#include "tLog_Category_default.h"

#include "xml_node.h"
#include "xml_element.h"
#include "xml_document.h"
#include "xml_text.h"
#include "xml_declaration.h"
#include "xml_comment.h"
#include "xml_exception.h"
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

   xml_node* xml_node::identify( rawxml_position& pos ) {

      pos.skip();

      string next_ = pos.next( string( "<" ).size() );

      if( next_ != "<" ) {
         // no '<'
         throw xml_exception( tlog_lfm_,
                              eException::unknown_node,
                              msg_unknown_node + ": " +
                              pos.next25() );
         //         return 0;
      }

      vector8_t::const_iterator ptr_end_tag = pos.find( static_cast<string>( ">" ) ); //">"
      // extract string until on after found position
      string const& content = pos.next( ptr_end_tag + 1 );
      return createNode( content );
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

   xml_node* xml_node::createNode( string const& temp ) {
      if( temp.size() < 2 ) { // only <> in element
         throw xml_exception( tlog_lfm_,
                              eException::unknown_node, msg_unknown_node + ": " + temp );
         //         return 0;
      }

      if( isElement( temp ) ) {  // is alpha or underscore
         LOGT_INFO( "Node: " + temp );
         xml_node* node =  xml_element::create();
         node->parent( this );
         return node;
      }

      if( isComment( temp ) ) {  // <!-- und -->
         xml_node* node = xml_comment::create();
         node->parent( this );
         return node;
      }


      if( isDeclaration( temp ) )  {   // "<?xml und ?>"
         xml_node* node = xml_declaration::create( tlog_lfm_ );
         node->parent( this );
         return node;
      }

      throw xml_exception( tlog_lfm_,
                           eException::unknown_node, msg_unknown_node + ": " + temp );
   }


   xml_node::xml_node( eNodeType type_ ) :
      acc(),
      _lookuppath(),
      _type( type_ ),
      _firstChild(),
      _lastChild(),
      _prev_sibling(),
      _next_sibling(),
      _parent(),
      _node_value(),
      _rawxml() {}




   xml_node::~xml_node() {
      if( !usePointerContainer ) {
         xml_node* node = firstChild();

         while( node != 0 ) {
            xml_node* temp = node;
            node = node->_next_sibling;
            //tAllocatedPtr a = AllocatedPtrList.to_ptr_instanz(temp, tAllocatedPtr::tNode);
            //AllocatedPtrList.Delete(a.Id());

            delete temp; // do_delete
         }
      }
   }

   void xml_node::clear() {
      if( !usePointerContainer ) {
         xml_node* node = firstChild();

         while( node ) {
            xml_node* temp = node;
            node = node->_next_sibling;
            delete temp;
         }

         _firstChild = nullptr;
         _lastChild = nullptr;
      }
      else{
         xml_node* node = firstChild();

         while( node ) {
            xml_node* temp = node;
            node = node->_next_sibling;
            temp = nullptr;
         }

         _firstChild = nullptr;
         _lastChild = nullptr;

      }
   }

   const string xml_node::value() const {
      return _node_value;
   }
   void xml_node::value( const string& _value )   {
      _node_value = _value;
   }

   const xml_node* xml_node::firstChild() const {
      return _firstChild;
   }
   xml_node* xml_node::firstChild() {
      return _firstChild;
   }

   string xml_node::tvalue() const {
      string t = stype();
      return t + ":" + _node_value;
   }
   std::string xml_node::stype()const {
      if( _type == eNodeType::DOCUMENT ) {
         return "doc";
      }

      if( _type == eNodeType::ELEMENT ) {
         return "elem";
      }

      if( _type == eNodeType::COMMENT ) {
         return "com";
      }

      if( _type == eNodeType::TEXT ) {
         return "txt";
      }

      if( _type == eNodeType::DECLARATION ) {
         return "decl";
      }

      return "unknown";

   }
   vector<std::string> xml_node::acc_all ;


   std::string xml_node::accs()const {
      std::string s;

      for( string a : acc ) {
         s += a;
      }

      return s;
   }
   std::string xml_node::accs_all() {
      std::string s;

      for( string & a : acc_all ) {
         s += a;
      }

      return s;
   }


   xml_node* xml_node::linkEndChild( xml_node* node ) {
      xml_node const* p = node->parent();

      assert( node->parent() == nullptr || node->parent() == this );
      xml_document const* doc = node->getDocument();
      xml_document const* d = getDocument();

      assert( node->getDocument() == nullptr || node->getDocument() == getDocument() );

      if( node->type() == xml_node::eNodeType::DOCUMENT ) {
         delete node; // do_delete
         throw xml_exception( tlog_lfm_,
                              eException::document_top_only, msg_document_top_only );
      }

      string lcname;
      string val  = stype();

      if( lastChild() != nullptr ) {
         val = lastChild()->tvalue();
      }

      string valn = node->tvalue();
      acc.push_back( ", a:" + val );
      acc_all.push_back( ", a:" + valn );
      //LOGT_INFO( "acc_all " + accs_all() );
      //LOGT_INFO( "acc     " + accs() );
      //LOGT_INFO( "value   '" + node->tvalue() + "'" );
      //LOGT_INFO( "this    '" + tvalue() + "'" );
      //      if( parent() != nullptr )
      //            LOGT_INFO( "parent  '" + parent()->value() + "'" );

      vector<string> vacc = acc;
      vector<string> vacca = acc_all;

      if( lastChild() != nullptr ) {
         lcname = lastChild()->value();
      }

      node->parent( this );

      if( _lastChild != nullptr ) {
         _lastChild->_next_sibling = node;
      } else {
         _firstChild = node;   // it was an empty list.
      }

      node->_prev_sibling = _lastChild;
      node->_next_sibling = nullptr;

      _lastChild = node;
      return node;
   }


   xml_node* xml_node::insertCommentBefore( xml_node* child,  xml_node* addThis ) {
      if( child == nullptr ) {
         return nullptr;
      }

      xml_node* parent_ = child->parent();

      if( parent_ != this ) {
         return nullptr;
      }

      if( addThis->type() == xml_node::eNodeType::DOCUMENT ) {
         throw xml_exception( tlog_lfm_,
                              eException::document_top_only, msg_document_top_only );
      }

      xml_node* node = addThis; //->clone();

      if( !node ) {
         return nullptr;
      }

      node->parent( this );

      node->_next_sibling = child;
      node->_prev_sibling = child->_prev_sibling;

      if( child->_prev_sibling ) {
         child->_prev_sibling->_next_sibling = node;
      } else {
         assert( _firstChild == child );
         _firstChild = node;
      }

      child->_prev_sibling = node;
      return node;
   }




   const xml_node* xml_node::iterateChildren( const xml_node* previous ) const {
      if( !previous ) {
         return firstChild();
      } else {
         assert( previous->parent() == this );
         return previous->_next_sibling;
      }
   }
   xml_node* xml_node::iterateChildren( xml_node* previous ) {
      if( !previous ) {
         return firstChild();
      } else {
         assert( previous->parent() == this );
         return previous->_next_sibling;
      }
   }

   xml_node* xml_node::lastChild()  {
      return _lastChild;
   }
   const xml_node* xml_node::lastChild()const {
      return _lastChild;
   }


   const xml_element* xml_node::lastChildElement( const string& value_ ) const {
      const xml_node* node;

      for( node = _lastChild; node != nullptr; node = node->_prev_sibling ) {
         if( node->value() == value_ &&
               dynamic_cast<xml_element const*>( node )

           ) {
            return
               dynamic_cast<xml_element const*>( node );
         }
      }

      return 0;
   }
   xml_element* xml_node::lastChildElement( const string& value_ ) {

      xml_node* node;

      for( node = lastChild(); node != nullptr; node = node->_prev_sibling ) {
         if( node->value() == value_ &&
               dynamic_cast<xml_element const*>( node )
           ) {
            return
               dynamic_cast<xml_element*>( node );
         }
      }

      return 0;
   }




   const xml_node* xml_node::next() const {
      return _next_sibling;
   }


   xml_node::eNodeType xml_node::type() const  {
      return _type;
   }

   void         xml_node::parent( xml_node* p ) {
      _parent = p;
   }


   xml_document const* xml_node::getDocument()  {
      for( const xml_node* node = this; node != nullptr; node = node->parent() ) {
         xml_document const* doc =
            dynamic_cast<xml_document const*>( node ) ;

         if( doc != nullptr ) {
            assert( doc->_next_sibling == nullptr );
            assert( doc->_prev_sibling == nullptr );
            assert( doc->_parent == nullptr );
            return doc;
         }
      }

      return nullptr;
   }

   void ph::clear_pointers() {
      if( usePointerContainer ) {
         for( auto & p : pointers ) {
            p.second.delete_ptr();
         }

         pointers.clear();
      }
   }
   void ph::delete_ptr() {
      if( _ptr != nullptr ) {
         delete _ptr;
         _ptr = nullptr;
      }
   }

   void ph::add( xml_node* p ) {
      if( usePointerContainer ) {
         ph p0( p );
         ph::pointers[p0.index()] = p0;
      }
   }

   std::map<xml_node*, ph> ph::pointers;


//   tIndexPtr::tIndexPtr( tIndexPtr const& ip ): _index( INVALID_ID ) {
//      if( this != & ip ) {
//         _index = ip._index;
//      }
//   }
//   tIndexPtr& tIndexPtr::operator=( tIndexPtr const& id ) {
//      if( this != &id ) {
//         _index = id._index;
//      }

//      return *this;
//   }

}

//EOF
