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


#ifndef RLF_PATH_H
#define RLF_PATH_H

#include <string>
#include <vector>

#include "path_element.h"


namespace txml {

   class xml_node;

   class xml_document;


   class path {
      std::vector<path_element> _keyentries;
      std::string  to_string()const;
   public:
      path();
      path( std::string const& key );
      void from_string( std::string const& key );
      bool operator>( path const& ke )const;
      bool operator==( path const& k )const;
      path& operator=( std::string const& key );
      path& operator=( path const& ke );

      operator std::string()const {
         return to_string();
      }


      bool points_to_attr()const;

      bool empty()const ;
      void addEmpty() ;
      void add( path_element const& ke );
      void insert_front( path_element const& ke );

      path_element const& last()const;
      path_element& last();

      size_t size()const ;

      path_element const& operator[]( size_t i )const;
      path_element& operator[]( size_t i );

      xml_node const* parentOfLast()const;

      void remove_last();

      bool compareByElement( path const& v1 )const ;
      bool compareByChildCount( path const& v1 ) const ;

      std::string toElementText( xml_document const& )const;


   };



} // end of namespace txml



#endif //

//EOF


