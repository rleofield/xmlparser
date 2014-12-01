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

   // keys for search in xml doc
   class keyentry {

   public:
      std::string to_string()const;
      keyentry();
      keyentry( std::string const& str );
      keyentry( keyentry const& keyentry_ );

      keyentry& operator=( keyentry const& keyentry_ );

      // compares elements only
      bool operator==( keyentry const& keyentry_ )const ;
      int childcount()const ;
      void childcount( int childcount_ ) ;

      std::string attr()const ;
      void attr( std::string const& a ) ;
      bool is_attr()const ;
      std::string Value()const ;
      void Value( std::string const& v ) ;

      std::string Element()const ;
      void Element( std::string element ) ;


      operator std::string() {
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

   class keyentries {
      std::vector<keyentry> _keyentries;
   public:
      keyentries();
      keyentries( std::string const& key );
      bool operator>( keyentries const& ke )const;
      bool operator==( keyentries const& k )const;
      keyentries& operator=( std::string const& key );
      keyentries& operator=( keyentries const& ke );

      std::string  to_string()const;
      void toPatterns( std::string const& key );
      bool empty()const ;
      //void add( std::string const& s ) ;
      void addEmpty() ;
      //void add( keyentry const& ke ) ;
      void insert_front( keyentry const& ke );
      keyentry const& last()const;
      keyentry& last();

      size_t size()const ;

      keyentry const& operator[]( size_t i )const;
      keyentry& operator[]( size_t i );

      xml_node const* parentOfLast()const;

      void remove_last() ;


      bool compareByElement( keyentries const& v1 )const ;

      bool compareByChildCount( keyentries const& v1 ) const ;

   };
   //std::vector<keyentry> to_keyentries( std::vector<std::string> );



} // end of namespace txml



#endif //

//EOF


