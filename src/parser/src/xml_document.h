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

#ifndef RLF_XML_DOCUMENT_H
#define RLF_XML_DOCUMENT_H

#include <list>

#include "xml_node.h"
#include "xml_utl.h"
#include "tPointers.h"



namespace txml {

   class tXmlInterfaceImpl;

   class xml_document : public xml_node {
      static void* operator new( size_t size, t_lfm const& lfm );
      static void operator delete( void* );
      static bool _isWhiteSpacePreserved;
      static Encoding _encoding;
      static std::string _bom;

      xml_document( const xml_document& copy );
      xml_document& operator=( const xml_document& copy );
      void clear();


   public:
      static tPointers pointers;
      static std::locale loc;
      bool save( const std::string& filename ) const ;

      xml_document();
      virtual ~xml_document();

      bool parse_begin( std::string  const& );

      void serialize( std::string& s ) const ;
      void serialize( std::list<std::string>& v ) const ;
      void serialize( std::vector<std::string>& v ) const ;

      void parse( raw_buffer& pos );
      //      const XmlElement* rootElement() const;
      //      XmlElement* rootElement();

      bool accept( xml_visitor* content ) const;

      static Encoding encoding() ;
      static void encoding( Encoding e );
      static void preserve_white_space( bool b )    ;
      static bool preserve_white_space();

   private:

      friend class tXmlInterfaceImpl;
      friend class xml_element;

   };


} // end of namespace txml


#endif

//EOF


