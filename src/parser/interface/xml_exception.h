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

#ifndef RLF_EXCEPTION_H
#define RLF_EXCEPTION_H

#include <string>
#include <stdexcept>

#include "tLfm.h"
using rlf_tlfm::t_lfm;

namespace txml {

   enum class eException : int {
      none = 0,
      no_parsed_document_found,
      bad_lexical_cast,
      reading_file,
      parsing_file,
      identify_element,
      failed_to_read_element_name,
      failed_to_read_element_closing_tag,
      failed_to_read_element_start_tag,
      reading_element_value,
      parse_attributes,
      parsing_empty,
      reading_endtag,
      parsing_element,
      parsing_comment,
      parse_text,
      parsing_declaration,
      document_empty,
      document_top_only,
      cannot_insert_root_as_default,
      key_points_not_to_an_element,
      key_points_not_to_an_attribute,
      key_not_found,
      unknown_node_type,
      unknown_node,
      iterator_underflow,
      list_is_empty,
      keylist_in_visitor_has_length_zero,
      operator_plus_at_or_after_end,
      operator_minus_at_or_after_end,
      alloc_id_not_found,
      alloc_node_not_found,
      bad_cast
   };
   std::string to_string( eException e );



   class xml_exception: public std::runtime_error {
      eException _enum;
      std::string _what;
      t_lfm _lfm;

   public:
      xml_exception( t_lfm const& lfm, eException e, std::string const& in );
      const char* what() const throw();
      std::string What() const throw();
      virtual ~xml_exception() throw();
      eException const& getEnum()const;
      t_lfm const& lfm()const;

   };



   const std::string msg_none = "no Error";
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
   const std::string msg_document_top_only( "xml_document added to document, because xml_document can only be at the root" );
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


