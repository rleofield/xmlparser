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


   xml_exception::xml_exception( t_lfm const& lflfm_, eEx e, string const& in )
      : runtime_error( in.c_str() ), _enum( e ), _what( in ), _lfm( lflfm_ )   {}


   const char* xml_exception::what() const throw() {
      return _what.c_str();
   }
   std::string xml_exception::What() const throw() {
      return _what;
   }
   xml_exception::~xml_exception() throw() {}
   eEx const& xml_exception::getEnum()const {
      return _enum;
   }

   t_lfm const& xml_exception::lfm()const {
      return _lfm;
   }


} // end of namespace txml






// EOF

