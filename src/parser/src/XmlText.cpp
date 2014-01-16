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


#include <boost/assign.hpp>
#include <vector>

#include "XmlText.h"
#include "XmlVisitor.h"
#include "XmlDocument.h"

#include "rawxml_position.h"
#include "strings.h"

#include "XmlException.h"
#include "xml_utl.h"
#include "alloccheck.h"

#include "win32.h"



using namespace std;
using alloccheck::t_alloc_line_file_method;

namespace txml {



   class tEntity {
   public:
      tEntity( std::string s, char ch ): str( s ), chr( ch ) {}
      std::string    str;
      char         chr;
   };


   vector<tEntity> entities = boost::assign::list_of
                              ( tEntity( "&amp;",  '&' ) )
                              ( tEntity( "&lt;",   '<' ) )
                              ( tEntity( "&gt;",   '>' ) )
                              ( tEntity( "&quot;", '\"' ) )
                              ( tEntity( "&apos;", '\'' ) );


   char findEntity( string s ) {
      for( size_t i = 0; i < entities.size(); i++ ) {
         if( entities[i].str == s ) {
            return entities[i].chr;
         }
      }

      return 0; //tEntity( string(),' ' );
   }


   string replace_first( string const& in, string const& pattern, string const& replace ) {
      string s = in;
      size_t pos = strings::index( s, pattern, 0 );

      if( pos != std::string::npos ) {
         s.erase( pos, pattern.size() );
         s.insert( pos, replace );
      }

      return s;
   }


   // print, ascii out
   std::string decodeEntities( string const& s ) {
      string temp = s;

      for( size_t i = 0;  i < entities.size(); i++ ) {
         tEntity e = entities[i];
         size_t p = temp.find( e.str );

         while( p != string::npos ) {
            string ech;
            ech += e.chr;
            temp = replace_first( temp, e.str, ech );
            p = temp.find( e.str );
         }
      }

      return temp;
   }
   std::string encodeEntities( std::string const& s ) {
      std::string temp;
      string::const_iterator start = s.begin();
      string::const_iterator end = s.end();

      while( start != end ) {
         char ch = *start;
         size_t i = 0;

         for( ;  i < entities.size(); i++ ) {
            tEntity e = entities[i];

            if( e.chr == ch ) {
               temp += e.str;
               break;
            }
         }

         if( i == entities.size() ) {
            temp += ch;
         }

         ++start;
      }

      return temp;
   }



   //00000000 -- 0000007F:    0xxxxxxx
   //00000080 -- 000007FF:    110xxxxx 10xxxxxx
   //00000800 -- 0000FFFF:    1110xxxx 10xxxxxx 10xxxxxx
   //00010000 -- 001FFFFF:    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
   std::string utf32_to_utf8( unsigned long input ) {
      int length = 0;

      if( input < 0x80 ) {
         length = 1;
      } else if( input < 0x800 ) {
         length = 2;
      } else if( input < 0x10000 ) {
         length = 3;
      } else if( input < 0x200000 ) {
         length = 4;
      }

      const uint8_t BYTE_MASK = 0xBF;

      const uint8_t BYTE_MARK = 0x80;

      const uint8_t FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

      char buf[32];

      memset( buf, 0, 32 );

      char* p = buf + length;

      switch( length ) {
      case 4:
         --p;
         *p = ( char )( ( input | BYTE_MARK ) & BYTE_MASK );
         input >>= 6;
      case 3:
         --p;
         *p = ( char )( ( input | BYTE_MARK ) & BYTE_MASK );
         input >>= 6;
      case 2:
         --p;
         *p = ( char )( ( input | BYTE_MARK ) & BYTE_MASK );
         input >>= 6;
      case 1:
         --p;
         *p = ( char )( input | FIRST_BYTE_MARK[length] );
      }

      return string( buf );
   }



   // Bunch of unicode info at:
   //    http://www.unicode.org/faq/utf_bom.html
   // Including the basic of this table, which determines the #bytes in the
   // sequence from the lead byte. 1 placed for invalid sequences --
   // although the result will be junk, pass it through as much as possible.
   // Beware of the non-characters in UTF-8:
   //          ef bb bf (Microsoft "lead bytes")
   //          ef bf be
   //          ef bf bf


   /*
      UTF-8

      http://en.wikipedia.org/wiki/UTF-8

      Bits  Last code point   Byte 1   Byte 2   Byte 3   Byte 4   Byte 5   Byte 6
      7   U+007F   0xxxxxxx
      11    U+07FF   110xxxxx    10xxxxxx
      16    U+FFFF   1110xxxx    10xxxxxx    10xxxxxx
      21    U+1FFFFF    11110xxx    10xxxxxx    10xxxxxx    10xxxxxx
      26    U+3FFFFFF   111110xx    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx
      31    U+7FFFFFFF  1111110x    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx

      BOM
      http://www.opentag.com/xfaq_enc.htm
      The BOM is a Unicode special marker placed at the top of the file that indicate its encoding. The BOM is optional for UTF-8.
      First bytes   Encoding assumed
      EF BB BF   UTF-8
      FE FF   UTF-16 (big-endian)
      FF FE   UTF-16 (little-endian)
      00 00 FE FF   UTF-32 (big-endian)
      FF FE 00 00   UTF-32 (little-endian)
      None of the above   UTF-8

      http://www.cl.cam.ac.uk/~mgk25/unicode.html#utf-8


      0xC0 0x8A
      0xE0 0x80 0x8A
      0xF0 0x80 0x80 0x8A
      0xF8 0x80 0x80 0x80 0x8A
      0xFC 0x80 0x80 0x80 0x80 0x8A

      */

