/* --------------------------------------------------------------------------
    Copyright 2013 by Richard Albrecht
    richard.albrecht@rleofield.de

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



#ifndef RL_XML_STRINGS_H
#define RL_XML_STRINGS_H

#include <string>
#include <vector>

namespace strings {


   std::vector<std::string> split( std::string const& l, std::string const& pat ) ;
   std::string replace_all( std::string const& ins, const std::string& pattern, const std::string& replace ) ;
   size_t index( std::string const& s, std::string const& pattern, size_t pos ) ;
   std::string trim( std::string const& str, char ch = ' ' ) ;
   std::string fillup( std::string const& in, char ch, size_t n ) ;
   std::string to_string( double val ) ;
   std::string to_string( int val ) ;
   std::string to_string( size_t val ) ;
   std::string to_string( int64_t val ) ;

   int to_int( std::string const& s );
   double to_double( std::string const& s, std::locale l ) ;
   double to_double( std::string const& s ) ;

   size_t hex_to_size_t( std::string const& s );
   std::vector<std::string> tokenize( std::string const& str, const std::string& delimiters ) ;
}

#endif

//EOF


