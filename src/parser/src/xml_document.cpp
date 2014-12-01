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

#include "stringhelper.h"

#include "xml_declaration.h"
#include "xml_document.h"
#include "xml_printer.h"
#include "rawxml_position.h"

#include "xml_utl.h"
#include "xml_exception.h"
#include "rList.h"
#include "wList.h"
#include "alloccheck.h"
#include "tLog_Category_default.h"


using namespace std;



namespace txml {




   namespace {

      // http://de.wikipedia.org/wiki/Byte_Order_Mark
      const char bom_utf8[3]     = {( char )0xEF, ( char )0xBB, ( char )0xBF};
      const char bom_utf16_big_endian[2] = {( char )0xFE, ( char )0xFF };
      const char bom_utf16_lower_endian[2] = {( char )0xFF, ( char )0xFE };


      const string string_bom_utf8 = string( bom_utf8, sizeof( bom_utf8 ) );
      const string string_bom_utf16_big_endian = string( bom_utf16_big_endian, sizeof( bom_utf16_big_endian ) );
      const string string_bom_utf16_lower_endian = string( bom_utf16_lower_endian, sizeof( bom_utf16_lower_endian ) );



   }


   bool xml_document::_isWhiteSpacePreserved = true;
   /// http://www.opentag.com/xfaq_enc.htm
   Encoding xml_document::_encoding = Encoding::UNKNOWN;
   std::string xml_document::bom = string();
   std::locale xml_document::loc; // = locEN ;

   void xml_document::setPreserveWhiteSpace( bool b )    {
      _isWhiteSpacePreserved = b;
   }

   bool xml_document::isWhiteSpacePreserved()            {
      return _isWhiteSpacePreserved;
   }


   void* xml_document::operator new( size_t size, t_lfm const& lfm ) {
      void* p =  alloccheck::checked_alloc( size, lfm );
      return p;
   }
   void xml_document::operator delete( void* p ) {
      xml_node* n = reinterpret_cast<xml_node*>( p );
      alloccheck::checked_delete( n );
   }


   Encoding xml_document::encoding() {
      return _encoding;
   }
   void xml_document::encoding( Encoding e ) {
      _encoding = e;
   }

   void xml_document::parse( rawxml_position& pos ) {
      if( pos.is_end() ) {
         throw xml_exception( tlog_lfm_,
                              eException::document_empty, msg_document_empty );
      }

      encoding( Encoding::UTF8 );

      // Check for the Microsoft UTF-8 lead bytes.
      string s = pos.next( string_bom_utf8.size() );

      if( s == string_bom_utf8 ) {
         encoding( Encoding::UTF8 );
         bom = string_bom_utf8;
         pos += string_bom_utf8.size();
      }

      if( *pos == 0 ) {
         throw xml_exception( tlog_lfm_,
                              eException::document_empty, msg_document_empty );
      }

      //      keyentry ke( _node_value );
      //      ke.node( this );
      //      _lookuppath.add( ke );

      while( !pos.is_end() ) {
         xml_node* node = identify( pos );

         if( node ) {
            node->_lookuppath = _lookuppath;
            node->parse( pos );
            xml_declaration const* decl = dynamic_cast<xml_declaration const*>( node );
            if( decl != nullptr ) {
               if( boost::iequals( decl->encoding(), UTF_8 ) ) {
                  _encoding = Encoding::UTF8;
               } else {
                  _encoding = Encoding::LEGACY;
               }
            }
            linkEndChild( node );
         } else {
            break;
         }

         pos.skip();
      }

      if( firstChild() == 0 ) {
         throw xml_exception( tlog_lfm_,
                              eException::document_empty, msg_document_empty );
      }

      return;
   }


   xml_document::xml_document() : xml_node( xml_node::eNodeType::DOCUMENT ) {
      //LOGT_INFO("");
   }

   xml_document::~xml_document() {
      //LOGT_INFO("");
   }



   bool xml_document::startparsing( std::vector<string>  const& l ) {
      class add {
      public:
         vector8_t& _v;
         add( vector8_t& v ): _v( v ) {}
         void operator()( string const& s ) {
            _v.insert( _v.end(), s.begin(), s.end() );
         }
      };

      clear();
      ph::clear_pointers();
      vector8_t  v;
      for_each( l.begin(), l.end() , add( v ) );
      // add on space, to prevent end of file after last element
      v.push_back( ' ' );
      rawxml_position pp( v );
      xml_node::acc_all.clear();
      parse( pp );
      return true;
   }

   void xml_document::clear() {
      xml_node::clear();
      if( usePointerContainer ) {
         ph::clear_pointers();
      }
   }

   bool xml_document::save( string const& filename ) const {

      std::vector<string> text;
      serialize( text );
      bool overwrite = true;
      rlf_txtrw::t_write_ascii()( filename, text, overwrite );
      return false;
   }

   void xml_document::serialize( list<string>& v ) const {
      v.clear();
      xml_printer p;
      this->accept( &p );
      std::list<string> text;
      string const& s = p.string_buffer();
      rlf_hstring::string_to_list( s, text );

      if( text.size() > 0 ) {
         if( bom.size() > 0 ) {
            string temp = bom;
            temp += text.front();    // set BOM to start of first line
            text.pop_front();        // remove first lien
            text.push_front( temp ); // add again with bom
         }
      }

      v = text;
   }

   void xml_document::serialize( vector<string>& v ) const {
      v.clear();
      xml_printer p;
      this->accept( &p );

      string const& s = p.string_buffer();
      rlf_hstring::string_to_vector( s, v );

      if( bom.size() > 0 ) {
         v[0] = bom + v[0]; // add with bom
      }

   }


   bool xml_document::accept( xml_visitor* visitor ) const {
      bool notAccepted = visitor->enter( *this );

      string n;

      if( notAccepted ) {

         for( const xml_node* node = firstChild(); node != nullptr; node = node->next() ) {
            notAccepted = node->accept( visitor );
            n = node->value();

            if( !notAccepted ) {
               break;
            }
         }
      }

      return visitor->exit( *this );
   }







} // end of namespace txml



// EOF
