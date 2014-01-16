
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





#include "XmlComment.h"

#include "xml_utl.h"
#include "XmlVisitor.h"
#include "XmlException.h"
#include "rawxml_position.h"
#include "alloccheck.h"


#include "win32.h"


namespace txml {

   using std::string;


   void* XmlComment::operator new( size_t size, t_alloc_line_file_method const& lfm ) {
      return alloccheck::LocalAlloc( size, lfm );
   }
   void XmlComment::operator delete( void* p ) {
      XmlNode* n = reinterpret_cast<XmlNode*>( p );
      alloccheck::LocalDelete( n );
   }

   XmlNode* XmlComment::create( t_alloc_line_file_method const& lfmcIn ) {
      XmlComment* p = new( lfmcIn ) XmlComment();
      return p;

   }
   XmlNode* XmlComment::create( ) {
      return create( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ) );
   }

   void XmlComment::parse( rawxml_position& pos ) {
      if( !pos.starts_with( comment_start ) ) { // "<!--"
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_parsing_comment,
                             msg_parsing_comment + ": '" +
                             pos.next25() + "'" );
      }

      string temp = pos.next_until( comment_end ); //"-->"
      /*
           example:
               <!-- declarations for <head> & <body> -->
         */
      string comment = extract( temp, comment_start, comment_end );
      pos += temp.size();
      value( comment );
      return;
   }

   XmlComment::XmlComment( const XmlComment& copy ) : XmlNode( XmlNode::RL_XML_COMMENT ) {
      value( copy.value() );
   }


   XmlComment& XmlComment::operator=( const XmlComment& from ) {
      clear();
      value( from.value() );
      return *this;
   }

   bool XmlComment::accept( XmlVisitor* visitor ) const {
      return visitor->visit( *this );
   }


   XmlNode* XmlComment::clone() const {
      return new( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ) ) XmlComment( *this );
   }




}

//EOF
