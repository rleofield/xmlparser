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
#include "tLog_Category_default.h"

using namespace std;



namespace txml {




   v_ret xml_printer::enter( const xml_element& element ) {
      string val = element.value();

      _buffer << indent();
      _buffer << "<";
      _buffer << element.value();


      string s = _buffer.str();
      //LOGT_INFO( s );


      for( auto & a :   element.Attributes() ) {
         _buffer << " ";
         _buffer <<  a.print( );
      }

      if( element.Attributes().size() > 0 ) {
         //_buffer << " ";
      }


      if( element.isClosed() ) {
         _buffer << string( " " ) + "/>"; // closed element, no childs
         _buffer << _lineBreak;
      } else {
         _buffer << ">"; // element end
         xml_text const* text = element.text();

         bool b = element.last_child() == element.first_child();

         if( text != nullptr && b
           ) {
            // has only text, no other childs
            // no _lineBreak
            _element_text_prints_inline = true;
         } else {
            _buffer << _lineBreak;
         }
      }

      s = _buffer.str();
      //LOGT_INFO( s );

      ++_depth;
      return v_ret::eRet::RECURSE;
   }


   v_ret xml_printer::exit( const xml_element& element ) {
      --_depth;

      xml_node const* ch = element.first_child();

      if( ch == nullptr ) {
         return v_ret::eRet::RECURSE;
      } else {
         if( _element_text_prints_inline ) {
            _element_text_prints_inline = false;
         } else {
            _buffer << indent();

         }

         std::string t = _buffer.str();
         //LOGT_DEBUG( t );

         _buffer << "</";
         string v = element.value();
         _buffer << v;
         _buffer << ">";
         _buffer << _lineBreak;
      }

      string s = _buffer.str();
      //LOGT_INFO( s );

      return v_ret::eRet::RECURSE;
   }


   v_ret xml_printer::visittext( const xml_text& text ) {
      if( _element_text_prints_inline ) {
         string s = _buffer.str();
         //LOGT_INFO( s );
         _buffer << text.value();
         return v_ret::eRet::RECURSE;
      }

      _buffer << indent();
      string v = text.value();
      _buffer <<  v << _lineBreak ;

      string s1 = _buffer.str();
      //LOGT_INFO( s1 );

      return v_ret::eRet::RECURSE;
   }


   v_ret xml_printer::visit( const xml_declaration& declaration ) {
      _buffer << indent();

      _buffer <<  declaration.print( 0 );
      _buffer << _lineBreak;
      return v_ret::eRet::RECURSE;
   }


   v_ret xml_printer::visit( const xml_comment& comment ) {
      _buffer << indent();
      _buffer << comment.print();
      _buffer << _lineBreak;
      return v_ret::eRet::RECURSE;
   }

   std::string xml_printer::indent() const  {
      std::string str;
      uint32_t d = _depth;

      for( int i = 0; i < _depth; ++i ) {
         str += _indent;
      }

      std::string tmp = std::move( str );
      //LOGT_DEBUG( "indent: \'" + tmp + "\"" );
      return tmp;
   }

}



//EOF


