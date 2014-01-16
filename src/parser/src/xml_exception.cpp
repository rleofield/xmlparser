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
#ifdef _WIN32
#pragma warning( disable : 4291 ) // Warning   no matching operator delete found; memory will not be freed if initialization throws an exception
#pragma warning( disable : 4800 ) //
#pragma warning( disable : 4996 ) // Warning   This function or variable may be unsafe. Consider using localtime_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning( disable : 4804 ) // Warning   unsafe use of type 'bool' in operation
#pragma warning( disable:4996 4100) // _CRT_SECURE_NO_WARNINGS
#endif

#include <vector>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <typeinfo>


#include "XmlException.h"
#include "xml_utl.h"
#include "XmlDocument.h"
#include "rawxml_position.h"
#include "xml_fs.h"

#include "strings.h"

using namespace std;


namespace txml {


   std::string to_string( exception_enum e ) {
      if( e == enum_none ) {
         return msg_none;
      }

      if( e == enum_no_parsed_document_found ) {
         return msg_no_parsed_document_found;
      }

      if( e == enum_bad_lexical_cast ) {
         return msg_bad_lexical_cast;
      }

      if( e == enum_reading_file ) {
         return msg_reading_file;
      }

      if( e == enum_parsing_file ) {
         return msg_parsing_file;
      }

      if( e == enum_parsing_element ) {
         return msg_parsing_element;
      }

      if( e == enum_identify_element ) {
         return msg_identify_element;
      }

      if( e == enum_failed_to_read_element_name ) {
         return msg_failed_to_read_element_name;
      }

      if( e == enum_failed_to_read_element_closing_tag ) {
         return msg_failed_to_read_element_closing_tag;
      }

      if( e == enum_failed_to_read_element_start_tag ) {
         return msg_failed_to_read_element_start_tag;
      }

      if( e == enum_reading_element_value ) {
         return msg_reading_element_value;
      }

      if( e == enum_parse_attributes ) {
         return msg_reading_attributes;
      }

      if( e == enum_parsing_empty ) {
         return msg_parsing_empty;
      }

      if( e == enum_reading_endtag ) {
         return msg_reading_endtag;
      }

      if( e == enum_parsing_comment ) {
         return msg_parsing_comment;
      }

      if( e == enum_parsing_declaration ) {
         return msg_parsing_declaration;
      }

      if( e == enum_document_empty ) {
         return msg_document_empty;
      }

      if( e == enum_document_top_only ) {
         return msg_document_top_only;
      }

      if( e == enum_key_points_not_to_an_element ) {
         return msg_key_points_not_to_an_element;
      }

      if( e == enum_key_points_not_to_an_attribute ) {
         return msg_key_points_not_to_an_attribute;
      }

      if( e == enum_key_not_found ) {
         return msg_key_not_found;
      }

      if( e == enum_unknown_node_type ) {
         return msg_unknown_node_type;
      }

      if( e == enum_unknown_node ) {
         return msg_unknown_node;
      }

      if( e == enum_iterator_underflow ) {
         return msg_iterator_underflow;
      }

      if( e == enum_list_is_empty ) {
         return msg_list_is_empty;
      }

      if( e == enum_keylist_in_visitor_has_length_zero ) {
         return msg_keylist_in_visitor_has_length_zero;
      }

      if( e == enum_operator_plus_at_or_after_end ) {
         return msg_operator_plus_at_or_after_end;
      }

      if( e == enum_operator_minus_at_or_after_end ) {
         return msg_operator_minus_at_or_after_end;
      }

      if( e == enum_parse_text ) {
         return msg_parse_text;
      }

      if( e == enum_alloc_id_not_found ) {
         return msg_alloc_id_not_found;
      }

      if( e == enum_alloc_node_not_found ) {
         return msg_alloc_node_not_found;
      }

      return "unknown enum";
   }

   XmlException::XmlException( t_exception_line_file_method const& lfmIn, exception_enum e, std::string const& in )
      : runtime_error( in.c_str() ), _enum( e ), _what( in ), lfm( lfmIn )   {}

   size_t XmlException::line()const {
      return lfm.line();
   }
   std::string const& XmlException::file()const {
      return lfm.file();
   }

   std::string const& XmlException::method()const {
      return lfm.method();
   }


} // end of namespace txml






// EOF

