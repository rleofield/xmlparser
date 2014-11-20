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

#include "stringhelper.h"
#include "alloccheck.h"


using namespace std;



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

         vector<txml::xml_attribute> const& v = pElement->Attributes();
         vector<txml::xml_attribute>::const_iterator begin = v.begin();
         vector<txml::xml_attribute>::const_iterator end = v.end();

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
         cout << "Element [" <<  pParent->value() << "]";
         num = dump_attribs_to_stdout( dynamic_cast<txml::xml_element const*>( pParent ), indent + 1 );

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

      case txml::xml_node::eNodeType::COMMENT:
         cout << "Comment: [" << pParent->value() << "]";
         break;

      case txml::xml_node::eNodeType::TEXT:
         pText = dynamic_cast<txml::xml_text*>( const_cast<txml::xml_node*>( pParent ) );
         v = pText->value();
         v = txml::decodeEntities( v );
         cout << "Text: [" << v << "]";
         break;

      case txml::xml_node::eNodeType::DECLARATION:
         cout << "Declaration";
         break;

      default:
         break;
      }

      cout << endl;
      txml::xml_node const* pChild;

      for( pChild = pParent->firstChild(); pChild != 0; pChild = pChild->next() ) {
         xml_dump_to_stdout( pChild, indent + 1 );
      }
   }


   void build_simple_doc( ) {
      // Make xml: <?xml ..><Hello>World</Hello>
      txml::xml_document doc;
      txml::xml_node* decl = txml::xml_declaration::create( tlog_lfm_ );
      txml::xml_element* element = txml::xml_element::create( "Hello" );
      txml::xml_node* text = txml::xml_text::create( tlog_lfm_, "World" );
      element->linkEndChild( text );
      doc.linkEndChild( decl );
      doc.linkEndChild( element );
      txml::xml_element* element2 = txml::xml_element::create( "ele2" );
      txml::xml_node* text2 = txml::xml_text::create( tlog_lfm_, "World2" );
      element2->linkEndChild( text2 );
      doc.linkEndChild( element2 );

      xmlinterface::tXmlInterface t;
      t.parse( doc );
      t.save( "madeByHand.xml" );
   }


   void write_simple_doc2( ) {
      // same as write_simple_doc1 but add each node
      // as early as possible into the tree.

      txml::xml_document doc;
      txml::xml_node* decl = txml::xml_declaration::create( tlog_lfm_);
      //doc.linkEndChild( decl );

      txml::xml_element* element = txml::xml_element::create( "Hello" );
      doc.linkEndChild( element );

      txml::xml_node* text = txml::xml_text::create( "World" );
      element->linkEndChild( text );
      xmlinterface::tXmlInterface t;
      t.parse( doc );
      t.save( "madeByHand2.xml" );
   }






   void write_app_settings_doc() {
      txml::xml_document doc;
      //txml::xml_node* decl = txml::xml_declaration::create( );
      //doc.linkEndChild( decl );

      txml::xml_element* root = txml::xml_element::create( "MyApp" );
      doc.linkEndChild( root );

      //txml::xml_node* comment = txml::xml_comment::create( );
      //comment->value( " Settings for MyApp " );
      //root->linkEndChild( comment );

      txml::xml_element* msgs = txml::xml_element::create( "Messages" );
      root->linkEndChild( msgs );

      txml::xml_element* msg = txml::xml_element::create( "Welcome" );
      msg->linkEndChild( txml::xml_text::create( "Welcome to MyApp" ) );
      msgs->linkEndChild( msg );


      //doc->saveFile( "appsettings_smart_Welcome.xml" );
      xmlinterface::tXmlInterface t;
      t.parse( doc );
      txml::xml_document const* doc1 = t.document();
      list<string> v;
      doc1->serialize(v);

      msg = txml::xml_element::create( tlog_lfm_, "Farewell" );
      msg->linkEndChild( txml::xml_text::create( "Thank you for using MyApp" ) );
      msgs->linkEndChild( msg );

      txml::xml_element* windows = txml::xml_element::create( "Windows" );
      root->linkEndChild( windows );

      txml::xml_element* window = txml::xml_element::create( "Window" );
      windows->linkEndChild( window );
      window->setAttribute( "name", "MainFrame" );
      window->setAttribute( "x", "5" );
      window->setAttribute( "y", "15" );
      window->setAttribute( "w", "400" );
      window->setAttribute( "h", "250" );

      txml::xml_element* cxn = txml::xml_element::create( tlog_lfm_, "Connection" );
      root->linkEndChild( cxn );
      cxn->setAttribute( "ip", "192.168.0.1" );
      cxn->setAttribute( "timäout", "123.456" ); // floating point attrib

      //xmlinterface::tXmlInterface t;

      t.parse( doc );
      t.save( "appsettings_old.xml" );

   }




} // end of ns

//EOF

