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

//#define _CRT_SECURE_NO_WARNINGS 
//#define _SCL_SECURE_NO_WARNINGS

#include "xml_declaration.h"\
 
#include "xml_visitor.h"
#include "xml_utl.h"
#include "xml_exception.h"
#include "rawxml_position.h"
#include "alloccheck.h"




using alloccheck::t_alloc_line_file_method;

namespace txml {
   using std::string;
   using std::vector;

   void* xml_declaration::operator new( size_t size, t_alloc_line_file_method const& lfm ) {
      void* p = alloccheck::LocalAlloc( size, lfm );
      return p;
   }


   void xml_declaration::operator delete( void* p ) {
      xml_node* n = reinterpret_cast<xml_node*>( p );
      alloccheck::LocalDelete( n );
   }

   xml_node* xml_declaration::create( t_alloc_line_file_method const& lfmcIn ) {
      xml_declaration* p = new( lfmcIn ) xml_declaration();
      //p->setStandalone("yes");
      return p;
   }


   xml_node* xml_declaration::create() {
      return create( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ) );
   }

   void xml_declaration::getAttributes( string const& temp ) {

      /*
       example:
           <?xml declaration ?>
      */
      string txt = extract( temp, "<?xml", "?>" );
      boost::trim( txt );

      vector<string> v;
      boost::split( v, txt, boost::is_any_of( " \t\n" ) );


      vector<string>::const_iterator begin = v.begin();
      vector<string>::const_iterator end = v.end();

      while( begin != end ) {
         vector<string> a;
         boost::split( a, *begin, boost::is_any_of( "=" ) );

         if( a.size() == 2 ) {
            if( boost::iequals( a[0], attr_version ) ) {
               boost::trim_if( a[1], boost::is_any_of( "\"" ) );
               _version = a[1];
            }

            if( boost::iequals( a[0], attr_encoding ) ) {
               boost::trim_if( a[1], boost::is_any_of( "\"" ) );
               _encoding = a[1];
            }

            if( boost::iequals( a[0], attr_standalone ) ) {
               boost::trim_if( a[1], boost::is_any_of( "\"" ) );
               _standalone = a[1];
            }
         }

         ++begin;
      }

      return;
   }

   void xml_declaration::parse( rawxml_position& pos ) {
      pos.skip();

      if( !pos.starts_with( declaration_start ) ) { //"<?xml"
         throw xml_exception( t_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_parsing_declaration,
                             msg_parsing_declaration  + ": '" +
                             pos.next25() + "'" );
      }

      string temp = pos.next_until( declaration_end ); // "?>"
      pos += temp.size();
      getAttributes( temp );
      return ;

   }

   xml_declaration::xml_declaration( const xml_declaration& copy_ )
      : xml_node( xml_node::RL_XML_DECLARATION ), _version( copy_._version ), _encoding( copy_._encoding ), _standalone( copy_._standalone ) {
      copy_.copy( *this );
   }

   xml_declaration& xml_declaration::operator=( const xml_declaration& copy_ ) {
      clear();
      copy_.copy( *this );
      return *this;
   }

   void xml_declaration::print( int , string& str ) const {
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
   }

   void xml_declaration::copy( xml_declaration& target ) const {
      target.value( value() );
      target._version = _version;
      target._encoding = _encoding;
      target._standalone = _standalone;
   }


   bool xml_declaration::accept( xml_visitor* visitor ) const {
      return visitor->visit( *this );
   }


   xml_node* xml_declaration::clone() const {
      xml_declaration* pclone = new( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ) ) xml_declaration();
      copy( *pclone );
      return pclone;
   }


}

//EOF
