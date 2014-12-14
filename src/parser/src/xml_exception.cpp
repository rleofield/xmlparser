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


#include <vector>
//#include <boost/assign.hpp>
//#include <boost/algorithm/string.hpp>
//#include <boost/algorithm/string.hpp>
//#include <boost/lexical_cast.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <typeinfo>


#include "stringhelper.h"
#include "xml_exception.h"
#include "xml_utl.h"
#include "xml_document.h"
#include "rawxml_position.h"
#include "xml_fs.h"


using namespace std;


namespace txml {

   struct enum_list_entry {
      enum_list_entry( eException ee ): e( ee ), s() {}
      enum_list_entry( eException ee, string const& s_ ): e( ee ), s( s_ ) {}
      eException e;
      string s;
   };
   namespace {
      vector<enum_list_entry> enum_entries {
         { eException::none,                            msg_none   },
         { eException::bad_lexical_cast,                msg_bad_lexical_cast},
         { eException::reading_file,                    msg_reading_file},
         { eException::parsing_file,                    msg_parsing_file},
         { eException::parsing_element,                 msg_parsing_element},
         { eException::identify_element,                msg_identify_element},
         { eException::failed_to_read_element_name,     msg_failed_to_read_element_name},
         { eException::failed_to_read_element_closing_tag, msg_failed_to_read_element_closing_tag},
         { eException::failed_to_read_element_start_tag, msg_failed_to_read_element_start_tag},
         { eException::reading_element_value,           msg_reading_element_value},
         { eException::parse_attributes,                msg_reading_attributes},
         { eException::parsing_empty,                   msg_parsing_empty},
         { eException::reading_endtag,                  msg_reading_endtag},
         { eException::parsing_comment,                 msg_parsing_comment},
         { eException::parsing_declaration,             msg_parsing_declaration},
         { eException::document_empty,                  msg_document_empty},
         { eException::document_top_only,               msg_document_top_only},
         { eException::key_points_not_to_an_element,    msg_key_points_not_to_an_element},
         { eException::key_points_not_to_an_attribute,  msg_key_points_not_to_an_attribute},
         { eException::key_not_found,                   msg_key_not_found},
         { eException::unknown_node_type,               msg_unknown_node_type},
         { eException::unknown_node,                    msg_unknown_node},
         { eException::iterator_underflow,              msg_iterator_underflow},
         { eException::list_is_empty,                   msg_list_is_empty},
         { eException::keylist_in_visitor_has_length_zero,          msg_keylist_in_visitor_has_length_zero},
         { eException::operator_plus_at_or_after_end,   msg_operator_plus_at_or_after_end},
         { eException::operator_minus_at_or_after_end,  msg_operator_minus_at_or_after_end},
         { eException::parse_text,                      msg_parse_text},
         { eException::alloc_id_not_found,              msg_alloc_id_not_found},
         { eException::alloc_node_not_found,            msg_alloc_node_not_found},
         {eException::no_parsed_document_found ,        msg_no_parsed_document_found}
      };
   }

   string to_string( eException e ) {
      class f {
         eException _e;
      public:
         f( eException e ): _e( e ) {}
         bool operator()( enum_list_entry const& e )const {
            if( e.e == _e ) {
               return true;
            }

            return false;
         }
      };

      vector<enum_list_entry>::iterator i = find_if( enum_entries.begin(), enum_entries.end(), f( e ) );

      if( i != enum_entries.end() ) {
         return i->s;
      }

      return "unknown enum";
   }

   xml_exception::xml_exception( t_lfm const& lflfm_, eException e, string const& in )
      : runtime_error( in.c_str() ), _enum( e ), _what( in ), _lfm( lflfm_ )   {}


   const char* xml_exception::what() const throw() {
      return _what.c_str();
   }
   std::string xml_exception::What() const throw() {
      return _what;
   }
   xml_exception::~xml_exception() throw() {}
   eException const& xml_exception::getEnum()const {
      return _enum;
   }

   t_lfm const& xml_exception::lfm()const {
      return _lfm;
   }


} // end of namespace txml






// EOF

