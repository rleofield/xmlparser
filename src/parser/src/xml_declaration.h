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

#ifndef RLF_XML_DECLARATION_H
#define RLF_XML_DECLARATION_H


#include "xml_node.h"



namespace txml {


   /** first line in xml
      <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
   */
   class xml_declaration : public xml_node {

      xml_declaration()   :
         xml_node( xml_node::eType::DECL ),
         _version( "1.0" ),
         _encoding(),
         _standalone() {
      }

      xml_declaration( const xml_declaration& copy );
      xml_declaration& operator=( const xml_declaration& copy );

      static void* operator new( size_t size, t_lfm const& lfm );
      static void operator delete( void* );

      std::string _version;
      std::string _encoding;
      std::string _standalone;


   public:
      static xml_declaration* create( t_lfm const& lfmcIn ) ;
      static xml_declaration* create() ;

      virtual ~xml_declaration()   {}


      void attributes( std::string const& temp );

      std::string const& version() const  {
         return _version;
      }
      std::string const& encoding() const {
         return _encoding;
      }
      std::string const& standalone() const {
         return _standalone;
      }

      virtual void parse( raw_buffer& pos );
      virtual void print( int depth, std::string& str ) const;
      virtual bool accept( xml_visitor* visitor ) const;

   protected:
      //void copy( xml_declaration& target ) const;

   };


} // end of namespace txml

#endif

//EOF


