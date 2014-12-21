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

#ifndef RLF_XML_TEXT_H
#define RLF_XML_TEXT_H


#include "xml_node.h"



namespace txml {



   class xml_visitor;
   class xml_element;
   class xml_document;
   class xml_comment;
   class xml_text;
   class xml_declaration;



   class xml_text : public xml_node {
      friend class xml_element;

      xml_text( const std::string& val ) :
         xml_node( xml_node::eType::TEXT, val ) {
      }
      static void* operator new( size_t size, t_lfm const& lfm );
      static void operator delete( void* );

      xml_text( const xml_text& xmlText ) : xml_node( xml_node::eType::TEXT ) {
         xml_node::value( xmlText.unencoded_value() );
      }
      xml_text& operator=( const xml_text& txt )                        {
         if( this != &txt ) {
            xml_node::value( txt.unencoded_value() );
         }

         return *this;
      }

   public:

      virtual ~xml_text() {}

      static xml_text* create( t_lfm const& lfmcIn, const std::string& value_ = std::string() ) ;
      static xml_text* create( const std::string& value_ ) ;

      const std::string value() const; // encoded value
      const std::string unencoded_value() const; // encoded value

      void parse( raw_buffer& );
      void parseText( std::string const& p );

      virtual bool accept( xml_visitor* content ) const;

   };


   std::string encodeEntities( std::string const& s ); // display -> XML
   std::string decodeEntities( std::string const& s ); // XML -> display




} // end of namespace txml




#endif //

//EOF


