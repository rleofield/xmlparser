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


      int dump_attribs_to_stdout( txml::xml_element const* pElement, unsigned int indent ) {
         if( !pElement ) {
            return 0;
         }

         vector<xml_attribute> const& v = pElement->Attributes();
         vector<xml_attribute>::const_iterator begin = v.begin();
         vector<xml_attribute>::const_iterator end = v.end();

         int count = static_cast<int>( end - begin );
         string pIndent = getIndent( indent );
         cout << endl;

         while( begin != end ) {
            cout << pIndent << begin->name() << ": value=[" <<  begin->value() << "]";
            cout << " int=" << xmlinterface::to_int( begin->value() );
            cout << " d=";
            cout << fixed;
            cout << setprecision( 1 ) << xmlinterface::to_double( begin->value() ) << endl;
            cout << endl;
            ++begin;
         }

         return count;
      }

   } // end of ns local


   // writes simple readable format of to cout (no xml)
   void xml_dump_to_stdout( txml::xml_node const* pParent, unsigned int indent ) {
      if( !pParent ) {
         return;
      }

      txml::xml_node::eNodeType t = pParent->type();
      cout << getIndent( indent );
      int num;
      txml::xml_text* pText;
      string v;
      size_t n = indent * NUM_INDENTS_PER_SPACE;
      string temp = rlf_hstring::fillup( " ", ' ', n );

      switch( t ) {
      case txml::xml_node::eNodeType::DOCUMENT:
         cout << "Document";
         break;

      case txml::xml_node::eNodeType::ELEMENT:
         std::cout << "Element [" <<  pParent->value() << "]";
         num = dump_attribs_to_stdout( dynamic_cast<xml_element const*>( pParent ), indent + 1 );

         switch( num ) {
         case 0:
            cout << " (No attributes)" ;
            break;

         case 1:
            cout << temp << "1 attribute";
            break;

         default:
            cout << temp << num <<  " attributes";
            break;
         }

         break;

      case xml_node::eNodeType::COMMENT:
         cout << "Comment: [" << pParent->value() << "]";
         break;

      case xml_node::eNodeType::TEXT:
         pText = dynamic_cast<xml_text*>( const_cast<xml_node*>( pParent ) );
         v = pText->value();
         v = txml::decodeEntities( v );
         cout << "Text: [" << v << "]";
         break;

      case xml_node::eNodeType::DECLARATION:
         cout << "Declaration";
         break;

      default:
         break;
      }

      cout << endl;
      xml_node const* pChild;

      for( pChild = pParent->firstChild(); pChild != 0; pChild = pChild->next() ) {
         xml_dump_to_stdout( pChild, indent + 1 );
      }
   }


   void build_simple_doc( ) {
      // Make xml: <?xml ..><Hello>World</Hello>
      xml_document doc;
      xml_node* decl = xml_declaration::create( tlog_lfm_ );
      xml_element* element = xml_element::create( "Hello" );
      xml_node* text = xml_text::create( tlog_lfm_, "World" );
      //text->parent( &doc );
      element->link_end_child( text );
      doc.link_end_child( decl );
      doc.link_end_child( element );
      xml_element* element2 = xml_element::create( "ele2" );
      xml_node* text2 = xml_text::create( tlog_lfm_, "World2" );
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
      xml_node* decl = xml_declaration::create( tlog_lfm_ );
      //doc.linkEndChild( decl );

      xml_element* element = xml_element::create( "Hello" );
      doc.link_end_child( element );

      xml_node* text = xml_text::create( "World" );
      element->link_end_child( text );
      xmlinterface::tXmlInterface t;
      t.rebuild( doc );
      t.save( "madeByHand2.xml" );
   }






   void write_app_settings_doc() {
      xml_document doc;
      //xml_node* decl = xml_declaration::create( );
      //doc.linkEndChild( decl );

      xml_element* root = xml_element::create( "MyApp" );
      doc.link_end_child( root );

      //xml_node* comment = xml_comment::create( );
      //comment->value( " Settings for MyApp " );
      //root->linkEndChild( comment );

      xml_element* msgs = xml_element::create( "Messages" );
      root->link_end_child( msgs );

      xml_element* msg = xml_element::create( "Welcome" );
      msg->link_end_child( xml_text::create( "Welcome to MyApp" ) );
      msgs->link_end_child( msg );


      //doc->saveFile( "appsettings_smart_Welcome.xml" );
      xmlinterface::tXmlInterface t;
      t.rebuild( doc );
      xml_document const* doc1 = t.document();
      list<string> v;
      doc1->serialize( v );

      msg = xml_element::create( tlog_lfm_, "Farewell" );
      msg->link_end_child( xml_text::create( "Thank you for using MyApp" ) );
      msgs->link_end_child( msg );

      xml_element* windows = xml_element::create( "Windows" );
      root->link_end_child( windows );

      xml_element* window = xml_element::create( "Window" );
      windows->link_end_child( window );
      window->setAttribute( "name", "MainFrame" );
      window->setAttribute( "x", "5" );
      window->setAttribute( "y", "15" );
      window->setAttribute( "w", "400" );
      window->setAttribute( "h", "250" );

      xml_element* cxn = xml_element::create( tlog_lfm_, "Connection" );
      root->link_end_child( cxn );
      cxn->setAttribute( "ip", "192.168.0.1" );
      cxn->setAttribute( "timäout", "123.456" ); // floating point attrib

      //xmlinterface::tXmlInterface t;

      t.rebuild( doc );
      t.save( "appsettings_old.xml" );

   }




} // end of ns

//EOF

