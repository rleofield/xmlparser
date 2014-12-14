
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

   xml_node* xml_node::identify_in_doc( raw_buffer& buffer ) {
      return identify_in_elem( buffer );
   }

   xml_node* xml_node::identify_in_elem( raw_buffer& buffer ) {

      buffer.skip();

      string next_open_brace = buffer.next( string( "<" ).size() );

      if( next_open_brace != "<" ) {
         // no '<'
         throw xml_exception( tlog_lfm_,
                              eException::unknown_node,
                              msg_unknown_node + ": " +
                              buffer.next25() );
         //         return 0;
      }

      // find closing brace
      auto content = buffer.next_until( ">" ); //">"

      // find node type by string between < .... >
      xml_node* n = createNode( content );
      n->parent( this );
      return n;
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
      if( IsAlpha( s[1] ) || s[1] == '_' || s[1] == ' ' ) {
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
         xml_node* node =  xml_element::create();
         return node;
      }

      if( isComment( temp ) ) {  // <!-- und -->
         xml_node* node = xml_comment::create();
         return node;
      }


      if( isDeclaration( temp ) )  {   // "<?xml und ?>"
         xml_node* node = xml_declaration::create( tlog_lfm_ );
         return node;
      }

      throw xml_exception( tlog_lfm_,
                           eException::unknown_node, msg_unknown_node + ": " + temp );
   }


   xml_node::xml_node( eNodeType type_ ) :
      //acc(),
      _lookuppath(),
      _type( type_ ),
      _nh(),
      _parent( nullptr ),
      _node_value(),
      _rawxml() {}


   xml_node::xml_node( eNodeType type_, string const& v ) :
      //acc(),
      _lookuppath(),
      _type( type_ ),
      _nh(),
      _parent( nullptr ),
      _node_value( v ),
      _rawxml() {}


   xml_node::~xml_node() {
      if( !usePointerContainer ) {
         xml_node* node = firstChild();

         while( node != 0 ) {
            xml_node* temp = node;
            node = node->next();
            delete temp; // do delete
         }
      }
   }

   void xml_node::clear() {
      xml_node* node = firstChild();

      while( node != nullptr ) {
         xml_node* temp = node;
         node = node->next();

         if( !usePointerContainer ) {
            delete temp;
         }

         temp = nullptr;
      }

      _nh.firstChild = nullptr;
      _nh.lastChild = nullptr;
   }

   const string xml_node::value() const {
      return _node_value;
   }
   void xml_node::value( const string& _value )   {
      _node_value = _value;
   }

   const xml_node* xml_node::firstChild() const {
      return _nh.firstChild;
   }
   xml_node* xml_node::firstChild() {
      return _nh.firstChild;
   }

   string xml_node::tvalue() const {
      string t =  to_string( _type );
      return t + ":" + _node_value;
   }

   std::string to_string( xml_node::eNodeType  _type ) {
      if( _type == xml_node::eNodeType::DOCUMENT ) {
         return "doc";
      }

      if( _type == xml_node::eNodeType::ELEMENT ) {
         return "elem";
      }

      if( _type == xml_node::eNodeType::COMMENT ) {
         return "com";
      }

      if( _type == xml_node::eNodeType::TEXT ) {
         return "txt";
      }

      if( _type == xml_node::eNodeType::DECLARATION ) {
         return "decl";
      }

      assert( false );

   }

   //vector<std::string> xml_node::acc_all ;


   //   std::string xml_node::accs()const {
   //      std::string s;

   //      for( string a : acc ) {
   //         s += a;
   //      }

   //      return s;
   //   }
   //   std::string xml_node::accs_all() {
   //      std::string s;

   //      for( string & a : acc_all ) {
   //         s += a;
   //      }

   //      return s;
   //   }


   xml_node* xml_node::link_end_child( xml_node* node ) {

      // document can't be linked
      if( node->type() == xml_node::eNodeType::DOCUMENT ) {
         delete node; // do_delete
         throw xml_exception( tlog_lfm_,
                              eException::document_top_only, msg_document_top_only );
      }

      // must have same parent or nothing
      xml_node const* node_parent = node->parent();
      assert( node->parent() == nullptr || node_parent == this );

      // must have same document or nothing
      xml_document const* node_doc = node->getDocument();
      xml_document const* this_doc = getDocument();
      assert( node_doc == nullptr || node_doc == this_doc );


//      string val  = to_string( _type );

//      if( last_child() != nullptr ) {
//         val = last_child()->tvalue();
//      }

      //string valn = node->tvalue();
      //acc.push_back( ", a:" + val );
      //acc_all.push_back( ", a:" + valn );
      //LOGT_INFO( "acc_all " + accs_all() );
      //LOGT_INFO( "acc     " + accs() );
      //LOGT_INFO( "value   '" + node->tvalue() + "'" );
      //LOGT_INFO( "this    '" + tvalue() + "'" );
      //      if( parent() != nullptr )
      //            LOGT_INFO( "parent  '" + parent()->value() + "'" );

      //vector<string> vacc = acc;
      //vector<string> vacca = acc_all;

//      if( last_child() != nullptr ) {
//         lcname = last_child()->value();
//      }

      node->parent( this );

      if( _nh.lastChild != nullptr ) {
         _nh.lastChild->_nh.next_sibling = node;
      } else {
         _nh.firstChild = node;   // it was an empty list.
      }

      node->_nh.prev_sibling = last_child();
      node->_nh.next_sibling = nullptr;

      _nh.lastChild = node;
      return node;
   }

   void xml_node::insert_comment_before( xml_node* child,  xml_comment* comment ) {
      if( !comment ) {
         return ;
      }
      if( child == nullptr ) {
         return;
      }

      xml_node const* parent_ = child->parent();

      if( parent_ != this ) {
         return ;
      }




      comment->parent( this );

      comment->_nh.next_sibling = child;
      comment->_nh.prev_sibling = child->prev();

      if( child->_nh.prev_sibling ) {
         child->_nh.prev_sibling->_nh.next_sibling = comment;
      } else {
         assert( _nh.firstChild == child );
         _nh.firstChild = comment;
      }

      child->_nh.prev_sibling = comment;
   }


   xml_node* xml_node::last_child()  {
      return _nh.lastChild;
   }
   const xml_node* xml_node::last_child()const {
      return _nh.lastChild;
   }


   const xml_element* xml_node::last_child_element( const string& value_ ) const {
      const xml_node* node;

      for( node = last_child(); node != nullptr; node = node->prev() ) {
         if( node->value() == value_ &&
               dynamic_cast<xml_element const*>( node )

           ) {
            xml_node const* pn = node;
            return dynamic_cast<xml_element const*>( node );
         }
      }

      return nullptr;
   }
   xml_element* xml_node::last_child_element( const string& val ) {

      xml_node* node;

      for( node = last_child(); node != nullptr; node = node->prev() ) {
         if( node->value() == val && dynamic_cast<xml_element const*>( node )
           ) {
            xml_node* pn = node;
            return dynamic_cast<xml_element*>( node );
         }
      }

      return nullptr;
   }


   const xml_node* xml_node::prev() const {
      return _nh.prev_sibling;
   }
   xml_node* xml_node::prev()  {
      return _nh.prev_sibling;
   }
   const xml_node* xml_node::next() const {
      return _nh.next_sibling;
   }
   xml_node* xml_node::next() {
      return _nh.next_sibling;
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
            assert( doc->next() == nullptr );
            assert( doc->prev() == nullptr );
            assert( doc->parent() == nullptr );
            return doc;
         }
      }

      return nullptr;
   }

   void tPointers::clear() {

      for( auto & p : pointers ) {
         p.second.delete_ptr();
      }

      pointers.clear();

   }
   void tPointers::delete_ptr() {
      if( _ptr != nullptr ) {
         delete _ptr;
         _ptr = nullptr;
      }
   }

   void tPointers::add( xml_node* p ) {
      if( usePointerContainer ) {
         tPointers p0( p );
         tPointers::pointers[p0.index()] = p0;
      }
   }

   std::map<xml_node*, tPointers> tPointers::pointers;



}

//EOF
