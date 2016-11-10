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

#include "xml_visitor.h"
#include "xml_utl.h"
#include "xml_exception.h"
#include "rawxml_position.h"
#include "alloccheck.h"

#include "tPointers.h"

using rlf_tlfm ::t_lfm;

namespace txml {
   using std::string;
   using std::vector;

   void* xml_declaration::operator new( size_t size, t_lfm const& lfm ) {
      void* p = alloccheck::checked_alloc( size, lfm );
      return p;
   }


   void xml_declaration::operator delete( void* p ) {
      xml_declaration* n = reinterpret_cast<xml_declaration*>( p );
      alloccheck::checked_delete( n );
   }

   xml_declaration* xml_declaration::create( t_lfm const& lfm ) {
      xml_declaration* p = new( lfm ) xml_declaration();
      return p;
   }
   xml_declaration* xml_declaration::create( ) {
      return create( tlfm_ );
   }


   void xml_declaration::attributes( string const& temp ) {

      /*
       example:
           <?xml declaration ?>
      */

      vector<string> v;
      boost::split( v, temp, boost::is_any_of( " \t\n" ) );

      for( auto s : v ) {
         vector<string> a;
         boost::split( a, s, boost::is_any_of( "=" ) );

         if( a.size() != 2 ) {
            continue;
         }

         string const& s0 = a[0];
         string& s1 = a[1];

         if( boost::iequals( s0, attr_version ) ) {
            boost::trim_if( s1, boost::is_any_of( "\"" ) );
            _version = s1;
            continue;
         }

         if( boost::iequals( s0, attr_encoding ) ) {
            boost::trim_if( s1, boost::is_any_of( "\"" ) );
            _encoding = s1;
            continue;
         }

         if( boost::iequals( s0, attr_standalone ) ) {
            boost::trim_if( s1, boost::is_any_of( "\"" ) );
            _standalone = s1;
         }
      }

      return;
   }

   void xml_declaration::parse( raw_buffer& pos ) {
      string temp = pos.next_until( "?>" );
      pos.advance( temp.size() );
      temp = extract( temp, "<?xml", "?>" );
      attributes( temp );
      return ;

   }

   string xml_declaration::print( int ) const {
      string str;
      str  += "<?xml ";

      if( !_version.empty() ) {
         str += "version=\"";
         str += _version;
         str += "\" ";
      }

      if( !_encoding.empty() ) {
         str += "encoding=\"";
         str += _encoding;
         str += "\"";
      }

      if( !_standalone.empty() ) {
         str += "standalone=\"";
         str += _standalone;
         str += "\"";
      }

      str  += "?>";
      return str;
   }

   v_ret xml_declaration::accept( visitor_base* visitor ) const {
      return visitor->visit( *this );
   }


}

//EOF
