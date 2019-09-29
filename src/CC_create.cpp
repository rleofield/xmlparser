
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



   void CC_creator_generate() ;
   void CC_create_test();


   const std::string xmlTest = "CC.xml";
   const std::string xmlTestPrint = "CC_print.xml";
   const std::string xmlTestOut = "CC_out.xml";
   const std::string xmlKeysOut = "CC_keys.txt";


   /*
    *
    Demo so ausführen:
    -- CC_create_test() auskommentieren
    -- CC_creator_generate() ausführen,
    -- den Inhalt von CC_keys.txt in die Funktion CC_create_test() kopieren
    -- Kommentar an CC_create_test() entfernen  und starten
    -- bei Bedarf den generierten Teil anpassen

    -- an LOGT_A_INFO( "" ); kann man Breakpoints des Debuggers setzen.

    **/


   void CC_xml_demo() {
      CC_creator_generate() ;
      CC_create_test();
   }

   void CC_creator_generate() {

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
         std::string w = ex.What();
         LOGT_A_INFO( ex.what() );
      }

      LOGT_A_INFO( "" );

   }

   void CC_create_test() {

      // hier den generierten Code aus CC_keys.txt einfügen

      // einfügen end

      // LOG Zeile für einen Breakpoint
      LOGT_A_INFO( "" );
   }





} // end of ns examples



