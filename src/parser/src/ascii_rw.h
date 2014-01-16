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

#ifndef RL_XML_ASCII_RW_H
#define RL_XML_ASCII_RW_H



#include <string>
#include <stdexcept>
#include <list>
#include <vector>



namespace ascii {


   /*! BadAsciiRead
   Exception, wenn ein text falsch gelesen wurde
   \param [in] msg  Fehlermeldung
   */
   class BadAsciiRead: public std::exception {
      std::string _what;
   public:
      BadAsciiRead( const std::string& msg ): _what( msg )
      { }
      ~BadAsciiRead()throw() {}
      const char* what() const throw() {
         return _what.c_str();
      }

   };
   class BadAsciiWrite: public std::runtime_error {
   public:
      BadAsciiWrite( const std::string& msg )
         : std::runtime_error( msg ) { }
   };


   /*! \class tReadAscii
   *  \brief reads and writes ascii files
   *
   * an asciifile is stored in a <b>vector</b>
   */
   class tReadText  {
      tReadText& operator= ( const tReadText& in );
      tReadText( const tReadText& in );

   public:
      tReadText() {}
      ~tReadText() {}
      void operator()( std::list<std::string> &v, const std::string& file );
      void operator()( std::vector<std::string> &v, const std::string& file );

   };


   /*! \class tWriteAscii
       *  \brief writes ascii files
       *
       * an asciifile is stored in an <b>string</b> list<br>
       */
   class tWriteText  {
      // no copy allowed
      tWriteText( const tWriteText& in );
      tWriteText& operator= ( const tWriteText& in );

   public:
      tWriteText() {}
      ~tWriteText() {}
      size_t operator()( std::list<std::string> const& lines , const std::string& file );
      size_t operator()( std::vector<std::string> const& lines , const std::string& file );


   };

}







#endif //

//EOF


