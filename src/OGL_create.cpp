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

   void OGL_creator_generate() ;
   void OGL_create_test();


   const std::string xmlTest = "CenterScreen.xml";
   const std::string xmlTestPrint = "CenterScreen_print.xml";
   const std::string xmlTestOut = "CenterScreen_out.xml";
   const std::string xmlKeysOut = "CenterScreen_keys.txt";

   /*
    *
    Demo so ausführen:
    -- OGL_create_test() auskommentieren
    -- OGL_creator_generate ausführen,
    -- den Inhalt von CenterScreen_keys.txt in die Funktion OGL_create_test() kopierten
    -- Kommentar an OGL_create_test() entfernen  und starten
    -- bei Bedarf den generierten Teil anpassen

       -- an LOGT_A_INFO( "" ); kann man Breakpoints des Debuggers setzen.

    **/
   void OGL_xml_demo() {
      OGL_creator_generate() ;
      OGL_create_test();
   }

   void OGL_creator_generate() {

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

   void OGL_create_test() {


      // hier den generierten Code aus OGL_keys.txt einfügen




      // einfügen end

      // LOG Zeile für einen Breakpoint

      LOGT_A_INFO( "" );

   }






} // end of ns examples



