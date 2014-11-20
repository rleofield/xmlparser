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

#ifndef RLF_COMMENT_H
#define RLF_COMMENT_H


#include "xml_node.h"



namespace txml {


   /**   A XML comment.
   */
   class xml_comment : public xml_node {


      xml_comment() : xml_node( xml_node::eNodeType::COMMENT ) {}
      xml_comment( const xml_comment& );
      xml_comment& operator=( const xml_comment& base );
      static void* operator new( size_t, t_lfm const& lfm );

   public:
      static void operator delete( void* );
      static xml_node* create( t_lfm const& lfm );
      static xml_node* create();

      virtual ~xml_comment() {}
      virtual xml_node* clone() const;
      virtual void parse( rawxml_position& );

      //      virtual const xml_comment*  toComment() const  {
      //         return this;
      //      }
      //      virtual       xml_comment*  toComment()     {
      //         return this;
      //      }

      virtual bool accept( xml_visitor* visitor ) const;

   protected:


   };




} // end of namespace txml



#endif

//EOF


