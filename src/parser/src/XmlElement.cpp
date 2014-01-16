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
#include <boost/algorithm/string.hpp>


#include "xml_utl.h"
#include "XmlElement.h"
#include "XmlText.h"
#include "XmlAttribute.h"
#include "XmlVisitor.h"

#include "XmlException.h"
#include "rawxml_position.h"

#include "strings.h"
#include "alloccheck.h"
#include "XmlLocator.h"

using namespace std;



using alloccheck::t_alloc_line_file_method;


namespace txml {



   namespace {

      bool char_is_white_space( char c ) {
         //'\t'   (0x09)   horizontal tab (TAB)
         //'\n'   (0x0a)   newline (LF)
         //'\v'   (0x0b)   vertical tab (VT)
         //'\f'   (0x0c)   feed (FF)
         //'\r'   (0x0d)   carriage return (CR)
         int b = isspace( ( uint8_t ) c ) ;

         if( b > 0 ) {
            return true;
         }

         return false;
      }


      bool isEmptyTextNode( string const& s )  {
         for( unsigned i = 0; i < s.size(); i++ )
            if( !char_is_white_space( s[i] ) ) {
               return false;
            }

         return true;
      }

   }


   void* XmlElement::operator new( size_t size, t_alloc_line_file_method const& lfm ) {
      void*  p;

      p = alloccheck::LocalAlloc( size, lfm );
      return p;
   }
   void XmlElement::operator delete( void* p ) {
      XmlNode* n = reinterpret_cast<XmlNode*>( p );
      alloccheck::LocalDelete( n );
   }

