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


#include "xml_utl.h"
#include "xml_printer.h"
#include "xml_text.h"
#include "xml_declaration.h"
#include "xml_comment.h"
#include "xml_printer.h"
#include "xml_attribute.h"
#include "xml_element.h"
#include "xml_exception.h"
#include "stringhelper.h"

#include "win32.h"

using namespace std;



namespace txml {


   void xml_printer::Buffer( list<string> &l )const {
      vector<string>  v = rlf_hstring::split( _buffer, _lineBreak );
      l.assign( v.begin(), v.end() );
   }



   bool xml_printer::enter( const xml_element& element ) {
      indent();
      _buffer += "<";
      _buffer += element.value();
      std::vector<xml_attribute> v = element.Attributes();

      for( vector<xml_attribute>::const_iterator attrib = v.begin(); attrib != v.end(); ++attrib ) {
         _buffer += " ";
         attrib->print( _buffer );
      }

      if( !element.firstChild() ) {
         _buffer += " />";
         _buffer += _lineBreak;
      } else {
         _buffer += ">";

         xml_text const* text = dynamic_cast<xml_text const*>( element.firstChild() );

         if( text != nullptr && element.lastChild() == element.firstChild()
           ) {
            simpleTextPrint = true;
            // no DoLineBreak()!
         } else {
            _buffer += _lineBreak;
         }
      }

      ++_depth;
      return true;
   }


   bool xml_printer::exit( const xml_element& element ) {
      --_depth;

      if( !element.firstChild() ) {
         // nothing.
      } else {
         if( simpleTextPrint ) {
            simpleTextPrint = false;
         } else {
            indent();
         }

         _buffer += "</";
         _buffer += element.value();
         _buffer += ">";
         _buffer += _lineBreak;
      }

      return true;
   }


   bool xml_printer::visit( const xml_text& text ) {
      if( simpleTextPrint ) {
         string str = text.value();
         _buffer += str;
         return true;
      }

      indent();
      string str = text.value();
      _buffer += str;
      _buffer += _lineBreak;

      return true;
   }


   bool xml_printer::visit( const xml_declaration& declaration ) {
      indent();

      declaration.print( 0, _buffer );
      _buffer += _lineBreak;
      return true;
   }


   bool xml_printer::visit( const xml_comment& comment ) {
      indent();
      _buffer += "<!--"; // "<!--";
      _buffer += comment.value();
      _buffer += "-->"; // "-->";
      _buffer += _lineBreak;
      return true;
   }

}



//EOF


