
#include <iostream>

#include <tLog_Category_A.h>

// demos
#include "xml_interface.h"
#include "xml_exception.h"

using namespace std;
using namespace rlf_tlog;

namespace examples {

   void eco_creator_generate() ;
   void eco_create_test();


   const std::string xmlTest = "eco.xml";
   const std::string xmlTestPrint = "eco_print.xml";
   const std::string xmlTestOut = "eco_out.xml";
   const std::string xmlKeysOut = "eco_keys.txt";



   /*
    *
    Demo so ausführen:
    -- eco_create_test() auskommentieren
    -- eco_creator_generate() ausführen,
    -- den Inhalt von eco_keys.txt in die Funktion eco_create_test() kopierten
    -- Kommentar an eco_create_test() entfernen  und starten, läuft sehr lange
    -- bei Bedarf den generierten Teil anpassen

    -- an LOGT_A_INFO( "" ); kann man Breakpoints des Debuggers setzen.
    **/

   void eco_xml_demo() {
      eco_creator_generate() ;
      eco_create_test();
   }
   std::string w ;

   void eco_creator_generate() {

      // make an instance of the Xml interface
      xmlinterface::tXmlInterface t;

      try {

         // parse first
         t.parse( xmlTest );

         // save to disk with implicit pretty printer
         t.save( xmlTestPrint );

         // generate the keylist of all nodes as const std::string
         t.print_creator( xmlKeysOut, xmlTestOut );

      } catch( txml::xml_exception& ex ) {
         w = ex.What();
         LOGT_A_INFO( ex.what() );
      }

      LOGT_A_INFO( "" );


   }

   void eco_create_test() {



      try {

         // hier den generierten Code aus eco_keys.txt einfügen


      } catch( txml::xml_exception& ex ) {
         w = ex.What();
         LOGT_A_INFO( ex.what() );
      }


      LOGT_A_INFO( "" );

   }


}

//EOF


