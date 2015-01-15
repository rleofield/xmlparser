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
#include "enum_macro.h"

#include "tLfm.h"
using rlf_tlfm::t_lfm;

namespace txml {


   DEFINE_ENUM_WITH_STRING_CONVERSIONSnc( eEx,
                                          ( parse )
                                          ( cast )
                                          ( read )
                                          ( pathlist )
                                          ( buffer )
                                          ( visitor )
                                          ( link )
                                          ( set )
                                        )



   class xml_exception: public std::runtime_error {
      eEx _enum;
      std::string _what;
      t_lfm _lfm;

   public:
      xml_exception( t_lfm const& lfm, eEx e, std::string const& in );
      const char* what() const throw();
      std::string What() const throw();
      virtual ~xml_exception() throw();
      eEx const& getEnum()const;
      t_lfm const& lfm()const;



   };


#define Xml_exception( v0, v1 )   xml_exception( tlfm_, v0, v1 );



   const std::string msg_parsing_comment( "parse comment" );
   const std::string msg_parsing_element( "parse element" );
   const std::string msg_parse_text( "use parse text() in" );

   const std::string msg_document( "document" );
   const std::string msg_cannot_insert_root_as_default( "Cant't insert root as default" );

   const std::string msg_key_points_not_to_an_element( "key points not to an element" );
   const std::string msg_unknown_node_type( "unknown node type" );
   const std::string msg_unknown_node( "unknown node" );
   const std::string msg_iterator_underflow( "next(it), iterator underflow: " );
   const std::string msg_list_is_empty( "list is empty" );
   const std::string msg_keylist_in_visitor_has_length_zero( "keylist in visitor has length zero" );
   const std::string msg_operator_plus_at_or_after_end( "operator += at or after end" );

} // end of namespace txml



#endif //

//EOF