   const int utf8ByteTable[256] = {
      // 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x00
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x10
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x20
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x30
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x40
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x50
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x60
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x70  End of ASCII range
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x80 - 0x80 to 0xc1 invalid
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0x90
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xa0
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0xb0
      1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 0xc0 - 0xc2 to 0xdf 2 byte
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 0xd0
      3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, // 0xe0 - 0xe0 to 0xef 3 byte
      4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1  // 0xf0 - 0xf0 to 0xf4 4 byte, 0xf5 and higher invalid
   };


   string getUntilNextSemicolon( string::const_iterator pos, string::const_iterator end ) {
      string::const_iterator last = std::find( pos, end, ';' );

      if( last == end ) { // no end marker ';' found
         return string();
      }

      return string( pos, last + 1 );
   }

   string decode_utf( string const& p1,  Encoding encoding ) {
      // &abcabcabca;
      rawxml_position pp( vector8_t ( p1.begin() + 1, p1.end() - 1 ) ); // no & at start, no ; at end

      if( *pp == '#' ) {
         ++pp; // skip #
         size_t unicode = 0;

         if( *pp == 'x' ) {
            // Hexadecimal
            ++pp; // skip 'x'
            string s( pp.running(), pp.end() );
            unicode = strings::hex_to_size_t( s );
         } else {
            string s( pp.running(), pp.end() );
            unicode = strings::to_int( s );
         }

         if( encoding == ENCODING_UTF8 ) {
            return utf32_to_utf8( static_cast<unsigned long>( unicode ) );
         }

         return string() + ( char )unicode;
      }

      char ch = findEntity( p1 );

      if( ch > 0 ) {
         return string() + ch;
      }

      return p1; // couldn't decode
   }



   // Get a character, while interpreting entities
   // The length can be from 1 to 4 bytes
   std::string next_char( rawxml_position& pos, Encoding encoding ) {
      int length = 1;
      uint8_t ch1 = ( uint8_t )( *pos );

      if( encoding == ENCODING_UTF8 ) {
         length = utf8ByteTable[ ch1 ];
      }

      if( length == 1 ) {
         if( *pos == '&' ) {  // is entity

            string p = pos.next( MAX_ENTITY_SIZE );
            string p1 = getUntilNextSemicolon( p.begin(), p.end() );

            if( p1.size() < MIN_ENTITY_SIZE ) {
               pos += 1; // skip & and return
               return string();
            }

            pos += p1.size();
            return decode_utf( p1, encoding );
         }

         std::string value = string() + ( char ) * pos;
         ++pos;
         return value;
      }

      // length > 1, simple copy
      std::string v = pos.next( length );
      pos += length;
      return v;
   }


   string notPreserveWhiteSpace( string const& p ) {
      rawxml_position pos( vector8_t ( p.begin(), p.end() ) );
      string text;

      while( pos.running() < pos.end() ) {
         string p1 = pos.next( MAX_ENTITY_SIZE ); // MAX_ENTITY_SIZE = 8
         rawxml_position pp( vector8_t ( p1.begin(), p1.end() ) );
         string ch  = next_char( pp, XmlDocument::encoding() );
         text.append( ch );
         pos += pp.position();
      }

      return text;
   }

   string preserveWhiteSpace( string const& p ) {
      rawxml_position pos( vector8_t ( p.begin(), p.end() ) );

      string text;
      bool iswhitespace = false;
      // Remove leading white space:
      pos.skip();

      while( pos.running() < pos.end() ) {
         if( pos.is_white_space() ) {
            iswhitespace = true;
            ++pos;
            continue;
         }

         if( iswhitespace ) {
            text  += ' ';
            iswhitespace = false;
         }

         string p1 = pos.next( MAX_ENTITY_SIZE );
         rawxml_position pp( vector8_t ( p1.begin(), p1.end() ) );
         string arr = next_char( pp, XmlDocument::encoding() );
         text.append( arr );
         pos += pp.position();

      }

      return text;


   }


   string readText( string const& text ) {
      if( XmlDocument::isWhiteSpacePreserved() ) {
         return preserveWhiteSpace( text );
      }

      return notPreserveWhiteSpace( text );
   }

   void* XmlText::operator new( size_t size, t_alloc_line_file_method const& lfm ) {
      return LocalAlloc( size, lfm );
   }
   void XmlText::operator delete( void* p ) {
      alloccheck::LocalDelete( p );
   }

   XmlText* XmlText::create( t_alloc_line_file_method const& lfmcIn, const std::string& value_ ) {
      XmlText* p = new( lfmcIn ) XmlText( value_ );
      return p;
   }

   XmlText* XmlText::create( const std::string& value_ ) {
      return create( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ), value_ );
   }


   void XmlText::parseText( string const& p ) {
      string const& temp = readText( p );
      XmlNode::value( temp );
   }

   void XmlText::parse( rawxml_position& ) {
      throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                          enum_parse_text, msg_parse_text );
   }

   const std::string XmlText::value() const {
      string nv = XmlNode::value();
      return encodeEntities( nv );
   }
   const std::string XmlText::unencoded_value() const {
      string nv = XmlNode::value();
      return nv;
   }


   void XmlText::copy( XmlNode& target ) const {
      target.value( value() );
   }


   bool XmlText::accept( XmlVisitor* visitor ) const {
      return visitor->visit( *this );
   }

   XmlNode* XmlText::clone() const {
      XmlText* pclone = new( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ) ) XmlText( "" );
      copy( *pclone );
      return pclone;
   }

}

//EOF