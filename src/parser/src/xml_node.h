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

#ifndef RLF_NODE_H
#define RLF_NODE_H


//#include <map>
#include <list>
#include <string>

#include "enum_macro.h"


#include "xml_exception.h"

#include "path.h"
#include "visitor_ret.h"

using std::vector;


namespace txml {
   class visitor_base;
   class xml_element;
   class xml_document;
   class xml_comment;
   class xml_text;
   class xml_declaration;


   class raw_buffer;




   class tchilds {

      std::vector<xml_node*> _childs;
      static const size_t err = size_t( -1 );
   public:
      tchilds(): _childs() {}
      ~tchilds() {}

      size_t size() const {
         return _childs.size();
      }
      void append( xml_node* p ) {
         _childs.push_back( p );
      }
      bool contains( xml_node const* p ) {
         size_t pp = pos( p );

         if( pp == err ) {
            return false;
         }

         return true;
      }
      std::vector<xml_node*> const& childs()const {
         return _childs;
      }
      std::vector<std::string> values()const;


      size_t pos( xml_node const* p ) const;
      xml_node* next( xml_node const* p ) const;


      xml_node* prev( xml_node const* p ) const {
         size_t i = pos( p );

         if( i == err ) {
            return nullptr;
         }

         if( i == 0 ) {
            return nullptr;
         }

         i--;
         xml_node* n = _childs[i];
         return n;
      }

      xml_node* first() const {
         if( _childs.size() == 0 ) {
            return nullptr;
         }

         xml_node* n = _childs[0];
         return n;
      }
      xml_node* last() const {
         if( _childs.size() == 0 ) {
            return nullptr;
         }

         xml_node* n = _childs[ _childs.size() - 1];
         return n;
      }
      xml_element* first_element( std::string const& val )const;
      xml_element* last_element( std::string const& val )const;
      xml_element* first_element()const;
      xml_element* last_element()const;

      void clear() {
         _childs.clear();
      }

   };


   class xml_node {

      friend class xml_element;
      friend class xml_document;

   public:



      virtual ~xml_node();

      DEFINE_ENUM_WITH_STRING_CONVERSIONS( eType, ( DOC )( ELEM )( COMMENT )( TEXT )( DECL ) );


      eType type() const;

      // the master document
      xml_document* document() ;

      /**
      in Document:   filename of the xml file
      in Element:    name of the element
      in Comment:    the comment text
      in Text:       the text string
      */
      const std::string value() const;
      void value( const std::string& );

      void clear();

      // first/last in list
      xml_node* first_child()const;
      xml_node* last_child()const;

      // first/last in list with check if is element, no comment, no declaration, no text
      xml_element* first_child_element( const std::string& _value )  const          ;
      xml_element* last_child_element( const std::string& _value ) const;

      // neighbour list, via parent
      xml_node* prev() const;
      xml_node* next() const;


      // ad a child to list
      xml_node* link_end_child( xml_node* addThis );

      // special for comment
      void insert_comment_before( xml_node* node, xml_comment* comment );


      // visitor
      // returns true, if not accepted, means go recurse to next
      virtual v_ret accept( visitor_base* visitor ) const = 0;


      xml_node*   parent( )const {
         return _parent;
      }
      path const& lookuppath()const {
         return _path;
      }
      path& lookuppath() {
         return _path;
      }

      std::vector<xml_node*> const& getChilds()const {
         return _childs.childs();
      }
      std::vector<std::string> getChildValues()const {
         return _childs.values();
      }

   protected:
      xml_node( eType _type );
      xml_node( eType t, std::string const& );

      xml_node* create_in_elem( raw_buffer& );
      xml_node* create_in_doc( raw_buffer& );
      xml_node* create( std::string const& );
      void         parent( xml_node* p ) ;

      virtual void parse( raw_buffer& ) = 0;




   private:

      // path to node
      path _path;

      eType     _type;

      tchilds _childs;

      xml_node*     _parent;

      std::string _value;
      std::string _rawxml;
      xml_node( const xml_node& );
      void operator=( const xml_node& base );
   };


} // end of namespace txml



#endif

//EOF