   XmlElement* XmlElement::create( t_alloc_line_file_method const& lfmcIn, const std::string& value_ ) {
      XmlElement* p ;
      p = new( lfmcIn ) XmlElement( value_ );
      return p;

   }
   XmlElement* XmlElement::create( const std::string& value_ ) {
      return create( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ), value_ );
   }



   XmlElement::XmlElement( const std::string& _value )
      : XmlNode( XmlNode::RL_XML_ELEMENT ), _attributes(), rawattributes() {
      _node_value = _value;
   }


   XmlElement::~XmlElement() {
      clear();
   }


   class namecompare {
      string _name;
   public:
      namecompare( string const& name ): _name( name ) {}
      bool operator()( XmlAttribute const& a )const {
         if( a.name() == _name ) {
            return true;
         }

         return false;
      }
   };

   class keycompare {
      string _key;
   public:
      keycompare( string const& name ): _key( name ) {}
      bool operator()( XmlAttribute const& a )const {
         if( a.keys.to_string() == _key ) {
            return true;
         }

         return false;
      }
   };


   std::string XmlElement::attribute( const std::string& name ) const {
      vector<XmlAttribute>::const_iterator v = find_if( _attributes.begin(), _attributes.end(), namecompare( name ) );

      if( v != _attributes.end() ) {
         return v->value();
      }

      return string();
   }
   std::string XmlElement::attributeByKey( const std::string& name ) const {
      vector<XmlAttribute>::const_iterator v = find_if( _attributes.begin(), _attributes.end(), keycompare( name ) );

      if( v != _attributes.end() ) {
         return v->value();
      }

      return string();
   }
   vector<XmlAttribute>::const_iterator XmlElement::AttributeObj( const std::string& name ) const {
      vector<XmlAttribute>::const_iterator v = find_if( _attributes.begin(), _attributes.end(), namecompare( name ) );
      return v;
   }


   void XmlElement::setAttribute( const std::string& name, const std::string& val ) {
      vector<XmlAttribute>::iterator v = find_if( _attributes.begin(), _attributes.end(), namecompare( name ) );

      if( v == _attributes.end() ) {
         XmlAttribute attrib = XmlAttribute();
         attrib.name( name );
         attrib.value( val );
         _attributes.push_back( attrib );
      } else {
         v->value( val );
      }
   }

   void XmlElement::copy( XmlElement& target ) const {
      target.value( value() );
      target._attributes = _attributes;

      for( XmlNode const* node = firstChild(); node != nullptr; node = node->_next ) {
         target.linkEndChild( node->clone() );
      }
   }

   bool XmlElement::accept( XmlVisitor* visitor ) const {

      std::string n;
      // a visitor returns true, until something is found
      bool notAccepted = visitor->visitEnter( *this );

      if( notAccepted == true ) {
         const XmlNode* node = firstChild();

         for( ; node; node = node->next() ) {
            n = node->value();
            bool notAccepted1 = node->accept( visitor );

            if( notAccepted1 == false ) {
               break;
            }
         }
      }

      bool acceptedEnd = visitor->visitExit( *this );
      return acceptedEnd;
   }


   XmlNode* XmlElement::clone() const {
      XmlElement* pClone = new( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ) ) XmlElement( value() );

      copy( *pClone );
      return pClone;
   }


   const string XmlElement::getText() const {
      const XmlNode* child = this->firstChild();

      if( child ) {
         const XmlText* childText = dynamic_cast<XmlText const*>( child );

         if( childText ) {
            string ct = childText->unencoded_value();
            return ct;
         }
      }

      return string();
   }

   void XmlElement::setText( string const& s )  {
      XmlNode* child = this->firstChild();

      if( child ) {
         XmlNode* childText = dynamic_cast<XmlText*>( child );

         if( childText ) {
            childText->value( s );
         }
      }

   }

   string keyentry::toValue()const {
      XmlElement const* n =  dynamic_cast<XmlElement const*>( _node );
      string val;

      if( n != nullptr ) {
         val =  n->getText();
         string a = Attr();

         if( !a.empty() ) {
            val = n->attribute( a );
         }
      }

      return val;
   }

   void XmlElement::parse( rawxml_position& pos ) {
      pos.skip();

      if( pos.is_end() ) {
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_parsing_element, msg_parsing_element + ", unexpected end reached" );
      }

      string temp;
      vector8_t::const_iterator vi = pos.find( string( ">" ) ); // ">"
      temp = pos.next( vi + 1 ); // skip >

      string txt = extract( temp, "<", "/>" ); // "<" and "/>"
      bool isclosed_Element = true;

      if( txt.size() == 0 ) {
         txt = extract( temp, "<", ">" );
         isclosed_Element = false;
      }

      if( txt.empty() ) {
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_failed_to_read_element_name, msg_failed_to_read_element_name +  " at: '" + temp + "'" );
      }

      // look for start tag, is not here = ok
      size_t si = txt.find( "<", 1 ); // "<"

      if( si != string::npos ) {
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_failed_to_read_element_closing_tag,
                             msg_failed_to_read_element_closing_tag +  " at: '" + temp + "'" );
      }

      _node_value = readName( txt );

      string lp = _lookuppath.to_string();

      pos += temp.size();

      if( pos.is_end() && !isclosed_Element ) {
         throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                             enum_failed_to_read_element_name,
                             msg_failed_to_read_element_name +  " at: '" + pos.next25() + "'" );
      }

      _lookuppath.addEmpty();
      keyentry& ke = _lookuppath.last();
      ke.Element( _node_value );
      ke.node( this );

      // look for previous child, for internal numbering
      XmlNode* parent_ = parent();
      XmlNode* prevChild = parent_->lastChildElement( _node_value );

      if( prevChild != nullptr ) {
         keyentry const& pe_prev = prevChild->lookuppath().last();
         keyentry& pe_local =  lookuppath().last();

         if( pe_prev == pe_local ) { // compare by element
            int iprev = pe_prev.childcount();

            if( iprev < 0 ) {
               iprev = 0;
            }

            pe_local.childcount( pe_prev.childcount() + 1 );
         }
      }

      while( !pos.is_end() ) {
         size_t s = _node_value.size();

         // if element has no attributes, txt = "" or "/"
         if( s < txt.size() ) {
            // txt is "/" or attributes
            txt = string( txt.begin() + _node_value.size() , txt.end() );
         }

         txt = strings::trim( txt );
         string::const_iterator begin = txt.begin();
         string::const_iterator end   = txt.end();

         if( !txt.empty() ) { // we have an empty tag and/or we have attributes
            if( txt[0] == '/' ) {
               // is empty tag
               ++begin;

               if( begin != end ) {
                  throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                      enum_parsing_empty, msg_parsing_empty );
               }

               return;
            } else {
               // parse attribute
               // txt contains _node_value or attributes
               vector8_t vectorattributes( begin, end );
               rawattributes = vectorattributes;
               txt = strings::trim( getAttributes( vectorattributes ) );
            }
         } else {
            pos.skip();

            size_t pos_end_tag_slash = temp.find( "/>" );

            // test, if we found an endtag with slash
            if( pos_end_tag_slash == temp.size() - 2 ) {
               // found <name />
               return;
            }

            while( !pos.is_end() ) {

               // if at text start
               string txt1 = pos.next( string( "<" ).size() );

               if( txt1 != string( "<" ) ) { // not at '<'
                  XmlText* textNode = XmlText::create( t_alloc_line_file_method( __LINE__, __FILE__, __FUNCTION__ ) ) ;
                  string text = pos.next( pos.find( string( "<" ) ) );
                  pos += text.size();
                  ++pos;
                  textNode->parseText( text );

                  if( isEmptyTextNode( textNode->value() ) ) {
                     delete textNode; // do_delete
                  } else {

                     linkEndChild( textNode );
                     ke.Value( textNode->value() );

                  }

                  if( !pos.is_end() ) {
                     pos += -1; // set pos before textending "<"
                  }
               }

               // We hit a '</'
               txt1 = pos.next( string( "</" ).size() );

               if( txt1 != "</" ) {  // "</"
                  XmlNode* node = identify( pos );

                  if( node == 0 ) {
                     throw XmlException( t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                                         enum_unknown_node_type, msg_unknown_node + ": " + pos.next( 10 ) );
                  }

                  if( node ) {
                     node->_lookuppath = _lookuppath;
                     node->parse( pos );
                     linkEndChild( node );
                  }
               }

               pos.skip();
               static string marker = "</";
               string next_ = pos.next( string( "</" ).size() );

               if( next_ == "</" ) {
                  break;
               }
            }

            // endtag ?
            // </tag > and </tag> are both valid
            std::string nodeClosing = "</"; //  "</" ;
            nodeClosing += _node_value;

            if( pos.starts_with( nodeClosing ) ) {
               pos += nodeClosing.length();
               pos.skip();

               if( pos.next( string( ">" ).size() ) == string( ">" ) ) { // '>'
                  ++pos;
                  return;
               }

               throw XmlException(
                  t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                  enum_reading_endtag, msg_reading_endtag + ", endtag: '" + nodeClosing + "'" );
            } else {
               throw XmlException(
                  t_exception_line_file_method( __LINE__, __FILE__, __FUNCTION__ ),
                  enum_reading_endtag, msg_reading_endtag + ", endtag: '" + nodeClosing + "'" );
            }
         }
      }

      return;
   }

   std::vector<XmlAttribute> const& XmlElement::Attributes() const {
      return _attributes;
   }

   vector<XmlAttribute>::const_iterator XmlElement::firstAttribute() const {
      return _attributes.begin();
   }
   vector<XmlAttribute>::iterator XmlElement::firstAttribute()             {
      return _attributes.begin();
   }

   vector<XmlAttribute>::const_iterator XmlElement::lastAttribute()  const {
      if( _attributes.empty() ) {
         return _attributes.begin();
      }

      return _attributes.end() - 1 ;
   }
   vector<XmlAttribute>::iterator XmlElement::lastAttribute()              {
      if( _attributes.size() == 0 ) {
         return _attributes.begin();
      }

      return _attributes.end() - 1 ;
   }


   string XmlElement::getAttributes( vector8_t const& v ) {

      rawxml_position pp( v );

      while( true ) {
         vector8_t::const_iterator i = pp.find( '=' );

         if( i == pp.end() ) {
            // no valid attr in pos
            break;
         }

         pp.skip();

         XmlAttribute attrib;
         attrib.keys = _lookuppath;
         attrib.parseAttr( pp );

         _attributes.push_back( attrib );
      }

      // must be v.size(), end of raw vector with attributes
      std::ptrdiff_t running_position = pp.running_position();
      string s = pp.next( running_position );
      // must be "", no attributes left
      return s;
   }



}

//EOF
