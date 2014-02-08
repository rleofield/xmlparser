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
#include <algorithm>

#include "xml_utl.h"

#include "rawxml_position.h"
#include "XmlException.h"
#include "stringhelper.h"


using namespace std;


namespace txml {



   void rawxml_position::next100()const {
      next100( int2type<useNext100>() );
   }


   void rawxml_position::next100( int2type<false> )const {
      _next = "";
   }
   void rawxml_position::next100( int2type<true> )const {
      vector8_t::const_iterator istart = _running;
      vector8_t::const_iterator iend = _running;

      while( iend < _rawxml.end() && ( iend - istart ) < 100 ) {
         iend++;
      }

      string s( istart, iend );
      _next.assign( istart, iend );
   }
   string rawxml_position::next25()const {
      return next25( int2type<useNext100>() );
   }

   string rawxml_position::next25( int2type<true> )const {
      vector8_t::const_iterator istart = _running;
      vector8_t::const_iterator iend = _running;

      while( iend < _rawxml.end() && ( iend - istart ) < 25 ) {
         iend++;
      }

      return string().assign( istart, iend );
   }
   string rawxml_position::next25( int2type<false> )const {
      return string();
   }


   rawxml_position::rawxml_position( vector8_t const& v ) :
      _rawxml( v ),
      _running( _rawxml.begin() ),
      _next()
   {}

   rawxml_position::rawxml_position( std::string const& v ) :
      _rawxml( v.begin(), v.end() ),
      _running( _rawxml.begin() ),
      _next()
   {}


   bool rawxml_position::starts_with( std::string const& s ) {
      std::string temp = next( s.size() );

      if( temp == s ) {
         return true;
      }

      return false;
   }
   bool rawxml_position::operator<( rawxml_position const& p )const {
      return  _running < p._running;
   }

   bool rawxml_position::is_end()const {
      next100();
      return _running == _rawxml.end();
   }
   vector8_t::const_iterator rawxml_position::end()const {
      return _rawxml.end();
   }
   std::ptrdiff_t rawxml_position::running_position()const {
      return _running -  _rawxml.begin();
   }

   string rawxml_position::next( ptrdiff_t n )const {
      size_t temp = n;
      ptrdiff_t diff = _rawxml.end() - _running;

      if( static_cast<ptrdiff_t>( temp ) > diff ) {
         temp = _rawxml.end() - _running;
      }

      if( temp == 0 ) {
         return string();
      }

      return std::string( _running,  _running + temp );
   }

   std::string rawxml_position::next( vector8_t::const_iterator it ) const {
      if( it < _running ) {
         int64_t pos = it - _running;
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_iterator_underflow,
                             "Position: nextX(it), iterator underflow: " + rlf_hstring::toString( static_cast<size_t>( pos ) ) );
      }

      if( it > _rawxml.end() ) {
         return std::string();
      }

      if( it > _rawxml.end() ) {
         int64_t pos1 = it - _running;
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_iterator_underflow,
                             "Position: nextX(it), iterator overflow: " + rlf_hstring::toString( static_cast<size_t>( pos1 ) ) );
      }

      return std::string( _running,  it );
   }

   std::string rawxml_position::next_until( std::string const& s )const {
      vector8_t::const_iterator vi = find( s );

      if( vi == end() ) {
         return std::string();
      }

      return next( vi + s.size() );
   }

   rawxml_position& rawxml_position::operator++() {
      ++_running;
      next100();
      return *this;
   }

   void rawxml_position::operator+=( int offset ) {
      if( _rawxml.end() - _running < offset ) {
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_operator_plus_at_or_after_end, "Position: operator += at or after end" );
      }

      if( _running >= _rawxml.end() ) {
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_operator_plus_at_or_after_end, "Position: operator += at or after end" );
      }

      _running += offset;
      next100();

   }
   void rawxml_position::operator-=( int i )const {
      if( i < _running - _rawxml.begin() ) {
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_operator_minus_at_or_after_end, "Position: operator -= at or after end" );
      }

      _running -= i;
      next100();

   }
   size_t rawxml_position::position()const {
      return _running - _rawxml.begin();
   }
   vector8_t::const_iterator rawxml_position::running()const {
      return _running;
   }

   rawxml_position::operator int8_t const* ()const {
      next100();
      return ( int8_t const* )( &*_running );
   }
   size_t rawxml_position::size()const {
      return _rawxml.size();
   }
   vector8_t::const_iterator rawxml_position::find( int8_t ch )const {
      vector8_t::const_iterator i = std::find( _running, _rawxml.end(), ch );
      return i ;
   }
   vector8_t::const_iterator rawxml_position::find( string const& s )const {
      vector8_t::const_iterator i = search( _running, _rawxml.end(), s.begin(), s.end() );
      return i;
   }




   bool rawxml_position::is_white_space()const {
      if( _running == _rawxml.end() ) {
         return false;
      }

      if( isspace( ( int8_t )( *_running ) ) > 0 ) {
         return true;
      }

      return false;
   }

   void rawxml_position::skip()const {
      if( _running == _rawxml.end() ) {
         return;
      }

      while(
         ( _running != _rawxml.end() )
         && ( ( *_running == ' ' )
              || ( *_running == '\t' )
              || ( *_running == '\n'
                   || ( *_running == '\r' ) )
            ) ) {
         ++_running;
      }

      if( _running >= _rawxml.end() ) {
         _next.clear();
         return;
      }

      next100();
   }
   size_t rawxml_position::remainder()const {
      return _rawxml.end() - _running ;

   }


}


// EOF
