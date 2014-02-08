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


#include <vector>

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <typeinfo>


#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>


#include "tLog_Category_A.h"

#include "xml_utl.h"
#include "xml_fs.h"

#include "stringhelper.h"
#include "alloccheck.h"

#include "win32.h"
using namespace std;




/**
     Überladung von new mit Markern:

     Jeder Aufruf von new/delete wird prokolliert.
     Im allozierten Speicher wird am bein Aufruf von New() eine Struktur eingefügt,
     die bei Delete geprüft wird.
     Alle new/delete werden in einer STL-Map abglegt.
     Am Programmende wird die Map ausgegeben.
     Wenn es zu jedem new ein Delete gibt, ist die Map am Programmende leer.
     */

namespace alloccheck {

   template<bool v>
   struct int2type {
      enum { value = v };
   };


   string clip_at_pos( string const& s, size_t pos ) {
      if( s.size() > 0 ) {
         size_t i = pos;

         if( i != string::npos ) {
            return s.substr( 0, i ) ;
         }
      }

      return s;
   }


   tAlloccheck& alloccheckInstance() {
      static tAlloccheck _alloccheckInstance;
      return  _alloccheckInstance;
   }

   const char pathSlash = '/';

   string getFilennameFromPath( string const&  path_file ) {
      size_t i = path_file.rfind( pathSlash );

      if( i  != string::npos ) {
         return path_file.substr( i );
      }

      //slash not found
      return path_file;
   }

   // help structure 'AllocHeader', to maintain new/delete info

   const size_t maxalloc = 32; // max length of filename, classname, methodname in help struct 'AllocHeader'
   const size_t maxallocinfo = 128; // max length of logging info in 'AllocHeader'
   struct alloc_header {     // Header, der jedem Speicherbereich vorangestellt wird
      size_t size;           // size of allocaed memory
      size_t line;           // line where new() was called
      size_t magic;          // magic number, added at end of allocated memory, test for buffer overflows
      char file[maxalloc];     // filename
      char info[maxallocinfo]; // short info msg
      char _class[maxalloc];   // Name der Klasse, in der new/delete aufgerufen wurde
      char function[maxalloc]; // Name der function, in der new/delete aufgerufen wurde
   };

#ifdef __linux__
   static boost::mutex MUTEX;   // locking new/delete in concurrent threads
#endif



   // statische Marker im Logfile,
   //   damit können mit grep die wichtigen Logzeilen gefiltert werden
   string grepMarker = "xmldemotest";

   //////////////////////////////////////////////////////////////////////////////////////
   // wird von new() aufgerufen, protokolliert Zeile, Name der Methode, Name der Klasse
   void* LocalAlloc( int2type<false>, size_t size, t_alloc_line_file_method const& )  {
      char* p = ::new char[size];
      return p;
   }


