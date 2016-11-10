
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
      //std::string b1 = buffer.next25();
      //LOGT_INFO( b1 );
      return create_in_elem( buffer );
   }

   xml_node* xml_node::create_in_elem( raw_buffer& buffer ) {

      std::string b = buffer.next25();
      //LOGT_INFO( b );

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
      // didn't set the string as value
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

      xml_document* d = document();
      assert( d != nullptr );

      if( isElement( temp ) ) {  // is alpha or underscore
         xml_node* node =  d->element_create();
         return node;
      }

      if( isComment( temp ) ) {  // <!-- und -->
         xml_node* node = d->comment_create();
         return node;
      }

      if( isDeclaration( temp ) )  {   // "<?xml und ?>"
         xml_node* node = d->declaration_create( tlfm_ );
         return node;
      }

      throw Xml_exception(
         eEx::parse, msg_unknown_node + ": " + temp );
   }


   xml_node::xml_node( eType type_ ) :
      _path(),
      _type( type_ ),
      _childs(),
      _parent( nullptr ),
      _value(),
      _rawxml() {}


   xml_node::xml_node( eType type_, string const& v ) :

      _path(),
      _type( type_ ),
      _childs(),
      _parent( nullptr ),
      _value( v ),
      _rawxml() {}


   xml_node::~xml_node() {
   }

   void xml_node::clear() {

      std::vector<xml_node*>  const& childs = getChilds();

      for( xml_node * node : childs ) {
         node->clear();
      }

      _childs.clear();
   }

   const string xml_node::value() const {
      return _value;
   }
   void xml_node::value( const string& value_ )   {
      _value = value_;
   }

   xml_node* xml_node::first_child() const {
      xml_node* n = _childs.first();
      return n;
   }


   //   string xml_node::tvalue() const {
   //      string t =  to_string( _type );
   //      return t + ":" + _value;
   //   }




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
      _childs.append( node );


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


   }


   xml_node* xml_node::last_child()const  {
      xml_node* n = _childs.last();
      return n;
   }


   xml_element* xml_node::first_child_element( const string& val ) const {
      xml_element* elem = _childs.first_element( val );
      return elem;
   }

   xml_element* xml_node::last_child_element( const string& val ) const {
      xml_element* elem = _childs.last_element( val );
      return elem;
   }


   xml_node* xml_node::prev() const {
      xml_node const* p = parent();

      if( p == nullptr ) {
         return nullptr;
      }

      tchilds const& ch = p->_childs;

      xml_node* n = ch.prev( this );
      return n;
   }

   xml_node* xml_node::next() const {

      xml_node const* p = parent();

      if( p == nullptr ) {
         return nullptr;
      }

      tchilds const& ch = p->_childs;

      // text has no next()
      if( _type  == eType::TEXT ) {
         return nullptr;
      }


      xml_node* n = ch.next( this );

      if( n == nullptr ) {
         //LOGT_INFO( "null" );
         return nullptr;
      }

      string s = n->value();
      // LOGT_INFO( s );

      return n;

   }


   xml_node::eType xml_node::type() const  {
      return _type;
   }

   void         xml_node::parent( xml_node* parent_ ) {
      _parent = parent_;
   }


   xml_document* xml_node::document()  {
      for( xml_node* node = this; node != nullptr; node = node->parent() ) {
         xml_document* doc =
            dynamic_cast<xml_document*>( node ) ;

         if( doc != nullptr ) {
            assert( doc->next() == nullptr );
            assert( doc->prev() == nullptr );
            assert( doc->parent() == nullptr );
            return doc;
         }
      }

      assert( true );
      return nullptr;
   }
   int i15 = 0;
   tNodes::~tNodes() {
      for( auto & p : nodes ) {
         delete p.second;
      }

      nodes.clear();

      i15 = 1;
   }

   void tNodes::clear_all() {

      for( auto & p : nodes ) {
         delete p.second;
      }

      nodes.clear();

   }


   void tNodes::add( xml_node* p ) {
      std::map<xml_node*, xml_node*>::const_iterator it = nodes.find( p );
      assert( it == nodes.end() );
      nodes[p] = p;
   }



   xml_element* tchilds::first_element( std::string const& val )const {
      if( val == "" ) {
         return nullptr;
      }

      if( _childs.size() == 0 ) {
         return nullptr;
      }

      for( xml_node * n : _childs ) {
         xml_element* elem = dynamic_cast<xml_element*>( n );

         if( elem != nullptr ) {
            if( elem->value() == val ) {
               return elem;
            }
         }
      }

      return nullptr;
   }

   xml_element* tchilds::first_element()const {
      if( _childs.size() == 0 ) {
         return nullptr;
      }

      for( xml_node * n : _childs ) {
         xml_element* elem = dynamic_cast<xml_element*>( n );

         if( elem != nullptr ) {
            return elem;
         }
      }

      return nullptr;
   }

   // http://goodliffe.blogspot.de/2013/05/c11-iterating-range-based-for-backwards.html
   template <typename T>
   class iterate_backwards {
   public:
      explicit iterate_backwards( const T& t_ ) : t( t_ ) {}
      typename T::const_reverse_iterator begin() const {
         return t.rbegin();
      }
      typename T::const_reverse_iterator end()   const {
         return t.rend();
      }
   private:
      const T& t;
   };
   template <typename T>
   iterate_backwards<T> backwards( const T& t ) {
      return iterate_backwards<T>( t );
   }

   xml_element* tchilds::last_element( std::string const& val )const {

      if( _childs.size() == 0 ) {
         return nullptr;
      }

      vector<xml_node*>::const_reverse_iterator rbegin = _childs.rbegin();
      vector<xml_node*>::const_reverse_iterator rend = _childs.rend();

      for( xml_node * n : backwards( _childs ) ) {
         xml_element* elem = dynamic_cast<xml_element*>( n );

         if( elem != nullptr ) {
            if( elem->value() == val ) {
               return elem;
            }
         }
      }

      return nullptr;
   }

   xml_element* tchilds::last_element()const {

      if( _childs.size() == 0 ) {
         return nullptr;
      }

      for( xml_node * n : backwards( _childs ) ) {
         xml_element* elem = dynamic_cast<xml_element*>( n );

         if( elem != nullptr ) {
            return elem;
         }
      }

      return nullptr;
   }


   std::vector<std::string> tchilds::values()const {
      std::vector<std::string> v;

      for( auto n : _childs ) {
         if( n != nullptr ) {
            std::string s = n->value();
            v.push_back( s );
         } else {
            std::string s2 = "null";
            v.push_back( s2 );
         }
      }

      return v;
   }


   xml_node* tchilds::next( xml_node const* p ) const {
      if( p == nullptr ) {
         return nullptr;
      }

      size_t i = pos( p );

      if( i == err ) {
         return nullptr;
      }

      i++;

      if( i == _childs.size() ) {
         return nullptr;
      }

      xml_node* n = _childs[i];
      return n;
   }

   size_t tchilds::pos( xml_node const* p ) const {
      if( p == nullptr ) {
         return err;
      }

      size_t index = 0;

      for( xml_node * n : _childs ) {
         if( n == p ) {
            return index;
         }

         index++;
      }

      return err;
   }


}

//EOF
