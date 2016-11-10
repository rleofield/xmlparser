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

#include "xml_comment.h"
#include "xml_declaration.h"
#include "xml_element.h"
#include "xml_text.h"

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
      const char bom_utf8[3]     = {static_cast<char>( 0xEF ), static_cast<char>( 0xBB ), static_cast<char>( 0xBF )};
      const char bom_utf16_big_endian[2] = {static_cast<char>( 0xFE ), static_cast<char>( 0xFF ) };
      const char bom_utf16_lower_endian[2] = {static_cast<char>( 0xFF ), static_cast<char>( 0xFE ) };


      const string string_bom_utf8 = string( bom_utf8, sizeof( bom_utf8 ) );
      const string string_bom_utf16_big_endian = string( bom_utf16_big_endian, sizeof( bom_utf16_big_endian ) );
      const string string_bom_utf16_lower_endian = string( bom_utf16_lower_endian, sizeof( bom_utf16_lower_endian ) );



   }


   bool xml_document::_isWhiteSpacePreserved = true;
   /// http://www.opentag.com/xfaq_enc.htm
   Encoding xml_document::_encoding = Encoding::UNKNOWN;
   std::string xml_document::_bom = string();
   std::locale xml_document::loc; // = locEN ;

   void xml_document::preserve_white_space( bool b )    {
      _isWhiteSpacePreserved = b;
   }

   bool xml_document::preserve_white_space()            {
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

   void xml_document::parse( raw_buffer& buffer ) {

      if( buffer.is_at_end() ) {
         throw Xml_exception( eEx::parse, msg_document );
      }

      encoding( Encoding::UTF8 );



      // Check for the Microsoft UTF-8 lead bytes.
      string s = buffer.next( string_bom_utf8.size() );

      if( s == string_bom_utf8 ) {
         encoding( Encoding::UTF8 );
         _bom = string_bom_utf8;
         buffer.advance( string_bom_utf8.size() );
      }

      if( buffer.value() == 0 ) {
         throw Xml_exception( eEx::parse, msg_document );
      }


      while( !buffer.is_at_end() ) {
         xml_node* node = create_in_doc( buffer );

         if( node ) {
            node->_path = _path;
            node->parse( buffer );

            if( encoding() == Encoding::UNKNOWN ) {
               xml_declaration const* decl = dynamic_cast<xml_declaration const*>( node );

               if( decl != nullptr ) {
                  if( boost::iequals( decl->encoding(), UTF_8 ) ) {
                     _encoding = Encoding::UTF8;
                  } else {
                     _encoding = Encoding::LEGACY;
                  }
               }
            }

            link_end_child( node );
         } else {
            break;
         }

         buffer.skip();
      }

      if( first_child() == nullptr ) {
         throw Xml_exception( eEx::parse, msg_document );
      }

   }


   xml_document::xml_document() : xml_node( xml_node::eType::DOC ), nodes() {
   }

   xml_document::~xml_document() {

   }




   xml_comment*  xml_document::comment_create( t_lfm const& lfmcIn ) {
      xml_comment* p = xml_comment::create( lfmcIn );
      nodes.add( p );
      return p;

   }
   xml_comment*  xml_document::comment_create( ) {
      xml_comment* p = comment_create( tlfm_ );
      return p;

   }

   xml_element* xml_document::element_create( t_lfm const& lfmcIn, const std::string& value_ ) {
      xml_element* p = xml_element::create( lfmcIn,  value_ );
      nodes.add( p );
      return p;

   }

   xml_element* xml_document::element_create( const std::string& value_ ) {
      xml_element* p = element_create( tlfm_, value_ );
      return p;
   }

   xml_text*  xml_document::text_create( t_lfm const& lfmcIn, const string& value_ ) {
      xml_text* p = xml_text::create( lfmcIn ,  value_ );
      nodes.add( p );
      return p;
   }

   xml_text*  xml_document::text_create( const string& value_ ) {
      return text_create( tlfm_, value_ );
   }

   xml_declaration* xml_document::declaration_create( t_lfm const& lfmcIn ) {
      xml_declaration* p = xml_declaration::create( lfmcIn ) ;
      nodes.add( p );
      return p;

   }
   xml_declaration* xml_document::declaration_create( ) {
      return declaration_create( tlfm_ );
   }


   bool xml_document::parse_begin( string  const& l ) {
      class add {
      public:
         vector8_t& _v;
         add( vector8_t& v ): _v( v ) {}
         void operator()( string const& s ) {
            _v.insert( _v.end(), s.begin(), s.end() );
         }
      };

      clear();
      // add space at end of file content,
      //  to prevent eof in buffer if parser is at end
      raw_buffer pp( l + " " );

      string w ;

      try {

         parse( pp );
      } catch( xml_exception& ex ) {
         w = ex.what();
         LOGT_ERROR( "ex: " + w );
      }

      return true;
   }

   void xml_document::clear() {
      xml_node::clear();
      nodes.clear_all();
   }

   bool xml_document::save( string const& filename ) const {

      bool pretty_print = true;
      std::vector<string> text;
      text = serialize_to_vector( "   ", pretty_print );
      bool overwrite = true;

      try {
         rlf_txtrw::t_write_text()( filename, text, overwrite );
      } catch( rlf_txtrw::bad_text_write& ex ) {
         string w =  ex.what();
         LOGT_ERROR( "ex: " + w );
         return false;
      }

      return true;
   }

   string xml_document::serialize( string indent,     bool pretty_print ) const {

      string s ;

      xml_printer pr( indent );

      if( !pretty_print ) {
         pr.pretty_print_off();
      }

      this->accept( &pr );
      s = _bom;
      s += pr.result();
      return move( s );
   }

   list<string> xml_document::serialize_to_list( string indent,     bool pretty_print ) const {
      string s = serialize( indent, pretty_print );
      return rlf_hstring::string_to_list( s, 0 );
   }

   vector<string> xml_document::serialize_to_vector( string indent,     bool pretty_print ) const {
      string s = serialize( indent, pretty_print );
      char trim_ch = 0;
      return rlf_hstring::string_to_vector( s,  trim_ch );
   }


   v_ret xml_document::accept( visitor_base* visitor ) const {
      v_ret ret = visitor->enter( *this );

      //string n;

      if( ret.recurse() ) {

         std::vector<xml_node*>  const& childs = getChilds();

         for( xml_node * n : childs ) {
            //            ns = n->value();
            //            LOGT_INFO( "typ:    " + typ );
            //            LOGT_INFO( "parent: " + nname );
            //            LOGT_INFO( "node:   " + n );
            //            LOGT_INFO( "ntype:  " + to_string( node->_type ) );
            //            LOGT_INFO( "path:   " +  static_cast<string>( node->lookuppath() ) );
            //            LOGT_INFO( "childs: " +  rlf_hstring::toString( childs.size() ) );
            ret = n->accept( visitor );

            if( ret.stop() ) {
               break;
            }
         }

         //         const xml_node* node = first_child();

         //         for( ; node != nullptr; node = node->next() ) {
         //            n = node->value();

         //            ret = node->accept( visitor );

         //            if( ret.stop() ) {
         //               break;
         //            }
         //         }
      }

      return visitor->exit( *this );
   }







} // end of namespace txml



// EOF
