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

#ifndef RLF_PRINTER_H
#define RLF_PRINTER_H

#include <string>
#include <sstream>

#include <list>

#include "xml_visitor.h"
#include "tLog_Category_default.h"



namespace txml {




   const std::string tab = "\t";

   /** Print to memory functionality. The xml_printer is useful when you need to:
      -# Print to memory
      -# Control formatting (line endings, etc.)

    */
   class xml_printer : public xml_visitor {
   public:
      xml_printer( std::string const& t = tab ) : _depth( 0 ), _element_text_prints_inline( false ),
         _buffer(), _indent( t ), _lineBreak( "\n" ) {}


      virtual bool enter( const xml_element& element );
      virtual bool exit( const xml_element& element );

      virtual bool visit( const xml_declaration& declaration );
      virtual bool visit( const xml_text& text );
      virtual bool visit( const xml_comment& comment );

      void pretty_print_off() {
         _indent = "";
         _lineBreak = "";
      }

      const std::string result() const   {
         //std::string t = _buffer.str();
         return move( _buffer.str() );
      }


   private:
      std::string indent() const;


      int _depth;
      bool _element_text_prints_inline;
      std::ostringstream _buffer;
      std::string _indent;
      std::string _lineBreak;
   };




} // end of namespace txml



#endif

//EOF


