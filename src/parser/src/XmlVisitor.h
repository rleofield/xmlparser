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
- locator 'XmlLocator' for simple searching/changing the xml document
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

#ifndef RL_XML_VISITOR_H
#define RL_XML_VISITOR_H


namespace txml {


   class XmlNode;
   class XmlDocument;
   class XmlElement;
   class XmlDeclaration;
   class XmlText;
   class XmlComment;

   /**

      (Document, Element) called with a VisitEnter/VisitExit pair.
      leaves are called with Visit().
      'true' == recursive parsing
      'false' == no children of this node visited
      Accept() is called on the XmlDocument, although all nodes suppert Visiting.
Visitor lets you define a new operation without changing the classes of the elements on which it operates.
      @sa XmlNode::Accept()
   */
   class XmlVisitor {

   public:
      XmlVisitor() {}
      virtual ~XmlVisitor() {}

      virtual bool enter( const XmlDocument& )        {
         return true;
      }
      virtual bool visitExit( const XmlDocument& )         {
         return true;
      }

      virtual bool visitEnter( const XmlElement& )  {
         return true;
      }
      virtual bool visitExit( const XmlElement& )      {
         return true;
      }

      virtual bool visit( const XmlDeclaration& )  {
         return true;
      }
      virtual bool visit( const XmlText& )             {
         return true;
      }
      virtual bool visit( const XmlComment& )       {
         return true;
      }
   };



} // end of namespace txml



#endif

//EOF


