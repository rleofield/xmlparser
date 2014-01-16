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


#ifndef RL_XML_ALLOC_CHECK_H
#define RL_XML_ALLOC_CHECK_H

#include <stdlib.h>
#include <map>


namespace alloccheck {

   const bool useAllocCheck = true;



   typedef std::map<size_t, std::string>::const_iterator allocIterator;
   typedef std::map<size_t, std::string> tAllocMap;

   class tAlloccheck {
      tAllocMap alloclist;

      // secure singleton, don't allow a copy
      tAlloccheck( tAlloccheck const& pos );
      void operator=( tAlloccheck const& pos );


   public:
      tAlloccheck(): alloclist(), alloccount( 0 ), totalalloc( 0 ) {}

      tAllocMap const& getAlloclist()const {
         return alloclist;
      }
      tAllocMap& getMutableAlloclist() {
         return alloclist;
      }
      size_t alloccount;
      int totalalloc;


   };

   tAlloccheck& alloccheckInstance();

   class t_alloc_line_file_method {
      size_t _line;
      std::string _file;
      std::string _method;
   public:
      t_alloc_line_file_method(): _line( 0 ), _file( "" ), _method( "" ) {}

      t_alloc_line_file_method( size_t line_, std::string const& file_, std::string const& method_ )
         : _line( line_ ), _file( file_ ), _method( method_ )
      {}
      size_t line()const {
         return _line;
      }
      std::string const& file()const {
         return _file;
      }

      std::string const& method()const {
         return _method;
      }
      ~t_alloc_line_file_method() {}
   };


   // wird von new() aufgerufen, protokolliert Zeile, Name der Methode, Name der Klasse
   void* LocalAlloc( size_t size, t_alloc_line_file_method const& lfmcIn );

   // wird von delete(void* p) aufgerufen
   void LocalDelete( void* p ) ;

} // end of ns alloccheck




#endif //

//EOF