   void* LocalAlloc( int2type<true>, size_t size, t_alloc_line_file_method const& lfmcIn )  {


      // es wird Speicher mit new angefordert, + sizeof(AllocHeader) mehr + 64 bit for magic number at end
      char* p = ::new( nothrow ) char[sizeof( alloc_header ) + size + sizeof( size_t )];

      if( NULL == p ) {
         throw "allocation fails : no free memory";
      }

      char* p_start = p + sizeof( alloc_header );
      char* p_magicend = p + sizeof( alloc_header ) + size;

      size_t line = lfmcIn.line();
      string file = lfmcIn.file();
      string method = lfmcIn.method();

      // strip path from file
      file = getFilennameFromPath( file );

      // clip file, method and class names
      file = clip_at_pos( file, maxalloc - 1 );
      method = clip_at_pos( method, maxalloc - 1 );

      alloc_header* pHeader = ( alloc_header* )p;

      if( file.size() > maxalloc - 1 ) {
         file = file.substr( 0, maxalloc - 1 );
      }

      tAlloccheck& check = alloccheckInstance();


      // build info message for logging
      string info = "size: " + rlf_hstring::toString( ( int )size ) + "/" + rlf_hstring::toString( ( int )check.totalalloc )
                    + ", file: " +  file
                    + ", method: " +  method
                    + ", line: " + rlf_hstring::toString( ( int )line );

      // clip info
      if( info.length() > maxallocinfo - 1 ) {
         info = info.substr( 0, maxallocinfo - 1 );
      }

      pHeader->size = size;
      pHeader->line = line;
      pHeader->magic = rand();

      pHeader->file[0] = 0;
      pHeader->info[0] = 0;

      ::strncat( pHeader->file, file.c_str(), file.size() );
      ::strncat( pHeader->info, info.c_str(), info.size() );

      *( ( size_t* )p_magicend ) = pHeader->magic;

      size_t key = ( size_t )pHeader;
      {
         //nsl::tLock l(m);
         check.alloccount++;
         check.totalalloc += static_cast<int>( size );
         tAllocMap& allocList = check.getMutableAlloclist();
         allocIterator it = allocList.find( key );

         if( it != allocList.end() ) {
            string ll = grepMarker + " 'allocs map has used key':  " + rlf_hstring::toString( ( int )key ) + ", info: " + info + "'";
            LOGT_A_INFO( ll );
            exit( 0 );
         }

         allocList[key] = info;
      }
      string logLine = grepMarker + " Alloc: count:  " + rlf_hstring::toString( ( float )check.alloccount ) + ", info: " + info;
      //LOGT_INFO( logLine );
      return ( void* )( p_start );

   }
   ////////////////////////////////////////////////////////////////////////////////////////////
   // wird von delete() aufgerufen
   void LocalDelete( int2type<false>, void* p )  {
      delete []( ( char* )p );
      return;
   }
   void LocalDelete( int2type<true>, void* p )  {


      //logger::tLog const& logger_ = logger::logger();
      char* p_start = ( ( char* )p - sizeof( alloc_header ) );

      alloc_header* pHeader = ( alloc_header* )( p_start );
      pHeader->file[maxalloc - 1] = 0;
      pHeader->info[maxallocinfo - 1] = 0;
      pHeader->_class[maxalloc - 1] = 0;

      string file = pHeader->file;
      string info = pHeader->info;
      size_t size = pHeader->size;
      size_t magic = pHeader->magic;

      char* p_magicend = ( char* )p + size;
      size_t magic_at_end = *( ( size_t* )p_magicend );

      if( magic != magic_at_end ) {
         LOGT_A_INFO( grepMarker + " buffer overflow at: " + string( info ) );
         exit( 0 );
      }

      size_t key = ( size_t )pHeader;

      tAlloccheck& check = alloccheckInstance();

      size_t lastalloccount = 0;
      {
         //nsl::tLock l(m);
         tAllocMap& allocList = check.getMutableAlloclist();
         allocIterator it = allocList.find( key );
         lastalloccount = allocList.size();
         check.alloccount--;

         if( it != allocList.end() ) {
            allocList.erase( key );
         } else {
            LOGT_A_INFO( grepMarker + " delete nicht in map gefunden: " + string( info ) );
            exit( 0 );
         }

         check.totalalloc -= static_cast<int>( size );
      }

      string infolog = grepMarker
                       + " delete: count: "
                       + rlf_hstring::toString( ( int )check.alloccount )
                       + ", mem left: "
                       + rlf_hstring::toString( check.totalalloc )
                       + ", info: '" + info + "'";
      //logger_.info( lfm_, infolog );

      // final delete of allocated memory
      // done in   AllocatedPtrList

      delete []( p_start );



      tAllocMap const& allocList2 = check.getAlloclist();

      if( check.alloccount != allocList2.size() ) {
         int diff = static_cast<int>( check.alloccount ) - static_cast<int>( lastalloccount );
         LOGT_A_INFO( grepMarker + " Error in 'delete': missingcount: " + rlf_hstring::toString( diff ) );
      }






   }

   void* LocalAlloc( size_t size, t_alloc_line_file_method const& lfmcIn )  {
      return LocalAlloc( int2type<useAllocCheck>(),  size, lfmcIn ) ;
   }
   void LocalDelete( void* p )  {
      LocalDelete( int2type<useAllocCheck>(),  p );
   }


} // end of namespace alloccheck





//EOF
