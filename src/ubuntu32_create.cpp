
/*
  test

*/

#include <iostream>


#include <tLog_Category_A.h>

// demos
#include "xml_interface.h"
#include "XmlException.h"



using namespace std;
using namespace rlf_tlog;

namespace examples {



   void ubuntu32_creator_generate() ;
   void ubuntu32_create_test();


   const std::string xmlTest = "ubuntu32.xml";
   const std::string xmlPrintOut = "ubuntu32_print.xml";
   const std::string xmlTestOut = "ubuntu32_out.xml";
   const std::string xmlKeysOut = "ubuntu32_keys.txt";


   /*
    *
    Demo so ausführen:
    -- ubuntu32_create_test() auskommentieren
    -- ubuntu32_creator_generate() ausführen,
    -- den Inhalt von ubuntu32_keys.txt in die Funktion ubuntu32_create_test() kopierten
    -- Kommentar an ubuntu32_create_test() entfernen  und starten
    -- bei Bedarf den generierten Teil anpassen

    -- an LOGT_A_INFO( "" ); kann man Breakpoints des Debuggers setzen.

    **/


   void ubuntu32_xml_demo() {
      ubuntu32_creator_generate() ;
      ubuntu32_create_test();
   }

   void ubuntu32_creator_generate() {

      std::string w;
      // make an instance of the Xml interface
      xmlinterface::tXmlInterface t;

      try {

         // parse first
         t.parse( xmlTest );

         // save to disk with implicit pretty printer
         t.save( xmlPrintOut );

         // generate the keylist of all nodes as const std::string
         t.print_creator( xmlKeysOut, xmlTestOut );

      } catch( txml::XmlException& ex ) {
         w = ex.What();
         LOGT_A_INFO( ex.what() );
         LOGT_A_INFO( ex.file_line_method() );
      }

      LOGT_A_INFO( "" );

   }

   void ubuntu32_create_test() {

      // hier den generierten Code aus ubuntu32_keys.txt einfügen


      LOGT_A_INFO( "" );
   }





} // end of ns examples




