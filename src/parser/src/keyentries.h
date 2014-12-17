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


#ifndef RLF_KEYENTRIES_H
#define RLF_KEYENTRIES_H

#include <string>
#include <vector>



namespace txml {

   class xml_node;

   // path_elements for search in xml doc
   // contains element names and attributename
   class path_element {
         std::string to_string()const;

   public:
      path_element();
      path_element( std::string const& str );
      path_element( path_element const& keyentry_ );

      path_element& operator=( path_element const& keyentry_ );

      // compares elements only
      bool operator==( path_element const& keyentry_ )const ;
      int childcount()const ;
      void childcount( int childcount_ ) ;

      std::string const& attr()const ;
      void attr( std::string const& a ) ;
      bool is_attr()const ;
      //std::string const& value()const ;
      void value( std::string const& v ) ;

      std::string Element()const ;
      void Element( std::string element ) ;


      operator std::string()const {
         return to_string();
      }

      xml_node const* node()const ;
      xml_node* node() ;
      void node( xml_node* n ) ;
      // for default inserter
      void removeAttr();

      std::string toValue()const;

   private:
      std::string _element;
      std::string _attr;
      int _childcount;
      std::string _value;
      static std::string left_bracket;
      static std::string right_bracket;
      static std::string double_colon;
      xml_node* _node;


   };

   class path {
      std::vector<path_element> _keyentries;
      std::string  to_string()const;
   public:
      path();
      path( std::string const& key );
      bool operator>( path const& ke )const;
      bool operator==( path const& k )const;
      path& operator=( std::string const& key );
      path& operator=( path const& ke );

      operator std::string()const {
         return to_string();
      }


      void toPatterns( std::string const& key );
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

   };



} // end of namespace txml



#endif //

//EOF


