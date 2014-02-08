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
#include <string>
#include <vector>
#include <iomanip>

#include <boost/assign.hpp>


#include "xml_interface.h"
#include "xml_builder.h"
#include "xml_utl.h"
#include "XmlElement.h"
#include "XmlText.h"
#include "XmlDocument.h"
#include "XmlDeclaration.h"
#include "XmlComment.h"

#include "stringhelper.h"
//#include "logger.h"
#include "alloccheck.h"

#include "win32.h"

using namespace std;

using alloccheck::t_alloc_line_file_method;

namespace demo {

   namespace {

      const unsigned int NUM_INDENTS_PER_SPACE = 2;


      string getIndent( unsigned int numIndents ) {
         unsigned int n = numIndents * NUM_INDENTS_PER_SPACE;
         string temp = rlf_hstring::fillup( " ", ' ', n );
         temp += '+';
         return temp;
      }


      int dump_attribs_to_stdout( txml::XmlElement const* pElement, unsigned int indent ) {
         if( !pElement ) {
            return 0;
         }

         vector<txml::XmlAttribute> const& v = pElement->Attributes();
         vector<txml::XmlAttribute>::const_iterator begin = v.begin();
         vector<txml::XmlAttribute>::const_iterator end = v.end();

         int count = static_cast<int>(end - begin);
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
   void xml_dump_to_stdout( txml::XmlNode const* pParent, unsigned int indent ) {
      if( !pParent ) {
         return;
      }

      int t = pParent->type();
      cout << getIndent( indent );
      int num;
      txml::XmlText* pText;
      string v;
      size_t n = indent * NUM_INDENTS_PER_SPACE;
      string temp = rlf_hstring::fillup( " ", ' ', n );

      switch( t ) {
      case txml::XmlNode::RL_XML_DOCUMENT:
         cout << "Document";
         break;

      case txml::XmlNode::RL_XML_ELEMENT:
         cout << "Element [" <<  pParent->value() << "]";
         num = dump_attribs_to_stdout( dynamic_cast<txml::XmlElement const*>( pParent ), indent + 1 );

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

      case txml::XmlNode::RL_XML_COMMENT:
         cout << "Comment: [" << pParent->value() << "]";
         break;

      case txml::XmlNode::RL_XML_TEXT:
         pText = dynamic_cast<txml::XmlText*>( const_cast<txml::XmlNode*>( pParent ) );
         v = pText->value();
         v = txml::decodeEntities( v );
         cout << "Text: [" << v << "]";
         break;

      case txml::XmlNode::RL_XML_DECLARATION:
         cout << "Declaration";
         break;
      default:
         break;
      }

      cout << endl;
      txml::XmlNode const* pChild;

      for( pChild = pParent->firstChild(); pChild != 0; pChild = pChild->next() ) {
         xml_dump_to_stdout( pChild, indent + 1 );
      }
   }


   void build_simple_doc( ) {
      // Make xml: <?xml ..><Hello>World</Hello>
      txml::XmlDocument doc;
      txml::XmlNode* decl = txml::XmlDeclaration::create( );
      txml::XmlElement* element = txml::XmlElement::create( "Hello" );
      txml::XmlNode* text = txml::XmlText::create( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ), "World" );
      element->linkEndChild( text );
      doc.linkEndChild( decl );
      doc.linkEndChild( element );
      txml::XmlElement* element2 = txml::XmlElement::create( "ele2" );
      txml::XmlNode* text2 = txml::XmlText::create( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ), "World2" );
      element2->linkEndChild( text2 );
      doc.linkEndChild( element2 );

      xmlinterface::tXmlInterface t;
      t.parse( doc );
      t.save( "madeByHand.xml" );
   }


   void write_simple_doc2( ) {
      // same as write_simple_doc1 but add each node
      // as early as possible into the tree.

      txml::XmlDocument doc;
      txml::XmlNode* decl = txml::XmlDeclaration::create( );
      doc.linkEndChild( decl );

      txml::XmlElement* element = txml::XmlElement::create( "Hello" );
      doc.linkEndChild( element );

      txml::XmlNode* text = txml::XmlText::create( "World" );
      element->linkEndChild( text );
      xmlinterface::tXmlInterface t;
      t.parse( doc );
      t.save( "madeByHand2.xml" );
   }




   void write_app_settings_doc() {
      txml::XmlDocument doc;
      txml::XmlNode* decl = txml::XmlDeclaration::create( );
      doc.linkEndChild( decl );

      txml::XmlElement* root = txml::XmlElement::create( "MyApp" );
      doc.linkEndChild( root );

      txml::XmlNode* comment = txml::XmlComment::create( );
      comment->value( " Settings for MyApp " );
      root->linkEndChild( comment );

      txml::XmlElement* msgs = txml::XmlElement::create( "Messages" );
      root->linkEndChild( msgs );

      txml::XmlElement* msg = txml::XmlElement::create( "Welcome" );
      msg->linkEndChild( txml::XmlText::create( "Welcome to MyApp" ) );
      msgs->linkEndChild( msg );

      msg = txml::XmlElement::create( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ), "Farewell" );
      msg->linkEndChild( txml::XmlText::create( "Thank you for using MyApp" ) );
      msgs->linkEndChild( msg );

      txml::XmlElement* windows = txml::XmlElement::create( "Windows" );
      root->linkEndChild( windows );

      txml::XmlElement* window = txml::XmlElement::create( "Window" );
      windows->linkEndChild( window );
      window->setAttribute( "name", "MainFrame" );
      window->setAttribute( "x", "5" );
      window->setAttribute( "y", "15" );
      window->setAttribute( "w", "400" );
      window->setAttribute( "h", "250" );

      txml::XmlElement* cxn = txml::XmlElement::create( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ), "Connection" );
      root->linkEndChild( cxn );
      cxn->setAttribute( "ip", "192.168.0.1" );
      cxn->setAttribute( "timäout", "123.456" ); // floating point attrib

      xmlinterface::tXmlInterface t;

      t.parse( doc );
      t.save( "appsettings.xml" );

   }




} // end of ns

//EOF

