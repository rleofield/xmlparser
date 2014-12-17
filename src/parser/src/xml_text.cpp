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


#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

#include "xml_text.h"
#include "xml_visitor.h"
#include "xml_document.h"

#include "rawxml_position.h"
#include "stringhelper.h"

#include "xml_exception.h"
#include "xml_interface.h"
#include "xml_utl.h"
#include "alloccheck.h"




using namespace std;


namespace txml {


namespace{
   class tEntity {
   public:
      tEntity( string s, char ch ): str( s ), chr( ch ) {}
      string    str;
      char         chr;
   };


   vector<tEntity> entities = boost::assign::list_of
                              ( tEntity( "&amp;",  '&' ) )
                              ( tEntity( "&lt;",   '<' ) )
                              ( tEntity( "&gt;",   '>' ) )
                              ( tEntity( "&quot;", '\"' ) )
                              ( tEntity( "&apos;", '\'' ) );


   char findEntity( string const& s_ ) {
      for( auto const& e: entities ){
         if( e.str == s_ ) {
            return e.chr;
         }
      }

      return 0;
   }
   string findEntity( char ch ) {
      for( auto const& e: entities ){
         if( e.chr == ch ) {
            return e.str;
         }
      }

      return string() + ch;
   }


   string replace_first( string const& in, string const& pattern, string const& replace ) {
      string s = in;
      size_t pos = rlf_hstring::index( s, pattern, 0 );

      if( pos != string::npos ) {
         s.erase( pos, pattern.size() );
         s.insert( pos, replace );
      }

      return s;
   }




   //00000000 -- 0000007F:    0xxxxxxx
   //00000080 -- 000007FF:    110xxxxx 10xxxxxx
   //00000800 -- 0000FFFF:    1110xxxx 10xxxxxx 10xxxxxx
   //00010000 -- 001FFFFF:    11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
   string utf32_to_utf8( uint32_t input ) {
      uint32_t length = 0;

      if( input < 0x80 ) {
         length = 1;
      } else {
         if( input < 0x800 ) {
            length = 2;
         } else {
            if( input < 0x10000 ) {
               length = 3;
            } else {
               if( input < 0x200000 ) {
                  length = 4;
               }
            }
         }
      }

      const uint8_t BYTE_MASK = 0xBF;

      const uint8_t BYTE_MARK = 0x80;

      const uint8_t FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

      vector<char> buf( 32 );

      char* p = &buf[0] + length;

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

      return &buf[0];
   }


   // converts a hex string to size_t
   size_t hex_to_size_t( string const& s )  {
      size_t x;
      std::istringstream insx( s );
      insx >> std::hex;
      insx >> x;
      return x;
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
      // example for an entity
      // &abcabcabca;
      raw_buffer pp( vector8_t ( p1.begin() + 1, p1.end() - 1 ) ); // no & at start, no ; at end

      if( pp.value() == '#' ) {
         ++pp; // skip #
         uint32_t unicode = 0;

         if( pp.value() == 'x' ) {
            // Hexadecimal
            ++pp; // skip 'x'
            string s = pp.next(pp.remainder());
            unicode = hex_to_size_t( s );
         } else {
            string s = pp.next(pp.remainder());
            unicode = boost::lexical_cast<uint32_t>( s );
         }

         if( encoding == Encoding::UTF8 ) {
            return utf32_to_utf8( static_cast<uint32_t>( unicode ) );
         }

         // not utf8, use simple char
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
   string next_char( raw_buffer& pos, Encoding encoding ) {
      int length = 1;
      uint8_t index = 0;

      if( encoding == Encoding::UTF8 ) {
         index = pos.value();
         length = utf8ByteTable[ index ];
      }

      if( length == 1 ) {
         if( pos.value() == '&' ) {  // is entity
            // MAX_ENTITY_SIZE = 8
            string p = pos.next( MAX_ENTITY_SIZE );
            string p1 = getUntilNextSemicolon( p.begin(), p.end() );

            if( p1.size() < MIN_ENTITY_SIZE ) {
               pos += 1; // skip & and return
               return string();
            }

            pos += p1.size();
            return decode_utf( p1, encoding );
         }

         string value( 1, ( char ) pos.value() ) ;
         ++pos;
         return value;
      }

      // length > 1, simple copy
      string v = pos.next( length );
      pos += length;
      return v;
   }



   string no_preserve_white_space( string const& s ) {
      raw_buffer buffer( s );
      string text;

      while( buffer.running() < buffer.end() ) {
         // MAX_ENTITY_SIZE = 8
         raw_buffer next( buffer.next( MAX_ENTITY_SIZE ) );
         string ch  = next_char( next, xml_document::encoding() );
         text.append( ch );
         buffer += next.position();
      }

      return text;
   }

   string preserve_white_space( string const& p ) {
      raw_buffer buffer( p );

      string text;

      while( buffer.running() < buffer.end() ) {
         if( buffer.is_white_space() ) {
            text += buffer.value();
            ++buffer;
            continue;
         }


         // entity or utf8
         raw_buffer next( buffer.next( MAX_ENTITY_SIZE ) );
         // maybe is utf8
         string nextchar = next_char( next, xml_document::encoding() );
         text.append( nextchar );
         buffer += next.position();

      }

      return text;
   }


} // end anon ns


// print, ascii out
string decodeEntities( string const& s ) {
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


string encodeEntities( string const& s ) {
   string temp;
   for( auto ch : s ){
      temp += findEntity( ch );
   }
   return temp;
}


   string readText( string const& text ) {
   if( xml_document::preserve_white_space() ) {
      return preserve_white_space( text );
   }

   return no_preserve_white_space( text );
}

   void* xml_text::operator new( size_t size, t_lfm const& lfm ) {
      return alloccheck::checked_alloc( size, lfm );
   }
   void xml_text::operator delete( void* p ) {
      alloccheck::checked_delete( p );
   }

   xml_text* xml_text::create( t_lfm const& lfmcIn, const string& value_ ) {
      xml_text* p = new( lfmcIn ) xml_text( value_ );
      xml_document::pointers.add( p );
      return p;
   }

   xml_text* xml_text::create( const string& value_ ) {
      return create( tlfm_, value_ );
   }


   void xml_text::parseText( string const& p ) {
      string const& temp = readText( p );
      xml_node::value( temp );
   }

   void xml_text::parse( raw_buffer& ) {
      throw Xml_exception( eEx::parse, msg_parse_text );
   }

   const string xml_text::value() const {
      string nv = xml_node::value();
      return encodeEntities( nv );
   }
   const string xml_text::unencoded_value() const {
      string nv = xml_node::value();
      return nv;
   }




   bool xml_text::accept( xml_visitor* visitor ) const {
      return visitor->visit( *this );
   }


}

//EOF
