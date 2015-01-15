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
#include <algorithm>

#include "xml_utl.h"

#include "rawxml_position.h"
#include "xml_exception.h"
#include "stringhelper.h"


using std::string;

namespace txml {



   void raw_buffer::next100()const {
      next100( int2type<useNext100>() );
   }


   void raw_buffer::next100( int2type<false> )const {
      _next = "";
   }
   void raw_buffer::next100( int2type<true> )const {
      _next = next_n( 100 );
   }
   string raw_buffer::next25()const {
      return next25( int2type<useNext100>() );
   }

   string raw_buffer::next25( int2type<true> )const {
      return next_n( 25 );
   }
   string raw_buffer::next_n( size_t n )const {
      auto istart = _position;
      auto iend = _position;

      while( iend < _rawxml.end() && ( iend - istart ) < static_cast<ptrdiff_t>( n ) ) {
         iend++;
      }

      return string().assign( istart, iend );
   }


   raw_buffer::raw_buffer( vector8_t const& v ) :
      _rawxml( v ),
      _position( _rawxml.begin() ),
      _next() {
   }

   raw_buffer::raw_buffer( string const& v ) :
      _rawxml( v.begin(), v.end() ),
      _position( _rawxml.begin() ),
      _next() {
   }


   bool raw_buffer::starts_with( string const& s ) {
      string temp = next( s.size() );

      if( temp == s ) {
         return true;
      }

      return false;
   }
   bool raw_buffer::operator<( raw_buffer const& p )const {
      return  _position < p._position;
   }

   bool raw_buffer::is_at_end()const {
      return _position == end();
   }
   vector8_t::const_iterator raw_buffer::end()const {
      return _rawxml.end();
   }

   string raw_buffer::next( ptrdiff_t n )const {
      ptrdiff_t temp = n;
      ptrdiff_t diff = _rawxml.end() - _position;

      if( temp > diff ) {
         temp = diff;
      }

      return string( _position,  _position + temp );
   }


   string raw_buffer::next( vector8_t::const_iterator it ) const {
      if( it < _position ) {
         vector8_t::difference_type pos = it - _position;
         throw Xml_exception( eEx::buffer, msg_iterator_underflow + rlf_hstring::toString( pos ) );
      }

      // check against end() = one char after end of string
      if( it > _rawxml.end() ) {
         return string();
      }

      string ret = string( _position,  it );
      return ret;
   }

   string raw_buffer::next_until( string const& s )const {
      auto vi = find_next( s );

      if( vi == end() ) {
         return string();
      }

      return next( vi + s.size() );
   }

//   raw_buffer raw_buffer::operator++( int ) {
//      _position++;

//      if( _position > _rawxml.end() ) {
//         throw Xml_exception(
//            eEx::buffer, msg_operator_plus_at_or_after_end );
//      }

//      next100();
//      return *this;
//   }

   void raw_buffer::advance( ptrdiff_t offset )const {
      auto diff = remainder();

      if( diff < offset ) {
         throw Xml_exception(
            eEx::buffer, msg_operator_plus_at_or_after_end );
      }

      if( _position > _rawxml.end() ) {
         throw Xml_exception(
            eEx::buffer, msg_operator_plus_at_or_after_end );
      }

      _position += offset;
      next100();

   }
   vector8_t::difference_type raw_buffer::position()const {
      return _position - _rawxml.begin();
   }



   vector8_t::const_iterator raw_buffer::running()const {
      return _position;
   }

   size_t raw_buffer::size()const {
      return _rawxml.size();
   }
   vector8_t::const_iterator raw_buffer::find_next( string const& s )const {
      auto i = search( _position, _rawxml.end(), s.begin(), s.end() );
      return i;
   }





   void raw_buffer::skip()const {
      if( _position == _rawxml.end() ) {
         return;
      }

      while(
         ( _position != _rawxml.end() )
         && ( ( *_position == ' ' )
              || ( *_position == '\t' )
              || ( *_position == '\n'
                   || ( *_position == '\r' ) )
            ) ) {
         ++_position;
      }

      if( _position >= _rawxml.end() ) {
         _next.clear();
         return;
      }

      next100();
   }
   vector8_t::difference_type raw_buffer::remainder()const {
      return _rawxml.end() - _position ;

   }

   uint8_t raw_buffer::value()const {
      return *_position;
   }

}


// EOF
