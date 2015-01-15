
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

   xml_node* xml_node::create_in_doc( raw_buffer& buffer ) {
      return create_in_elem( buffer );
   }

   xml_node* xml_node::create_in_elem( raw_buffer& buffer ) {

      buffer.skip();

      string next_open_brace = buffer.next( string( "<" ).size() );

      if( next_open_brace != "<" ) {
         // no '<'
         throw Xml_exception(
            eEx::parse,
            msg_unknown_node + ": " +
            buffer.next25() );
      }

      // find closing brace
      auto content = buffer.next_until( ">" ); //">"

      // find node type by string between < .... >
      xml_node* n = create( content );
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



      bool isElement( string const& s ) noexcept { // size is > 2
         if( IsAlpha( s[1] ) || s[1] == '_' || s[1] == ' ' ) {
            return true;
         }

         return false;
      }
      bool isComment( string const& s ) noexcept { // size is > 2
         // <!-- und -->
         if( boost::starts_with( s, "<!--" )
         && boost::ends_with( s, "-->" ) ) {
            return true;
         }

         return false;
      }
      bool isDeclaration( string const& s ) { // size is > 2
         // "<?xml und ?>"
         if( boost::starts_with( s, "<?xml" )
               && boost::ends_with( s,   "?>" ) ) {
            return true;
         }

         return false;
      }
   } // end of anon ns

   xml_node* xml_node::create( string const& temp ) {
      if( temp.size() < 2 ) { // only <> in element
         throw Xml_exception(
            eEx::parse, msg_unknown_node + ": " + temp );
         //         return 0;
      }



      if( isElement( temp ) ) {  // is alpha or underscore
         xml_element* node =  xml_element::create();
         return node;
      }

      if( isComment( temp ) ) {  // <!-- und -->
         xml_comment* node = xml_comment::create();
         return node;
      }

      if( isDeclaration( temp ) )  {   // "<?xml und ?>"
         xml_declaration* node = xml_declaration::create( tlfm_ );
         return node;
      }

      throw Xml_exception(
         eEx::parse, msg_unknown_node + ": " + temp );
   }


   xml_node::xml_node( eType type_ ) :
      _path(),
      _type( type_ ),
      p(nullptr),
      fc(nullptr),
      lc(nullptr),
      ps(nullptr),
      ns(nullptr),
      _value(),
      _rawxml() {}


   xml_node::xml_node( eType type_, string const& v ) :

      _path(),
      _type( type_ ),
      p(nullptr),
      fc(nullptr),
      lc(nullptr),
      ps(nullptr),
      ns(nullptr),
      _value( v ),
      _rawxml() {}


   xml_node::~xml_node() {
      if( !usePointerContainer ) {
         xml_node* node = first_child();

         while( node != nullptr ) {
            xml_node* temp = node;
            node = node->next();
            delete temp; // do delete
         }
      }
   }

   void xml_node::clear() {
      xml_node* node = first_child();

      while( node != nullptr ) {
         xml_node* temp = node;
         node = node->next();

         if( !usePointerContainer ) {
            delete temp;
         }

         temp = nullptr;
      }

      fc = nullptr;
      lc = nullptr;
   }

   const string xml_node::value() const {
      return _value;
   }
   void xml_node::value( const string& value_ )   {
      _value = value_;
   }

   const xml_node* xml_node::first_child() const {
      return fc;
   }
   xml_node* xml_node::first_child() {
      return fc;
   }

   string xml_node::tvalue() const {
      string t =  to_string( _type );
      return t + ":" + _value;
   }




   xml_node* xml_node::link_end_child( xml_node* node ) {

      // document can't be linked
      if( node->type() == xml_node::eType::DOC ) {
         delete node; // do_delete
         throw Xml_exception( eEx::link, msg_document );
      }

      // must have same parent or nothing
      xml_node const* node_parent = node->parent();
      assert( node->parent() == nullptr || node_parent == this );

      // must have same document or nothing
      xml_document const* node_doc = node->document();
      xml_document const* this_doc = document();
      assert( node_doc == nullptr || node_doc == this_doc );


      node->parent( this );

      if( lc != nullptr ) {
         lc->ns = node;
      } else {
         fc = node;   // it was an empty list.
      }

      node->ps = last_child();
      node->ns = nullptr;

      lc = node;
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

      comment->ns = child;
      comment->ps = child->prev();

      if( child->ps ) {
         child->ps->ns = comment;
      } else {
         assert( fc == child );
         fc = comment;
      }

      child->ps = comment;
   }


   xml_node* xml_node::last_child()  {
      return lc;
   }
   const xml_node* xml_node::last_child()const {
      return lc;
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
      return ps;
   }
   xml_node* xml_node::prev()  {
      return ps;
   }
   const xml_node* xml_node::next() const {
      return ns;
   }
   xml_node* xml_node::next() {
      return ns;
   }


   xml_node::eType xml_node::type() const  {
      return _type;
   }

   void         xml_node::parent( xml_node* p1 ) {
      p = p1;
   }


   xml_document const* xml_node::document()  {
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
