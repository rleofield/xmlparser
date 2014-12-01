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

   class rawxml_position {

   public:

   private:
      vector8_t _rawxml;
      std::string next25( int2type<false> )const;
      std::string next25( int2type<true> )const;

      void next100( int2type<false> )const;
      void next100( int2type<true> )const;

      mutable vector8_t::const_iterator _running;
      mutable std::string _next;

      rawxml_position( rawxml_position const& pos );
      void operator=( rawxml_position const& pos );

   public :
      rawxml_position( vector8_t const& v );
      rawxml_position( std::string  const& v );

      vector8_t::const_iterator end() {
         return _rawxml.end();
      }


      // next bytes for the debugger, "" if release
      void next100()const;
      std::string next25()const;

      // compares until s.size()
      bool starts_with( std::string const& s );

      bool operator<( rawxml_position const& p )const;

      bool is_end()const;
      vector8_t::const_iterator end()const;
      std::ptrdiff_t running_position()const;

      std::string next( ptrdiff_t size )const;
      std::string next( vector8_t::const_iterator it )const;
      std::string next_until( std::string const& s )const;


      rawxml_position& operator++();

      void operator+=( int offset );
      void operator+=( size_t offset ) {
         operator+=( static_cast<int>( offset ) );
      }
      void operator-=( int i )const;

      size_t position()const;
      size_t size()const;

      vector8_t::const_iterator running()const;

      operator int8_t const* ()const;
      vector8_t::const_iterator find( int8_t ch )const;
      vector8_t::const_iterator find( std::string const& s )const;

      bool is_white_space()const;

      // skip ' ', '\n', '\t'
      void skip()const;

      // skip BOM, 2 or 3 chars
      void skip( Encoding e )const;
      vector8_t::difference_type remainder()const;

   };



} // end of namespace txml

#endif //

//EOF


