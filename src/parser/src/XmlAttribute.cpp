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

#include <string>
#include <sstream>
#include <iomanip>

#include "rawxml_position.h"

#include "XmlAttribute.h"
#include "XmlException.h"


using namespace std;

namespace txml {


   namespace {

      string EncodeString( const std::string& str ) {
         size_t  i = 0;
         string temp;

         while( i < str.size() ) {
            uint8_t c = ( uint8_t ) str[i];

            if( c == '&'
                  && i < ( str.length() - 2 )
                  && str[i + 1] == '#'
                  && str[i + 2] == 'x' ) {
               // Hexadecimal character reference.
               // Pass through unchanged.
               // &#xA9;   -- copyright symbol, for example.
               while( i < str.length() - 1 ) {
                  temp.append( str.c_str() + i, 1 );
                  ++i;

                  if( str[i] == ';' ) {
                     break;
                  }
               }

               continue;
            }

            if( c < 32 ) {
               std::ostringstream o;
               o << string( "&#x" ) << std::setfill( '0' ) << std::setw( 2 ) << std::hex << c;
               temp += o.str();
               ++i;
               continue;
            }

            temp += ( char ) c;
            ++i;
         }

         return temp;
      }

      class selectInQuotes {
         rawxml_position& _pos;
         //XmlAttribute& _attr;
         string _value;

      public:
         selectInQuotes( rawxml_position& raw_pos ) : _pos( raw_pos ), _value() {}

         void operator()( string const& quote ) {
            size_t pos = _pos.position();

            if( quote.empty() ) {
               return;
            }

            if( pos >= _pos.size() ) {
               return;
            }

            if( *_pos != quote[0] ) {
               return;
            }

            ++_pos; // skip quote
            string p = _pos.next( _pos.find( quote ) );
            _pos += p.size();
            ++_pos; // skip quote
            _value = readText( p );
         }
         string const& value()const {
            return _value;
         }
      };
   }




   void XmlAttribute::parseAttr( rawxml_position& pos ) {

      try {
         vector8_t::const_iterator i = pos.find( '=' );

         // Read the name, the '=' and the value.
         string temp = pos.next( i );
         _name = readName( temp );
         pos += _name.size();

         pos.skip();
         ++pos;  // skip '='
         pos.skip();

         // value is quoted
         vector<string> quotes = boost::assign::list_of( SINGLE_QUOTE )( DOUBLE_QUOTE );
         string val  =  for_each( quotes.begin(), quotes.end(), selectInQuotes( pos ) ).value();
         _value = val;
         keyentry& pe = keys.last();
         pe.attr( _name );
         pe.Value( val );



      } catch( XmlException& ex ) {
         throw ex;
      }

      return;
   }



   void XmlAttribute::print( std::string& str ) const {

      std::string n = EncodeString( _name );
      std::string v = EncodeString( _value );

      if( _value.find( '\"' ) == std::string::npos ) {
         ( str ) += n;
         ( str ) += "=\"";
         ( str ) += v;
         ( str ) += "\"";
      } else {
         ( str ) += n;
         ( str ) += "='";
         ( str ) += v;
         ( str ) += "'";
      }
   }



}

//EOF

