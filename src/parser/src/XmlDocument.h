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

#ifndef RL_XML_DOCUMENT_H
#define RL_XML_DOCUMENT_H



#include "XmlNode.h"
#include "xml_utl.h"


namespace txml {

   class tXmlInterfaceImpl;

   class XmlDocument : public XmlNode {
      static void* operator new( size_t size, t_alloc_line_file_method const& lfm );
      static void operator delete( void* );
      static bool _isWhiteSpacePreserved;
      static Encoding _encoding;
      static bool useMicrosoftBOM;

      XmlDocument( const XmlDocument& copy );
      XmlDocument& operator=( const XmlDocument& copy );
      void clear();

   public:
      static std::locale loc;
      bool saveFile( const std::string& filename ) const ;

      XmlDocument();
      virtual ~XmlDocument() {}

      bool parseStart( std::list<std::string>  const& v );

      void serialize( std::list<std::string> & v ) const ;

      virtual void parse( rawxml_position& pos );
      //      const XmlElement* rootElement() const;
      //      XmlElement* rootElement();

      virtual bool accept( XmlVisitor* content ) const;

      static Encoding encoding() ;
      static void encoding( Encoding e );
      static void setPreserveWhiteSpace( bool b )    ;
      static bool isWhiteSpacePreserved()   ;

   protected :
      // [internal use]
      virtual XmlNode* clone() const;

   private:
      void copy( XmlDocument& target ) const;

      friend class tXmlInterfaceImpl;


   };


} // end of namespace txml


#endif

//EOF


