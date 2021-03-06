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
#include <string>
#include <vector>
#include <iomanip>

#include <boost/assign.hpp>


#include "xml_interface.h"
#include "xml_builder.h"
#include "xml_utl.h"
#include "xml_element.h"
#include "xml_text.h"
#include "xml_document.h"
#include "xml_declaration.h"
#include "xml_comment.h"
#include "xml_attribute.h"

#include "stringhelper.h"
#include "alloccheck.h"

using txml::xml_node;
using namespace std;
using txml::xml_attribute;
using txml::xml_document;
using txml::xml_element;
using txml::xml_text;
using txml::xml_declaration;

namespace demo {

   namespace {

      const unsigned int NUM_INDENTS_PER_SPACE = 2;


      string getIndent( unsigned int numIndents ) {
         unsigned int n = numIndents * NUM_INDENTS_PER_SPACE;
         string temp = rlf_hstring::fillup( " ", ' ', n );
         temp += '+';
         return temp;
      }


      int dump_attribs_to_stdout( std::ostream& o, txml::xml_element const* node, unsigned int indent ) {
         if( !node ) {
            return 0;
         }

         vector<xml_attribute> const& v = node->Attributes();
         vector<xml_attribute>::const_iterator begin = v.begin();
         vector<xml_attribute>::const_iterator end = v.end();

         string s_indent = getIndent( indent );
         o << std::endl;

         while( begin != end ) {
            o << s_indent << begin->name() << ": value=[" <<  begin->value() << "]";
            o << " int=" << begin->value();
            o << " d=";
            o << fixed;
            o << setprecision( 1 ) << begin->value() << endl;
            o << endl;
            ++begin;
         }

         int count = static_cast<int>( v.size() );
         return count;
      }

   } // end of ns local


   // writes simple readable format of to cout (no xml)
   void node_dump( std::ostream& o, txml::xml_node const* node, unsigned int indent ) {
      if( node == nullptr ) {
         return;
      }

      xml_node::eType t = node->type();
      o << getIndent( indent );

      txml::xml_text* pText;
      string v;
      size_t n = indent * NUM_INDENTS_PER_SPACE;
      string temp = rlf_hstring::fillup( " ", ' ', n );

      switch( t ) {
      case txml::xml_node::eType::DOC:
         o << "Document";
         break;

      case txml::xml_node::eType::ELEM: {
         o << "Element [" <<  node->value() << "]";
         vector<xml_attribute> const& v1 = dynamic_cast<xml_element const*>( node )->Attributes();
         size_t s = v1.size();
         int num = dump_attribs_to_stdout( o, dynamic_cast<xml_element const*>( node ), indent + 1 );

         switch( num ) {
         case 0:
            o << " (No attributes)" ;
            break;

         case 1:
            o << temp << "1 attribute";
            break;

         default:
            o << temp << num <<  " attributes";
            break;
         }
      }
      break;

      case xml_node::eType::COMMENT:
         o << "Comment: [" << node->value() << "]";
         break;

      case xml_node::eType::TEXT:
         pText = dynamic_cast<xml_text*>( const_cast<xml_node*>( node ) );
         v = pText->value();
         v = txml::decodeEntities( v );
         o << "Text: [" << v << "]";
         break;

      case xml_node::eType::DECL:
         o << "Declaration";
         break;

      default:
         break;
      }

      o << endl;
      xml_node const* pChild;


      std::vector<xml_node*>  const& childs = node->getChilds();

      for( xml_node const * no : childs ) {
         node_dump( o, no, indent + 1 );
      }


   }


   void build_simple_doc( ) {
      // Make xml: <?xml ..><Hello>World</Hello>
      xml_document doc;
      xml_node* decl = doc.declaration_create( tlfm_ );
      xml_node* element = doc. element_create( "Hello" );
      xml_node* text = doc.text_create( tlfm_, "World" );
      //text->parent( &doc );
      element->link_end_child( text );
      doc.link_end_child( decl );
      doc.link_end_child( element );
      xml_node* element2 = doc.element_create( "ele2" );
      xml_node* text2 = doc.text_create( tlfm_, "World2" );
      element2->link_end_child( text2 );
      doc.link_end_child( element2 );

      xmlinterface::tXmlInterface t;
      t.rebuild( doc );
      t.save( "madeByHand.xml" );
   }


   void write_simple_doc2( ) {
      // same as write_simple_doc1 but add each node
      // as early as possible into the tree.

      xml_document doc;
      xml_node* decl = doc.declaration_create( tlfm_ );
      doc.link_end_child( decl );

      xml_node* element = doc.element_create( "Hello" );
      doc.link_end_child( element );

      xml_node* text = doc.text_create( "World" );
      element->link_end_child( text );
      xmlinterface::tXmlInterface t;
      t.rebuild( doc );
      t.save( "madeByHand2.xml" );
      t.save( "madeByHand2.xml" );
   }






   void write_app_settings_doc() {
      xml_document doc;

      xml_node* decl =  doc.declaration_create();
      doc.link_end_child( decl );

      xml_node* root = doc.element_create( "MyApp" );
      doc.link_end_child( root );

      xml_node* comment = doc.comment_create();
      comment->value( " Settings for MyApp " );
      root->link_end_child( comment );

      xml_node* msgs = doc.element_create( "Messages" );
      root->link_end_child( msgs );

      xml_node* msg = doc.element_create( "Welcome" );
      msg->link_end_child( doc.text_create( "Welcome to MyApp" ) );
      msgs->link_end_child( msg );


      //doc->saveFile( "appsettings_smart_Welcome.xml" );
      xmlinterface::tXmlInterface t;
      t.rebuild( doc );
      xml_document* doc1 = t.document();
      list<string> v;
      bool pretty_print = true;
      v = doc1->serialize_to_list( "        ", pretty_print );

      msg = doc1-> element_create( tlfm_, "Farewell" );
      msg->link_end_child( doc1-> text_create( "Thank you for using MyApp" ) );
      msgs->link_end_child( msg );

      xml_node* windows = doc1->element_create( "Winfffffffffffffffdows" );
      root->link_end_child( windows );
      windows->link_end_child( doc1-> text_create( "in win" ) );

      xml_element* window = doc1->element_create( "Window" ) ;
      windows->link_end_child( window );
      window->attribute( "name", "MainFrame" );
      window->attribute( "x", "5" );
      window->attribute( "y", "15" );
      window->attribute( "w", "400" );
      window->attribute( "h", "250" );

      xml_element* cxn = doc1->element_create( tlfm_, "Connection" ) ;
      root->link_end_child( cxn );
      cxn->attribute( "ip", "192.168.0.1" );
      cxn->attribute( "timäout", "123.456" ); // floating point attrib


      t.rebuild( doc );
      t.save( "appsettings_old.xml" );
      t.save( "appsettings_old1.xml" );

   }




} // end of ns

//EOF

