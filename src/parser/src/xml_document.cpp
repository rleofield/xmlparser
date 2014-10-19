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

#include "xml_declaration.h"
#include "xml_document.h"
#include "xml_printer.h"
#include "rawxml_position.h"

#include "xml_utl.h"
#include "xml_exception.h"
#include "rList.h"
#include "wList.h"
#include "alloccheck.h"



using namespace std;

using alloccheck::t_alloc_line_file_method;

namespace txml {




   namespace {

      // http://de.wikipedia.org/wiki/Byte_Order_Mark
      const char bom_UTF8[3]     = {( char )0xEF, ( char )0xBB, ( char )0xBF};
      const char bom_UTF16_BE[2] = {( char )0xFE, ( char )0xFF };
      const char bom_UTF16_LE[2] = {( char )0xFF, ( char )0xFE };


      bool checkMsBOM( Encoding& encoding, string const& s ) {

         // Check for the Microsoft UTF-8 lead bytes.
         if( s.size() < 3 ) {
            return false;
         }

         if( s == string( bom_UTF8, sizeof( bom_UTF8 ) ) ) {
            encoding = ENCODING_UTF8;
            return true;
         }

         // other BOM ist not supported
         return false;
      }

   }


   bool xml_document::_isWhiteSpacePreserved = true;
   /// http://www.opentag.com/xfaq_enc.htm
   Encoding xml_document::_encoding = ENCODING_UNKNOWN;
   bool xml_document::useMicrosoftBOM = false;
   std::locale xml_document::loc; // = locEN ;

   void xml_document::setPreserveWhiteSpace( bool b )    {
      _isWhiteSpacePreserved = b;
   }

   bool xml_document::isWhiteSpacePreserved()            {
      return _isWhiteSpacePreserved;
   }


   void* xml_document::operator new( size_t size, t_alloc_line_file_method const& lfm ) {
      return alloccheck::LocalAlloc( size, lfm );
   }
   void xml_document::operator delete( void* p ) {
      xml_node* n = reinterpret_cast<xml_node*>( p );
      alloccheck::LocalDelete( n );
   }


   Encoding xml_document::encoding() {
      return _encoding;
   }
   void xml_document::encoding( Encoding e ) {
      _encoding = e;
   }

   void xml_document::parse( rawxml_position& pos ) {
      if( pos.is_end() ) {
         throw xml_exception( t_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_document_empty, msg_document_empty );
      }

      encoding( ENCODING_UTF8 );

      // Check for the Microsoft UTF-8 lead bytes.
      string s = pos.next( 3 );
      useMicrosoftBOM = checkMsBOM( _encoding, s );

      if( useMicrosoftBOM ) {
         pos += 3;
      }

      pos.skip();

      if( *pos == 0 ) {
         throw xml_exception( t_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_document_empty, msg_document_empty );
      }

      //      keyentry ke( _node_value );
      //      ke.node( this );
      //      _lookuppath.add( ke );

      while( !pos.is_end() ) {
         xml_node* node = identify( pos );
         if( node ) {
            node->_lookuppath = _lookuppath;
            node->parse( pos );
            linkEndChild( node );
            xml_declaration const* decl = dynamic_cast<xml_declaration const*>( node );


            if( decl != nullptr ) {
               if( boost::iequals( decl->encoding(), UTF_8 ) ) {
                  _encoding = ENCODING_UTF8;
               } else {
                  _encoding = ENCODING_LEGACY;
               }
            }
         } else {
            break;
         }

         pos.skip();
      }

      if( firstChild() == 0 ) {
         throw xml_exception( t_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_document_empty, msg_document_empty );
      }

      return;
   }


   xml_document::xml_document() : xml_node( xml_node::RL_XML_DOCUMENT ) {}


   xml_document::xml_document( const xml_document& copy_ ) :
      xml_node( xml_node::RL_XML_DOCUMENT ) {
      copy_.copy( *this );
   }


   xml_document& xml_document::operator=( const xml_document& copy_ ) {
      clear();
      copy_.copy( *this );
      return *this;
   }


   bool xml_document::parseStart( std::list<string>  const& l ) {
      class addline {
      public:
         vector8_t& _v;
         addline( vector8_t& v ): _v( v ) {}
         void operator()( string const& s ) {
            _v.insert( _v.end(), s.begin(), s.end() );
         }
      };

      clear();
      vector8_t  v;
      for_each( l.begin(), l.end() , addline( v ) );
      rawxml_position pp( v );
      parse( pp );
      return true;
   }

   void xml_document::clear() {
      xml_node::clear();
   }

   bool xml_document::saveFile( string const& filename ) const {

      xml_printer p;
      this->accept( &p );
      std::list<string> text;
      p.Buffer( text );
      bool overwrite = true;
      rlf_txtrw::t_write_ascii()( filename, text, overwrite );
      return false;
   }

   void xml_document::serialize( list<string> &v ) const {
      v.clear();
      xml_printer p;
      this->accept( &p );
      std::list<string> text;
      p.Buffer( text );

      if( text.size() > 0 ) {
         if( useMicrosoftBOM ) {
            string temp;
            temp.append( bom_UTF8, bom_UTF8 + sizeof( bom_UTF8 ) );
            temp += text.front();    // set BOM to start of first line
            text.pop_front();        // remove first lien
            text.push_front( temp ); // add again with bom
         }
      }

      v = text;
   }





   //   const XmlElement* xml_document::rootElement() const      {
   //      return firstChildElement();
   //   }
   //   XmlElement* xml_document::rootElement()               {
   //      return firstChildElement();
   //   }

   void xml_document::copy( xml_document& target ) const {
      target.value( value() );
      target.useMicrosoftBOM = useMicrosoftBOM;
      xml_node const* node = nullptr;

      for( node = firstChild(); node != nullptr; node = node->next() ) {
         target.linkEndChild( node->clone() );
      }
   }

   xml_node* xml_document::clone() const {
      xml_document* pclone = new( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ) ) xml_document;

      copy( *pclone );
      return pclone;
   }


   bool xml_document::accept( xml_visitor* visitor ) const {
      bool notAccepted = visitor->enter( *this );

      if( notAccepted ) {

         for( const xml_node* node = firstChild(); node != nullptr; node = node->next() ) {
            notAccepted = node->accept( visitor );

            if( !notAccepted ) {
               break;
            }
         }
      }

      return visitor->visitExit( *this );
   }







} // end of namespace txml



// EOF
