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
#include "xml_element.h"
#include "xml_text.h"
#include "xml_attribute.h"
#include "xml_visitor.h"

#include "xml_exception.h"
#include "rawxml_position.h"

#include "stringhelper.h"
#include "alloccheck.h"
#include "xml_locator.h"

#include <tLog_Category_default.h>


#include "tPointers.h"

using namespace std;


using txml::tPointers;


namespace txml {



   namespace {

      bool char_is_white_space( char c ) {
         //'\t'   (0x09)   horizontal tab (TAB)
         //'\n'   (0x0a)   newline (LF)
         //'\v'   (0x0b)   vertical tab (VT)
         //'\f'   (0x0c)   feed (FF)
         //'\r'   (0x0d)   carriage return (CR)
         int b = isspace( c ) ;

         if( b > 0 ) {
            return true;
         }

         return false;
      }


      bool isEmptyTextNode( string const& s )  {
         for( auto ch : s ) {
            if( !char_is_white_space( ch ) ) {
               return false;
            }
         }

         return true;
      }

   }


   void* xml_element::operator new( size_t size, t_lfm const& lfm ) {
      return alloccheck::checked_alloc( size, lfm );
   }
   void xml_element::operator delete( void* p ) {
      xml_node* n = reinterpret_cast<xml_node*>( p );
      alloccheck::checked_delete( n );
   }

   xml_element* xml_element::create( t_lfm const& lfmcIn, const string& value_ ) {
      xml_element* p = new( lfmcIn ) xml_element( value_ );
      xml_document::pointers.add( p );
      return p;

   }
   xml_element* xml_element::create( const string& value_ ) {
      return create( tlfm_, value_ );
   }



   xml_element::xml_element( const string& value_ )
      : xml_node( eType::ELEM ), _attributes() {
      _value = value_;
   }


   xml_element::~xml_element() {
      if( !usePointerContainer ) {
         clear();
      }
   }

   string xml_element::attribute( const string& name ) const {

      auto v = find_if( _attributes.begin(), _attributes.end(),
      [&name]( xml_attribute const & a ) {
         return a.name() == name;
      } );

      if( v != _attributes.end() ) {
         return v->value();
      }

      return string();
   }
   string xml_element::attribute_by_path( const path& n ) const {
      auto v = find_if( _attributes.begin(), _attributes.end(),
      [&]( xml_attribute const & a ) {
         return a._path == n;
      } );

      if( v != _attributes.end() ) {
         return v->value();
      }

      return string();
   }



   void xml_element::attribute( const string& name, const string& val ) {
      auto v = find_if( _attributes.begin(), _attributes.end(),
      [&name]( xml_attribute const & a ) {
         return a.name() == name;
      } );

      if( v == _attributes.end() ) {
         _attributes.push_back( xml_attribute( name, val ) );
      } else {
         v->value( val );
      }
   }


   v_ret xml_element::accept( visitor_base* visitor ) const {

      string n;
      // a visitor returns true, until something is found
      v_ret ret = visitor->enter( *this );

      if( ret.recurse() ) {
         const xml_node* node = first_child();

         for( ; node; node = node->next() ) {
            n = node->value();
            ret = node->accept( visitor );

            if( ret.stop() ) {
               break;
            }
         }
      }

      ret = visitor->exit( *this );
      return ret;
   }




   // text is first child
   const string xml_element::unencoded_text() const {
      xml_text const* childText = text();

      if( childText ) {
         string ct = childText->unencoded_value();
         return ct;
      }

      return string();
   }

