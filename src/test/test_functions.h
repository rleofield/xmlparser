/* --------------------------------------------------------------------------
Copyright 2012 by Richard Albrecht
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


#ifndef T_TEST_H
#define T_TEST_H


#define Expects assert
#define Ensures assert


// https://isocpp.org/wiki/faq/wg21
template<typename T, size_t Size> T* begin( T( & array )[Size] ) {
   return array;
}
template<typename T, size_t Size> T* end( T( & array )[Size] ) {
   return array + Size;
}

template <typename T>
inline char* to_char_ptr( std::vector<T>& b ) {
   return reinterpret_cast<char* >( b.data()  );
}
template <typename T>
inline char const* to_char_ptr( std::vector<T> const& b ) {
   return reinterpret_cast<char const* >( b.data()  );
}



#endif

//EOF

