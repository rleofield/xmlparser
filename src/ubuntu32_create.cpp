
/*

  test

*/

#include <iostream>


#include <tLog_Category_default.h>

// demos
#include "xml_interface.h"
#include "xml_exception.h"



using namespace std;
using namespace rlf_tlog;

namespace examples {


   void ubuntu32_creator_generate() ;
   void ubuntu32_create_test();


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
         t.save( xmlPrintOut );

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
         instance.create( "domain.os.type", "hvm" );
         instance.create( "domain.os.type:arch", "i686ä" );
         instance.create( "domain.os.type:arch", "i686" );

         instance.create( "domain.os.type:machine", "pc-1.0" );
         instance.create( "domain.os.boot", "" );
         instance.create( "domain.os.boot:dev", "hd" );
         instance.create( "domain.features", "" );
         instance.create( "domain.features.acpi", "" );
         instance.create( "domain.features.apic", "" );
         instance.create( "domain.features.pae", "" );
         instance.create( "domain.clock", "" );
         instance.create( "domain.clock:offset", "set_element_attribute_values(), test, new test clock, seond try" );
         instance.create( "domain.on_poweroff", "destroy" );
         instance.create( "domain.on_reboot", "restart" );
         instance.create( "domain.on_crash", "restart" );
         instance.create( "domain.devices", "" );
         instance.create( "domain.devices.emulator", "/usr/bin/kvm" );
         instance.create( "domain.devices.disk", "" );
         instance.create( "domain.devices.disk:type", "file" );
         instance.create( "domain.devices.disk:device", "disk" );
         instance.create( "domain.devices.disk.driver", "" );
         instance.create( "domain.devices.disk.driver:name", "qemu" );
         instance.create( "domain.devices.disk.driver:type", "raw" );
         instance.create( "domain.devices.disk.source", "" );
         instance.create( "domain.devices.disk.source:file", "/home/qemu/ubuntu32/ubuntu32.raw" );
         instance.create( "domain.devices.disk.target", "" );
         instance.create( "domain.devices.disk.target:dev", "vda" );
         instance.create( "domain.devices.disk.target:bus", "virtio" );
         instance.create( "domain.devices.disk.address", "" );
         instance.create( "domain.devices.disk.address:type", "pci" );
         instance.create( "domain.devices.disk.address:domain", "0x0000" );
         instance.create( "domain.devices.disk.address:bus", "0x00" );
         instance.create( "domain.devices.disk.address:slot", "0x05" );
         instance.create( "domain.devices.disk.address:function", "0x0" );
         instance.create( "domain.devices.controller", "" );
         instance.create( "domain.devices.controller:type", "virtio-serial" );
         instance.create( "domain.devices.controller:index", "0" );
         instance.create( "domain.devices.controller.address", "" );
         instance.create( "domain.devices.controller.address:type", "pci" );
         instance.create( "domain.devices.controller.address:domain", "0x0000" );
         instance.create( "domain.devices.controller.address:bus", "0x00" );
         instance.create( "domain.devices.controller.address:slot", "0x08" );
         instance.create( "domain.devices.controller.address:function", "0x0" );
         instance.create( "domain.devices.interface", "" );
         instance.create( "domain.devices.interface:type", "bridge" );
         instance.create( "domain.devices.interface.mac", "" );
         instance.create( "domain.devices.interface.mac:address", "52:54:00:6c:77:77" );
         instance.create( "domain.devices.interface.source", "" );
         instance.create( "domain.devices.interface.source:bridge", "br0" );
         instance.create( "domain.devices.interface.model", "" );
         instance.create( "domain.devices.interface.model:type", "virtio" );
         instance.create( "domain.devices.interface.address", "" );
         instance.create( "domain.devices.interface.address:type", "pci" );
         instance.create( "domain.devices.interface.address:domain", "0x0000" );
         instance.create( "domain.devices.interface.address:bus", "0x00" );
         instance.create( "domain.devices.interface.address:slot", "0x03" );
         instance.create( "domain.devices.interface.address:function", "0x0" );
         instance.create( "domain.devices.interface[1]", "" );
         instance.create( "domain.devices.interface[1]:type", "bridge" );
         instance.create( "domain.devices.interface[1].mac", "" );
         instance.create( "domain.devices.interface[1].mac:address", "52:54:00:01:96:02" );
         instance.create( "domain.devices.interface[1].source", "" );
         instance.create( "domain.devices.interface[1].source:bridge", "br1" );
         instance.create( "domain.devices.interface[1].address", "" );
         instance.create( "domain.devices.interface[1].address:type", "pci" );
         instance.create( "domain.devices.interface[1].address:domain", "0x0000" );
         instance.create( "domain.devices.interface[1].address:bus", "0x00" );
         instance.create( "domain.devices.interface[1].address:slot", "0x07" );
         instance.create( "domain.devices.interface[1].address:function", "0x0" );
         instance.create( "domain.devices.serial", "" );
         instance.create( "domain.devices.serial:type", "pty" );
         instance.create( "domain.devices.serial.target", "" );
         instance.create( "domain.devices.serial.target:port", "0" );
         instance.create( "domain.devices.console", "" );
         instance.create( "domain.devices.console:type", "pty" );
         instance.create( "domain.devices.console.target", "" );
         instance.create( "domain.devices.console.target:type", "serial" );
         instance.create( "domain.devices.console.target:port", "0" );
         instance.create( "domain.devices.input", "" );
         instance.create( "domain.devices.input:type", "mouse" );
         instance.create( "domain.devices.input:bus", "ps2" );
         instance.create( "domain.devices.graphics", "" );
         instance.create( "domain.devices.graphics:type", "vnc" );
         instance.create( "domain.devices.graphics:port", "-1" );
         instance.create( "domain.devices.graphics:autoport", "yes" );
         instance.create( "domain.devices.sound", "" );
         instance.create( "domain.devices.sound:model", "ich6" );
         instance.create( "domain.devices.sound.address", "" );
         instance.create( "domain.devices.sound.address:type", "pci" );
         instance.create( "domain.devices.sound.address:domain", "0x0000" );
         instance.create( "domain.devices.sound.address:bus", "0x00" );
         instance.create( "domain.devices.sound.address:slot", "0x04" );
         instance.create( "domain.devices.sound.address:function", "0x0" );
         instance.create( "domain.devices.video", "" );
         instance.create( "domain.devices.video.model", "" );
         instance.create( "domain.devices.video.model:type", "cirrus" );
         instance.create( "domain.devices.video.model:vram", "9216" );
         instance.create( "domain.devices.video.model:heads", "1" );
         instance.create( "domain.devices.video.address", "" );
         instance.create( "domain.devices.video.address:type", "pci" );
         instance.create( "domain.devices.video.address:domain", "0x0000" );
         instance.create( "domain.devices.video.address:bus", "0x00" );
         instance.create( "domain.devices.video.address:slot", "0x02" );
         instance.create( "domain.devices.video.address:function", "0x0" );
         instance.create( "domain.devices.memballoon", "" );
         instance.create( "domain.devices.memballoon:model", "virtio" );
         instance.create( "domain.devices.memballoon.address", "" );
         instance.create( "domain.devices.memballoon.address:type", "pci" );
         instance.create( "domain.devices.memballoon.address:domain", "0x0000" );
         instance.create( "domain.devices.memballoon.address:bus", "0x00" );
         instance.create( "domain.devices.memballoon.address:slot", "0x06" );
         instance.create( "domain.devices.memballoon.address:function", "0x0" );
         instance.save();

      }
      LOGT_INFO( "" );
   }





} // end of ns examples




