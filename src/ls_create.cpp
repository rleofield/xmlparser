
/*
  test SVS Vistek

*/

#include <iostream>

#include <tLog_Category_A.h>

// demos
#include "xml_interface.h"
#include "xml_exception.h"



using namespace std;
using namespace rlf_tlog;

namespace examples {



   void ls_creator_generate() ;
   void ls_create_test();


   const std::string xmlTest = "ls.xml";
   const std::string xmlPrintOut = "ls_print.xml";
   const std::string xmlTestOut = "ls_out.xml";
   const std::string xmlKeysOut = "ls_keys.txt";


   /*
    *
    Demo so ausführen:
    -- ls_create_test() auskommentieren
    -- ls_creator_generate() ausführen,
    -- den Inhalt von ls_keys.txt in die Funktion ls_create_test() kopierten
    -- Kommentar an ls_create_test() entfernen  und starten
    -- bei Bedarf den generierten Teil anpassen

    -- an LOGT_A_INFO( "" ); kann man Breakpoints des Debuggers setzen.

    **/


   void ls_xml_demo() {
      ls_creator_generate() ;
      ls_create_test();
   }

   void ls_creator_generate() {

      std::string w;
      // make an instance of the Xml interface

      try {
         xmlinterface::tXmlInterface t;

         // parse first
         t.parse( xmlTest );

         // save to disk with implicit pretty printer
         t.save( xmlPrintOut );

         // generate the keylist of all nodes as const std::string
         t.print_creator( xmlKeysOut, xmlTestOut );

      } catch( txml::xml_exception& ex ) {
         w = ex.What();
         LOGT_A_INFO( ex.what() );
      }

      LOGT_A_INFO( "" );

   }

   void ls_create_test() {

      {
         // hier den generierten Code aus ls_keys.txt einfügen





      }

      LOGT_A_INFO( "" );
   }





} // end of ns examples



