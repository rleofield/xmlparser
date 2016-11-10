
/*

  test

*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>


#include "tLog_Category_default.h"


#include "wList.h"
#include "stringhelper.h"

// demos
#include "xml_interface.h"
#include "xml_exception.h"


#include "xml_builder.h"

using namespace std;
using namespace rlf_tlog;

namespace examples {


   void ubuntu32_creator_generate() ;
   void ubuntu32_create_test();


   //const string xmlTest = "utf8test.xml"; //ubuntu32.xml";
   const string xmlTest = "ubuntu32.xml";
   const string xmlPrintOut = "ubuntu32_print.xml";
   const string xmlTestOut = "ubuntu32_out.xml";
   const string xmlKeysOut = "ubuntu32_keys.txt";


   /*
    *
    Demo so ausführen:
    -- ubuntu32_create_test() auskommentieren
    -- ubuntu32_creator_generate() ausführen,
    -- den Inhalt von ubuntu32_keys.txt in die Funktion ubuntu32_create_test() kopierten
    -- Kommentar an ubuntu32_create_test() entfernen  und starten
    -- bei Bedarf den generierten Teil anpassen

    -- an LOGT_INFO( "" ); kann man Breakpoints des Debuggers setzen.

    **/


   void ubuntu32_xml_demo() {
      ubuntu32_creator_generate() ;
      ubuntu32_create_test();
   }

   void ubuntu32_creator_generate() {

      string w;
      // make an instance of the Xml interface


      try {
         xmlinterface::tXmlInterface t;
         // parse first
         t.parse( xmlTest );

         // save to disk with implicit pretty printer
         t.save( "pretty_print.xml" );

         // generate the keylist of all nodes as const string
         t.print_creator( xmlKeysOut, xmlTestOut );

      } catch( txml::xml_exception& ex ) {
         w = ex.What();
         LOGT_INFO( ex.what() );
         LOGT_INFO( static_cast<std::string>( ex.lfm() ) );
      }

      LOGT_INFO( "" );

   }

   void ubuntu32_create_test() {

      // hier den generierten Code, 2. Teil, aus ubuntu32_keys.txt einfügen
      {



         xmlinterface::tXmlInterface instance;
         instance.parse( xmlTestOut );
         instance.create( "domain.name", "ubuntu32" );
         instance.create( "domain.uuid", "8aa438a7-07da-f904-6c53-96adf2c6188b" );
         instance.create( "domain.memory", "4194304" );
         instance.create( "domain.currentMemory", "4194304" );
         instance.create( "domain.vcpu", "4" );
         instance.create( "domain.os", "" );
         instance.create( "domain.os.boot", "" );
         //instance.create( "domain.os.type", "hvm" );
         instance.  attr( "domain.os.type:arch", "i686" );
         instance.  attr( "domain.os.type:machine", "pc-1.0" );
         instance.create( "domain.os.boot", "" );
         instance.  attr( "domain.os.boot:dev", "hd" );
         instance.create( "domain.features", "" );
         instance.create( "domain.features.acpi", "" );
         instance.create( "domain.features.apic", "" );
         instance.create( "domain.features.pae", "" );
         instance.create( "domain.clock", "" );
         instance.  attr( "domain.clock:offset", "utc" );
         instance.create( "domain.on_poweroff", "destroy" );
         instance.create( "domain.on_reboot", "restart" );
         instance.create( "domain.on_crash", "restart" );
         instance.create( "domain.devices", "" );
         instance.create( "domain.devices.emulator", "/usr/bin/kvm" );
         instance.create( "domain.devices.disk", "" );
         instance.  attr( "domain.devices.disk:type", "file" );
         instance.  attr( "domain.devices.disk:device", "disk" );
         instance.create( "domain.devices.disk.driver", "" );
         instance.  attr( "domain.devices.disk.driver:name", "qemu" );
         instance.  attr( "domain.devices.disk.driver:type", "raw" );
         instance.create( "domain.devices.disk.source", "" );
         instance.  attr( "domain.devices.disk.source:file", "/home/qemu/ubuntu32/ubuntu32.raw" );
         instance.create( "domain.devices.disk.target", "" );
         instance.  attr( "domain.devices.disk.target:dev", "vda" );
         instance.  attr( "domain.devices.disk.target:bus", "virtio" );
         instance.create( "domain.devices.disk.address", "" );
         instance.  attr( "domain.devices.disk.address:type", "pci" );
         instance.  attr( "domain.devices.disk.address:domain", "0x0000" );
         instance.  attr( "domain.devices.disk.address:bus", "0x00" );
         instance.  attr( "domain.devices.disk.address:slot", "0x05" );
         instance.  attr( "domain.devices.disk.address:function", "0x0" );
         instance.create( "domain.devices.controller", "" );
         instance.  attr( "domain.devices.controller:type", "virtio-serial" );
         instance.  attr( "domain.devices.controller:index", "0" );
         instance.create( "domain.devices.controller.address", "" );
         instance.  attr( "domain.devices.controller.address:type", "pci" );
         instance.  attr( "domain.devices.controller.address:domain", "0x0000" );
         instance.  attr( "domain.devices.controller.address:bus", "0x00" );
         instance.  attr( "domain.devices.controller.address:slot", "0x08" );
         instance.  attr( "domain.devices.controller.address:function", "0x0" );
         instance.create( "domain.devices.interface", "" );
         instance.  attr( "domain.devices.interface:type", "bridge" );
         instance.create( "domain.devices.interface.mac", "" );
         instance.  attr( "domain.devices.interface.mac:address", "52:54:00:6c:77:77" );
         instance.create( "domain.devices.interface.source", "" );
         instance.  attr( "domain.devices.interface.source:bridge", "br0" );
         instance.create( "domain.devices.interface.model", "" );
         instance.  attr( "domain.devices.interface.model:type", "virtio" );
         instance.create( "domain.devices.interface.address", "" );
         instance.  attr( "domain.devices.interface.address:type", "pci" );
         instance.  attr( "domain.devices.interface.address:domain", "0x0000" );
         instance.  attr( "domain.devices.interface.address:bus", "0x00" );
         instance.  attr( "domain.devices.interface.address:slot", "0x03" );
         instance.  attr( "domain.devices.interface.address:function", "0x0" );
         instance.create( "domain.devices.interface[1]", "" );
         instance.  attr( "domain.devices.interface[1]:type", "bridge" );
         instance.create( "domain.devices.interface[1].mac", "" );
         instance.  attr( "domain.devices.interface[1].mac:address", "52:54:00:01:96:02" );
         instance.create( "domain.devices.interface[1].source", "" );
         instance.  attr( "domain.devices.interface[1].source:bridge", "br1" );
         instance.create( "domain.devices.interface[1].address", "" );
         instance.  attr( "domain.devices.interface[1].address:type", "pci" );
         instance.  attr( "domain.devices.interface[1].address:domain", "0x0000" );
         instance.  attr( "domain.devices.interface[1].address:bus", "0x00" );
         instance.  attr( "domain.devices.interface[1].address:slot", "0x07" );
         instance.  attr( "domain.devices.interface[1].address:function", "0x0" );
         instance.create( "domain.devices.serial", "" );
         instance.  attr( "domain.devices.serial:type", "pty" );
         instance.create( "domain.devices.serial.target", "" );
         instance.  attr( "domain.devices.serial.target:port", "0" );
         instance.create( "domain.devices.console", "" );
         instance.  attr( "domain.devices.console:type", "pty" );
         instance.create( "domain.devices.console.target", "" );
         instance.  attr( "domain.devices.console.target:type", "serial" );
         instance.  attr( "domain.devices.console.target:port", "0" );
         instance.create( "domain.devices.input", "" );
         instance.  attr( "domain.devices.input:type", "mouse" );
         instance.  attr( "domain.devices.input:bus", "ps2" );
         instance.create( "domain.devices.graphics", "" );
         instance.  attr( "domain.devices.graphics:type", "vnc" );
         instance.  attr( "domain.devices.graphics:port", "-1" );
         instance.  attr( "domain.devices.graphics:autoport", "yes" );
         instance.create( "domain.devices.sound", "" );
         instance.  attr( "domain.devices.sound:model", "ich6" );
         instance.create( "domain.devices.sound.address", "" );
         instance.  attr( "domain.devices.sound.address:type", "pci" );
         instance.  attr( "domain.devices.sound.address:domain", "0x0000" );
         instance.  attr( "domain.devices.sound.address:bus", "0x00" );
         instance.  attr( "domain.devices.sound.address:slot", "0x04" );
         instance.  attr( "domain.devices.sound.address:function", "0x0" );
         instance.create( "domain.devices.video", "" );
         instance.create( "domain.devices.video.model", "" );
         instance.  attr( "domain.devices.video.model:type", "cirrus" );
         instance.  attr( "domain.devices.video.model:vram", "9216" );
         instance.  attr( "domain.devices.video.model:heads", "1" );
         instance.create( "domain.devices.video.address", "" );
         instance.  attr( "domain.devices.video.address:type", "pci" );
         instance.  attr( "domain.devices.video.address:domain", "0x0000" );
         instance.  attr( "domain.devices.video.address:bus", "0x00" );
         instance.  attr( "domain.devices.video.address:slot", "0x02" );
         instance.  attr( "domain.devices.video.address:function", "0x0" );
         instance.create( "domain.devices.memballoon", "" );
         instance.  attr( "domain.devices.memballoon:model", "virtio" );
         instance.create( "domain.devices.memballoon.address", "" );
         instance.  attr( "domain.devices.memballoon.address:type", "pci" );
         instance.  attr( "domain.devices.memballoon.address:domain", "0x0000" );
         instance.  attr( "domain.devices.memballoon.address:bus", "0x00" );
         instance.  attr( "domain.devices.memballoon.address:slot", "0x06" );
         instance.  attr( "domain.devices.memballoon.address:function", "0x0" );
         instance.save();

         // get element text
         string suuid = instance.get_string( "domain.uuid" );

         // get attribute
         string adresstype = instance.get_string( "domain.devices.interface.address:type" );



         txml::xml_node const* n =  instance.document();

         std::ofstream fp( "ubuntu32_dump.txt", std::ios_base::app );

         if( fp.bad() ) {
            string s = "logfile: ";// + _file + " not found." ;
            std::cout << s << std::endl;
            return ;
         }

         std::ostringstream o;
         demo::node_dump( o, instance.document(), 2 );
         string ret = o.str();
         list<string> text;
         text = rlf_hstring::string_to_list( ret, static_cast<char>(0) );

         bool overwrite = true;

         try {
            rlf_txtrw::t_write_text()( "ubuntu32_dump.txt", text, overwrite );
         } catch( rlf_txtrw::bad_text_write& ex ) {
            string w =  ex.what();
            LOGT_ERROR( "ex: " + w );
            return ;
         }



      }
      LOGT_INFO( "" );
   }

} // end of ns examples




