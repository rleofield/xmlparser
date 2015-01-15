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

#include <string>
#include <sstream>
#include <iomanip>


#include "stringhelper.h"

using namespace std;


namespace txml {




   /*
   U+03A3   Decimal  &#931;   Σ
   U+03A3   Decimal  &#0931; Σ
   U+03A3   Hexadecimal    &#x3A3;

   http://en.wikipedia.org/wiki/Numeric_character_reference
   Character U+0026 (ampersand), followed by character U+0023 (number sign), followed by one of the following choices:

   one or more decimal digits zero (U+0030) through nine (U+0039); or
   character U+0078 ("x") followed by one or more hexadecimal digits,
   which are zero (U+0030) through nine (U+0039), Latin capital letter A (U+0041) through F (U+0046), and Latin small letter a (U+0061) through f (U+0066);

   all followed by character U+003B (semicolon).

   A "predefined entity reference" is a reference to one of the special characters denoted by:
   name  value    character   code (dec)  meaning
   quot  &#34;    "  x22 (34)    (double) quotation mark
   amp   &#38;    &  x26 (38)    ampersand
   apos  &#39;    '  x27 (39)    apostrophe (= apostrophe-quote)
   lt    &#60;    <  x3C (60)    less-than sign
   gt    &#62;    >  x3E (62)    greater-than sign
   [edit]

   http://www.cl.cam.ac.uk/~mgk25/unicode.html#utf-8

   */


   string extract( string const& s, string const& start, string const& end ) {
      if( boost::starts_with( s, start )
            && boost::ends_with( s, end ) ) {
         string txt( s.begin() + start.size(), s.end() - end.size() );
         boost::trim( txt );
         return txt;
      }

      return string();
   }

   namespace {
      bool IsAlphaNum( uint8_t ch ) {
         if( ch < 127 ) {
            int b = ::isalnum( ch );

            if( b ) {
               return true;
            }

            return false;
         } else {
            return true;   // fallback to EN code, unicode is too large for checking
         }
      }
      bool IsAlpha( uint8_t ch ) {
         if( ch < 127 ) {
            int b = ::isalpha( ch );

            if( b ) {
               return true;
            }

            return false;
         } else {
            return true;   // fallback to EN code, unicode is too large for checking
         }
      }

   }

   string readName( string const& temp ) {
      if( temp.size() == 0 ) {
         return string();
      }

      string name;
      // names start with letters or underscores.
      // after that, they can be letters, underscores, numbers,
      //   hyphens, or colons. (colons are valid ony for namespaces)

      auto begin = temp.begin();

      // check first char
      if( IsAlpha( static_cast< uint8_t >( *begin ) ) || *begin == '_' ) {
         name += *begin;
         begin++;
         // first char is letter or underscores.
         // check next

         auto end   = temp.end();

         while( begin != end
                && (
                   IsAlphaNum( static_cast< uint8_t >(  *begin ) )
                   || *begin == '_'
                   || *begin == '-'
                   || *begin == '.'
                   || *begin == ':'
                )
              ) {
            name += *begin;
            ++begin;
         }
      }

      return move( name );
   }





} // end of namespace txml






// EOF
