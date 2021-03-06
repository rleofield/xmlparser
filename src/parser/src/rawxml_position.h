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

#ifndef RLF_RAWXMLPOSITION_H
#define RLF_RAWXMLPOSITION_H


#include <vector>
#include <cstdint>
#include <cstddef>

#include "xml_utl.h"

namespace txml {

   typedef std::vector<int8_t> vector8_t;

   const bool useNext100 = true;

   class raw_buffer {

   private:
      vector8_t _rawxml;
      std::string next25( int2type<false> )const;
      std::string next25( int2type<true> )const;

      void next100( int2type<false> )const;
      void next100( int2type<true> )const;

      // position change doesn't change value of buffer
      mutable vector8_t::const_iterator _position;
      mutable std::string _next;

      raw_buffer( raw_buffer const& pos );
      void operator=( raw_buffer const& pos );
      std::string next_n( size_t n )const;

   public :
      raw_buffer( vector8_t const& v );
      raw_buffer( std::string  const& v );

      vector8_t::const_iterator end() {
         return _rawxml.end();
      }


      // next bytes for the debugger, "" in release
      void next100()const;
      std::string next25()const;

      // compares until s.size()
      bool starts_with( std::string const& s );

      bool operator<( raw_buffer const& p )const;

      bool is_at_end()const;
      vector8_t::const_iterator end()const;

      // remaining number of chars in buffer
      vector8_t::difference_type remainder()const;

      vector8_t::difference_type position()const;
      size_t size()const;

      // iterator at current position
      vector8_t::const_iterator running()const;

      // string from current to pos

      // pos is given
      std::string next( ptrdiff_t size )const;
      // an interator is given
      std::string next( vector8_t::const_iterator it )const;
      // ends after place of s in buffer
      std::string next_until( std::string const& s )const;


      //raw_buffer operator++( int );
      //void operator+=( ptrdiff_t offset )const;
      void advance( ptrdiff_t offset )const;


      // value at current pos
      uint8_t value()const;

      // find next string in buffer
      vector8_t::const_iterator find_next( std::string const& s )const;

      // skip ' ', '\n', '\t'
      void skip()const;

      // skip BOM, 2 or 3 chars
      void skip( Encoding e )const;

   };



} // end of namespace txml

#endif //

//EOF


