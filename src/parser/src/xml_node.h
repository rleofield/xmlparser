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


#include <map>
#include <string>

#include "xml_exception.h"

#include "keyentries.h"

struct null_deleter {
   void operator()( void const* ) const {
   }
};



namespace txml {
   class xml_visitor;
   class xml_element;
   class xml_document;
   class xml_comment;
   class xml_text;
   class xml_declaration;




   class raw_buffer;

   // nodeholder
   class nodeh {
   public:
      xml_node*     firstChild;
      xml_node*     lastChild;

      xml_node*      prev_sibling;
      xml_node*      next_sibling;
      nodeh(): firstChild( nullptr ), lastChild( nullptr ), prev_sibling( nullptr ), next_sibling( nullptr ) {}
   };


   class xml_node {

      friend class xml_element;
      friend class xml_document;

   public:


      //static std::vector<std::string> acc_all ;
      //std::vector<std::string> acc ;
      //std::string accs()const;
      //static std::string accs_all();

      virtual ~xml_node();

      enum class eNodeType : uint32_t {
         DOCUMENT,
         ELEMENT,
         COMMENT,
         TEXT,
         DECLARATION
      };


      /**
      in Document:   filename of the xml file
      in Element:    name of the element
      in Comment:    the comment text
      in Text:       the text string
      */
      const std::string value() const;
      void value( const std::string& );
      std::string tvalue() const;

      void clear();

      const xml_node* firstChild()const;
      xml_node* firstChild();
      const xml_element* firstChildElement( const std::string& _value ) const   ;
      xml_element* firstChildElement( const std::string& _value )            ;


      const xml_node* last_child()const;
      xml_node* last_child();
      const xml_element* last_child_element( const std::string& _value ) const;
      xml_element* last_child_element( const std::string& _value ) ;


      xml_node* link_end_child( xml_node* addThis );
      void insert_comment_before( xml_node* node, xml_comment* comment );


      const xml_node* prev() const;
      xml_node* prev();
      const xml_node* next() const;
      xml_node* next();

      eNodeType type() const;

      xml_document const* getDocument() ;

      bool noChildren() const {
         return firstChild() == nullptr;
      }


      //virtual xml_node* clone() const = 0;

      virtual bool accept( xml_visitor* visitor ) const = 0;


      xml_node const*  parent( )const {
         return _parent;
      }
      xml_node*   parent( ) {
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
      xml_node( eNodeType _type );
      xml_node( eNodeType t, std::string const& );

      xml_node* identify_in_elem( raw_buffer&  );
      xml_node* identify_in_doc( raw_buffer&  );
      xml_node* createNode( std::string const&  );
      void         parent( xml_node* p ) ;

      virtual void parse( raw_buffer& ) = 0;

   private:

      // path to node
      keyentries _lookuppath ;

      eNodeType     _type;

      nodeh _nh;

      xml_node*     _parent;

      std::string _node_value;
      std::string _rawxml;
      xml_node( const xml_node& );
      void operator=( const xml_node& base );
   };

   std::string to_string( xml_node::eNodeType  t ) ;


} // end of namespace txml



#endif

//EOF


