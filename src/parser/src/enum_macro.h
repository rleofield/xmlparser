/*
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

#ifndef RLF_ENUM_MACRO_H
#define RLF_ENUM_MACRO_H


#include <string>
#include <boost/preprocessor.hpp>



// http://stackoverflow.com/questions/5093460/how-to-convert-an-enum-type-variable-to-a-string
#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem)    \
   case elem : return BOOST_PP_STRINGIZE(elem);




#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)                \
   enum name {                                                               \
      BOOST_PP_SEQ_ENUM(enumerators)                                        \
   };                                                                        \
   \
   inline std::string to_string(name v) const                                      \
{                                                                         \
   switch (v)                                                            \
{                                                                     \
   BOOST_PP_SEQ_FOR_EACH(                                            \
   X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,          \
   name,                                                         \
   enumerators                                                   \
   )                                                                 \
   default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";         \
   }                                                                     \
   }


#define DEFINE_ENUM_WITH_STRING_CONVERSIONSnc(name, enumerators)                \
   enum name {                                                               \
      BOOST_PP_SEQ_ENUM(enumerators)                                        \
   };                                                                        \
   \
   inline std::string to_string(name v)                                       \
{                                                                         \
   switch (v)                                                            \
{                                                                     \
   BOOST_PP_SEQ_FOR_EACH(                                            \
   X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,          \
   name,                                                         \
   enumerators                                                   \
   )                                                                 \
   default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";         \
   }                                                                     \
   }


#endif

//EOF


