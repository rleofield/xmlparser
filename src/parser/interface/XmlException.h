/* --------------------------------------------------------------------------
    Copyright 2013 by Richard Albrecht
    richard.albrecht@rleofield.de
    www.rleofield.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ------------------------------------------------------------------------------
*/

#ifndef RL_XML_EXCEPTION_H
#define RL_XML_EXCEPTION_H

#include <string>
#include <stdexcept>
//#include "stringhelper.h"


namespace txml {

   enum exception_enum {
      enum_none = 1,
      enum_no_parsed_document_found,
      enum_bad_lexical_cast,
      enum_reading_file,
      enum_parsing_file,
      enum_identify_element,
      enum_failed_to_read_element_name,
      enum_failed_to_read_element_closing_tag,
      enum_failed_to_read_element_start_tag,
      enum_reading_element_value,
      enum_parse_attributes,
      enum_parsing_empty,
      enum_reading_endtag,
      enum_parsing_element,
      enum_parsing_comment,
      enum_parse_text,
      enum_parsing_declaration,
      enum_document_empty,
      enum_document_top_only,
      enum_cannot_insert_root_as_default,
      enum_key_points_not_to_an_element,
      enum_key_points_not_to_an_attribute,
      enum_key_not_found,
      enum_unknown_node_type,
      enum_unknown_node,
      enum_iterator_underflow,
      enum_list_is_empty,
      enum_keylist_in_visitor_has_length_zero,
      enum_operator_plus_at_or_after_end,
      enum_operator_minus_at_or_after_end,
      enum_alloc_id_not_found,
      enum_alloc_node_not_found,
      enum_bad_cast

   };
   std::string to_string( exception_enum e );

   class t_exception_line_file_method {
      size_t _line;
      std::string _file;
      std::string _method;
   public:
      t_exception_line_file_method(): _line( 0 ), _file( "" ), _method( "" ) {}

      t_exception_line_file_method( size_t line_, std::string const& file_, std::string const& method_ )
         : _line( line_ ), _file( file_ ), _method( method_ )
      {}
      size_t line()const {
         return _line;
      }
      std::string const& file()const {
         return _file;
      }

      std::string const& method()const {
         return _method;
      }
      std::string to_string()const {
         return "File: " + file() + ", Line: " + std::to_string( line() ) + ",Method: " + method();
      }
      ~t_exception_line_file_method() {}
   };


   class XmlException: public std::runtime_error {
      exception_enum _enum;
      std::string _what;
      t_exception_line_file_method lfm;

      //      XmlException( XmlException const&);
      //      void operator=( XmlException const& );
   public:
      XmlException( t_exception_line_file_method const& lfm, exception_enum e, std::string const& in );
      const char* what() const throw() {
         return _what.c_str();
      }
      std::string What() const throw() {
         return _what;
      }
      virtual ~XmlException() throw() {}
      exception_enum const& getEnum()const {
         return _enum;
      }
      std::string getEnumString()const {
         return to_string( _enum );
      }
      size_t line()const;
      std::string const& file()const;
      std::string const& method()const;
      std::string file_line_method()const {
         return lfm.to_string();
      }
   };



   const std::string msg_none( "no Error" );
   const std::string msg_no_parsed_document_found( "no parsed document found" );
   const std::string msg_bad_lexical_cast( "bad lexical cast" );
   const std::string msg_reading_file( "reading file" );
   const std::string msg_parsing_file( "parsing file" );
   const std::string msg_identify_element( "identify element" );
   const std::string msg_failed_to_read_element_name( "read element name" );
   const std::string msg_failed_to_read_element_closing_tag( "read element name, closing bracket " );
   const std::string msg_failed_to_read_element_start_tag( "read element name, start bracket " );
   const std::string msg_reading_element_value( "read element value." );
   const std::string msg_reading_attributes( "read attributes." );
   const std::string msg_parsing_empty( "empty tag" );
   const std::string msg_reading_endtag( "read end tag" );
   const std::string msg_parsing_comment( "parse comment" );
   const std::string msg_parsing_element( "parse element" );
   const std::string msg_parse_text( "use ParseText() in" );

   const std::string msg_parsing_declaration( "parsing declaration" );
   const std::string msg_document_empty( "document empty" );
   const std::string msg_document_top_only( "XmlDocument added to document, because XmlDocument can only be at the root" );
   const std::string msg_cannot_insert_root_as_default( "Cant't insert root as default" );

   const std::string msg_key_points_not_to_an_element( "key points not to an element" );
   const std::string msg_key_points_not_to_an_attribute( "key points not to an attribute" );
   const std::string msg_key_not_found( "key not found" );
   const std::string msg_unknown_node_type( "unknown node type" );
   const std::string msg_unknown_node( "unknown node" );
   const std::string msg_iterator_underflow( "iterator underflow" );
   const std::string msg_list_is_empty( "list is empty" );
   const std::string msg_keylist_in_visitor_has_length_zero( "keylist in visitor has length zero" );
   const std::string msg_operator_plus_at_or_after_end( "operator plus at or after end" );
   const std::string msg_operator_minus_at_or_after_end( "operator minus at or after end" );

   const std::string msg_alloc_id_not_found( "alloc id not found" );
   const std::string msg_alloc_node_not_found( "alloc node not found" );
   const std::string msg_bad_cast( "bad cast" );




} // end of namespace txml



#endif //

//EOF


