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

#ifndef RLF_ELEMENT_LOCATOR_H
#define RLF_ELEMENT_LOCATOR_H


#include "xml_element.h"
#include "xml_visitor.h"


namespace txml {


   /** Locate element value in tree
   */
   class element_locator : public visitor_base {
   public:
      element_locator( std::string const& key );
      element_locator( txml::path const& key );


      v_ret enter( const xml_document& ) override final        {
         return v_ret::eRet::RECURSE;
      }
      v_ret exit( const xml_document& )  override final       {
         return v_ret::eRet::RECURSE;
      }

      v_ret enter( const xml_element& ) override final;
      v_ret exit( const xml_element& ) override final;
      v_ret visittext( const xml_text& text ) override final;

      v_ret visit( const xml_declaration& ) override final {
         return v_ret::eRet::RECURSE;
      }
      v_ret visit( const xml_comment& )   override final    {
         return v_ret::eRet::RECURSE;
      }


      xml_element* elementfound()const {
         return _elementfound;
      }
      bool accepted() const{
         return _elementfound != nullptr;
      }

      path lookup_path;
      path remainder;

   private:
      xml_element* _elementfound;
      //std::string _value;
      //std::string _attr;
      element_locator( element_locator const& );
      void operator=( element_locator const& );

   };

} // end of namespace txml


#endif

//EOF