   void xml_element::text( string const& s )  {
      xml_node* child = this->first_child();

      if( child ) {
         xml_text* childText = dynamic_cast<xml_text*>( child );

         if( childText ) {
            childText->value( s );
         }
      }

   }

//   string path_element::toValue()const {
//      xml_element const* n =  dynamic_cast<xml_element const*>( _node );
//      string val;

//      if( n != nullptr ) {
//         val =  n->unencoded_text();
//         string a = attr();

//         if( !a.empty() ) {
//            val = n->attribute( a );
//         }
//      }

//      return val;
//   }
   // <elem attr="mm" >abc</elem>  <-- normales element, chileds/.
   // <elem attr="mm" />  <-- closed element, no child, but siblings
   void xml_element::parse( raw_buffer& pos ) {


      string ndebug = "ADVTBeziehung";

      pos.skip();

      if( pos.is_at_end() ) {
         throw Xml_exception(
            eEx::parse, msg_parsing_element + ", unexpected end reached" );
      }

      // pos is at <, look for >
      auto vi = pos.find_next( string( ">" ) ); // ">"

      // get element name + attributes
      string elem_all = pos.next( vi + 1 ); // skip >

      // look for closed element, ends with />, has no text, no childs
      size_t s1 = elem_all.rfind( "/>" );

      bool isclosed_Element =  false;

      if( s1 + string( "/>" ).size() == elem_all.size() ) {
         isclosed_Element = true;
      }

      string inner;

      if( isclosed_Element ) {
         inner = rlf_hstring::trim( extract( elem_all, "<", "/>" ) );
      } else {
         inner = rlf_hstring::trim( extract( elem_all, "<", ">" ) );
      }

      if( inner.empty() ) {
         throw Xml_exception(
            eEx::parse, msg_parsing_element +  " at: '" + elem_all + "'" );
      }

      // look for start tag, shouldn't be here
      size_t si = inner.find( "<", 1 ); // "<"

      if( si != string::npos ) {
         throw Xml_exception(
            eEx::parse,
            msg_parsing_element +  " at: '" + elem_all + "'" );
      }

      //  read name and attributes
      _value = readName( inner );
      //LOGT_DEBUG( _value );

      string lp = _path;

      pos.advance( elem_all.size() );

      if( pos.is_at_end() && !isclosed_Element ) {
         throw Xml_exception(
            eEx::parse,
            msg_parsing_element +  " at: '" + pos.next25() + "'" );
      }

      if( _value == ndebug ){
         ndebug = "";
      }
      path_element ke( _value );
      ke.node( this );
      _path.add( ke );

      // look for previous child, for internal numbering
      xml_node* parent_ = parent();
      xml_node* prevChild = parent_->last_child_element( _value );

      if( prevChild != nullptr ) {
         path_element const& pe_prev = prevChild->lookuppath().last();
         path_element& pe_local =  lookuppath().last();
         string spe_prev = prevChild->lookuppath().last();
         string spe_local =  lookuppath().last();
         spe_prev = rlf_hstring::clip_at_char( spe_prev, path_element::element_count_left_bracket[0] );

         if( spe_prev == spe_local ) { // compare by element name
            int iprev = pe_prev.childcount();

            if( iprev < 0 ) {
               iprev = 0;
            }

            pe_local.childcount( pe_prev.childcount() + 1 );
         }
      }


      string attributes = inner;
      bool attr_checked = false;
      while( !pos.is_at_end() ) {
         size_t value_size = _value.size();


         // if element has no attributes, txt = "" or "/"
         if( value_size <= attributes.size() ) {
            // txt has attributes
            // get remainder of inner = inner - value
            attributes = rlf_hstring::trim( attributes.substr( value_size ) );
         }

         if( isclosed_Element &&  attributes.size() == 0 ) {
            return;
         }
         // get attributes, if not, go to next
         if( attributes.size() > 0 ) {  // we have an empty tag with /> and/or we have attributes

            // parse attributes, attributepart is empty after function call
            attributes = rlf_hstring::trim( parse_attributes( attributes ) );
            attr_checked = true;
            if( attributes.size() != 0 ) {
               throw Xml_exception(
                  eEx::parse, msg_parsing_element + ", attrbutepart ahs unknown parts: '" + inner + "'" );

            }

         } else { // goto next in tree
            pos.skip();

            // recursive loop
             while( !pos.is_at_end() ) {

               // if at text start
               string txt1 = pos.next( string( "<" ).size() );

               // if not at at '<' then is element text
               if( txt1 != string( "<" ) ) { // not at '<'
                  xml_text* textNode = xml_text::create( tlfm_ ) ;
                  string text1 = pos.next( pos.find_next( string( "<" ) ) );
                  pos.advance( text1.size() );
                  pos.advance( 1 ); // skip <
                  textNode->parseText( text1 );

                  if( isEmptyTextNode( textNode->value() ) ) {
                     delete textNode; // do_delete
                  } else {

                     link_end_child( textNode );
                     ke.value( textNode->value() );

                  }

                  if( !pos.is_at_end() ) {
                     pos.advance( -1 ); // set pos before textending "<"
                  }
               }

               // We hit a '</' == end tag start, if text in element
               txt1 = pos.next( string( "</" ).size() );

               if( txt1 != "</" ) {  // "</"
                  xml_node* node = create_in_elem( pos );

                  if( node == nullptr ) {
                     throw Xml_exception(
                        eEx::parse, msg_unknown_node + ": " + pos.next( 10 ) );
                  }

                  if( node ) {
                     node->_path = _path;

                     // recursive call pf parse
                     node->parse( pos );

                     link_end_child( node );
                  }
               }

               pos.skip();
               static string marker = "</";
               string next_ = pos.next( string( "</" ).size() );

               if( next_ == "</" ) {
                  break;
               }
            } // while 2

            // is endtag ?
            // </tag > and </tag> are both valid, check for '</name' not for '</name>'
            string nodeClosing = "</"; //  "</" ;
            nodeClosing += _value;

            if( pos.starts_with( nodeClosing ) ) {
               pos.advance( nodeClosing.length() );
               pos.skip();

               if( pos.next( string( ">" ).size() ) == string( ">" ) ) { // '>'
                  pos.advance( 1 );
                  return;
               }

               throw Xml_exception(
                  eEx::parse, msg_parsing_element + ", endtag: '" + nodeClosing + "'" );
            } else {
               throw Xml_exception(
                  eEx::parse, msg_parsing_element + ", endtag: '" + nodeClosing + "'" );
            }
         }
      } // while

      return;
   }

   std::vector<xml_attribute> const& xml_element::Attributes() const {
      return _attributes;
   }

   bool xml_element::isClosed()const {
      xml_node const* n0 = first_child();
      xml_node const* n1 = last_child();
      return ( n0 == n1 ) && ( n0 == nullptr );
   }
   xml_text const* xml_element::text() const {
      xml_node const* ch = first_child();

      if( ch == nullptr ) {
         return nullptr;
      }

      xml_text const* p1 = dynamic_cast<xml_text const*>( ch );
      return p1;
   }



   string xml_element::parse_attributes( string const& v ) {

      if( v.size() == 0 ) {
         return "";
      }

      raw_buffer pp( v );

      while( true ) {
         vector8_t::const_iterator i = pp.find_next( "=" );

         if( i == pp.end() ) {
            // no valid attr in pos
            break;
         }

         pp.skip();

         xml_attribute attrib;
         attrib._path = _path;
         attrib.parse( pp );

         _attributes.push_back( attrib );
      }

      // must be v.size(), end of raw vector with attributes
      std::ptrdiff_t running_position = pp.position();
      string s = pp.next( running_position );
      // must be "", no attributes left
      return s;
   }



}

//EOF
