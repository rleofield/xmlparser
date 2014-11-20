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

#ifndef RLF_UTL_H
#define RLF_UTL_H



#include <locale>
#include <string>
#include <cstdint>





namespace txml {



   template<int v>
   struct int2type {
      enum { value = v };
   };

   enum  class Encoding : int {
      UNKNOWN = 0,
      UTF8 = 1,
      LEGACY = 2
   };


   const std::string comment_start = "<!--";
   const std::string comment_end = "-->";

   const std::string declaration_start = "<?xml";
   const std::string declaration_end =  "?>";

   const std::string SINGLE_QUOTE = "\'";
   const std::string DOUBLE_QUOTE = "\"";
   const char whitespace = ' ';


   const std::string attr_version    = "version" ;
   const std::string attr_encoding   = "encoding" ;
   const std::string attr_standalone = "standalone" ;
   const std::string UTF_8 = "UTF-8";

   const size_t MAX_ENTITY_SIZE = 8; // &#xnnnn;
   const size_t MIN_ENTITY_SIZE = 3; // &n;


   std::string extract( std::string const& s, std::string const& start, std::string const& end );
   std::string readName( std::string const& temp );
   std::string readText( std::string const& p );


} //



#endif //

//EOF


