
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
         LOGT_INFO( static_cast<std::string>( ex ) );
      }

      LOGT_INFO( "" );

   }

   void ubuntu32_create_test() {

      // hier den generierten Code aus ubuntu32_keys.txt einfügen

      const string domain_name = "domain.name";  //   element:    = 'ubuntu32'
      const string domain_uuid = "domain.uuid";  //   element:    = '8aa438a7-07da-f904-6c53-96adf2c6188b'
      const string domain_memory = "domain.memory";  //   element:    = '4194304'
      const string domain_currentMemory = "domain.currentMemory";  //   element:    = '4194304'
      const string domain_vcpu = "domain.vcpu";  //   element:    = '4'
      const string domain_os = "domain.os";  //   element:    = ''
      const string domain_os_type = "domain.os.type";  //   element:    = 'hvm'
      const string domain_os_type__arch = "domain.os.type:arch";  //   attribute:  = 'i686'
      const string domain_os_type__machine = "domain.os.type:machine";  //   attribute:  = 'pc-1.0'
      const string domain_os_boot = "domain.os.boot";  //   element:    = ''
      const string domain_os_boot__dev = "domain.os.boot:dev";  //   attribute:  = 'hd'
      const string domain_features = "domain.features";  //   element:    = ''
      const string domain_features_acpi = "domain.features.acpi";  //   element:    = ''
      const string domain_features_apic = "domain.features.apic";  //   element:    = ''
      const string domain_features_pae = "domain.features.pae";  //   element:    = ''
      const string domain_clock = "domain.clock";  //   element:    = ''
      const string domain_clock__offset = "domain.clock:offset";  //   attribute:  = 'utc'
      const string domain_on_poweroff = "domain.on_poweroff";  //   element:    = 'destroy'
      const string domain_on_reboot = "domain.on_reboot";  //   element:    = 'restart'
      const string domain_on_crash = "domain.on_crash";  //   element:    = 'restart'
      const string domain_devices = "domain.devices";  //   element:    = ''
      const string domain_devices_emulator = "domain.devices.emulator";  //   element:    = '/usr/bin/kvm'
      const string domain_devices_disk = "domain.devices.disk";  //   element:    = ''
      const string domain_devices_disk__type = "domain.devices.disk:type";  //   attribute:  = 'file'
      const string domain_devices_disk__device = "domain.devices.disk:device";  //   attribute:  = 'disk'
      const string domain_devices_disk_driver = "domain.devices.disk.driver";  //   element:    = ''
      const string domain_devices_disk_driver__name = "domain.devices.disk.driver:name";  //   attribute:  = 'qemu'
      const string domain_devices_disk_driver__type = "domain.devices.disk.driver:type";  //   attribute:  = 'raw'
      const string domain_devices_disk_source = "domain.devices.disk.source";  //   element:    = ''
      const string domain_devices_disk_source__file = "domain.devices.disk.source:file";  //   attribute:  = '/home/qemu/ubuntu32/ubuntu32.raw'
      const string domain_devices_disk_target = "domain.devices.disk.target";  //   element:    = ''
      const string domain_devices_disk_target__dev = "domain.devices.disk.target:dev";  //   attribute:  = 'vda'
      const string domain_devices_disk_target__bus = "domain.devices.disk.target:bus";  //   attribute:  = 'virtio'
      const string domain_devices_disk_address = "domain.devices.disk.address";  //   element:    = ''
      const string domain_devices_disk_address__type = "domain.devices.disk.address:type";  //   attribute:  = 'pci'
      const string domain_devices_disk_address__domain = "domain.devices.disk.address:domain";  //   attribute:  = '0x0000'
      const string domain_devices_disk_address__bus = "domain.devices.disk.address:bus";  //   attribute:  = '0x00'
      const string domain_devices_disk_address__slot = "domain.devices.disk.address:slot";  //   attribute:  = '0x05'
      const string domain_devices_disk_address__function = "domain.devices.disk.address:function";  //   attribute:  = '0x0'
      const string domain_devices_controller = "domain.devices.controller";  //   element:    = ''
      const string domain_devices_controller__type = "domain.devices.controller:type";  //   attribute:  = 'virtio-serial'
      const string domain_devices_controller__index = "domain.devices.controller:index";  //   attribute:  = '0'
      const string domain_devices_controller_address = "domain.devices.controller.address";  //   element:    = ''
      const string domain_devices_controller_address__type = "domain.devices.controller.address:type";  //   attribute:  = 'pci'
      const string domain_devices_controller_address__domain = "domain.devices.controller.address:domain";  //   attribute:  = '0x0000'
      const string domain_devices_controller_address__bus = "domain.devices.controller.address:bus";  //   attribute:  = '0x00'
      const string domain_devices_controller_address__slot = "domain.devices.controller.address:slot";  //   attribute:  = '0x08'
      const string domain_devices_controller_address__function = "domain.devices.controller.address:function";  //   attribute:  = '0x0'
      const string domain_devices_interface = "domain.devices.interface";  //   element:    = ''
      const string domain_devices_interface__type = "domain.devices.interface:type";  //   attribute:  = 'bridge'
      const string domain_devices_interface_mac = "domain.devices.interface.mac";  //   element:    = ''
      const string domain_devices_interface_mac__address = "domain.devices.interface.mac:address";  //   attribute:  = '52:54:00:6c:77:77'
      const string domain_devices_interface_source = "domain.devices.interface.source";  //   element:    = ''
      const string domain_devices_interface_source__bridge = "domain.devices.interface.source:bridge";  //   attribute:  = 'br0'
      const string domain_devices_interface_model = "domain.devices.interface.model";  //   element:    = ''
      const string domain_devices_interface_model__type = "domain.devices.interface.model:type";  //   attribute:  = 'virtio'
      const string domain_devices_interface_address = "domain.devices.interface.address";  //   element:    = ''
      const string domain_devices_interface_address__type = "domain.devices.interface.address:type";  //   attribute:  = 'pci'
      const string domain_devices_interface_address__domain = "domain.devices.interface.address:domain";  //   attribute:  = '0x0000'
      const string domain_devices_interface_address__bus = "domain.devices.interface.address:bus";  //   attribute:  = '0x00'
      const string domain_devices_interface_address__slot = "domain.devices.interface.address:slot";  //   attribute:  = '0x03'
      const string domain_devices_interface_address__function = "domain.devices.interface.address:function";  //   attribute:  = '0x0'
      const string domain_devices_interface__1 = "domain.devices.interface[1]";  //   element:    = ''
      const string domain_devices_interface__1__type = "domain.devices.interface[1]:type";  //   attribute:  = 'bridge'
      const string domain_devices_interface__1_mac = "domain.devices.interface[1].mac";  //   element:    = ''
      const string domain_devices_interface__1_mac__address = "domain.devices.interface[1].mac:address";  //   attribute:  = '52:54:00:01:96:02'
      const string domain_devices_interface__1_source = "domain.devices.interface[1].source";  //   element:    = ''
      const string domain_devices_interface__1_source__bridge = "domain.devices.interface[1].source:bridge";  //   attribute:  = 'br1'
      const string domain_devices_interface__1_address = "domain.devices.interface[1].address";  //   element:    = ''
      const string domain_devices_interface__1_address__type = "domain.devices.interface[1].address:type";  //   attribute:  = 'pci'
      const string domain_devices_interface__1_address__domain = "domain.devices.interface[1].address:domain";  //   attribute:  = '0x0000'
      const string domain_devices_interface__1_address__bus = "domain.devices.interface[1].address:bus";  //   attribute:  = '0x00'
      const string domain_devices_interface__1_address__slot = "domain.devices.interface[1].address:slot";  //   attribute:  = '0x07'
      const string domain_devices_interface__1_address__function = "domain.devices.interface[1].address:function";  //   attribute:  = '0x0'
      const string domain_devices_serial = "domain.devices.serial";  //   element:    = ''
      const string domain_devices_serial__type = "domain.devices.serial:type";  //   attribute:  = 'pty'
      const string domain_devices_serial_target = "domain.devices.serial.target";  //   element:    = ''
      const string domain_devices_serial_target__port = "domain.devices.serial.target:port";  //   attribute:  = '0'
      const string domain_devices_console = "domain.devices.console";  //   element:    = ''
      const string domain_devices_console__type = "domain.devices.console:type";  //   attribute:  = 'pty'
      const string domain_devices_console_target = "domain.devices.console.target";  //   element:    = ''
      const string domain_devices_console_target__type = "domain.devices.console.target:type";  //   attribute:  = 'serial'
      const string domain_devices_console_target__port = "domain.devices.console.target:port";  //   attribute:  = '0'
      const string domain_devices_input = "domain.devices.input";  //   element:    = ''
      const string domain_devices_input__type = "domain.devices.input:type";  //   attribute:  = 'mouse'
      const string domain_devices_input__bus = "domain.devices.input:bus";  //   attribute:  = 'ps2'
      const string domain_devices_graphics = "domain.devices.graphics";  //   element:    = ''
      const string domain_devices_graphics__type = "domain.devices.graphics:type";  //   attribute:  = 'vnc'
      const string domain_devices_graphics__port = "domain.devices.graphics:port";  //   attribute:  = '-1'
      const string domain_devices_graphics__autoport = "domain.devices.graphics:autoport";  //   attribute:  = 'yes'
      const string domain_devices_sound = "domain.devices.sound";  //   element:    = ''
      const string domain_devices_sound__model = "domain.devices.sound:model";  //   attribute:  = 'ich6'
      const string domain_devices_sound_address = "domain.devices.sound.address";  //   element:    = ''
      const string domain_devices_sound_address__type = "domain.devices.sound.address:type";  //   attribute:  = 'pci'
      const string domain_devices_sound_address__domain = "domain.devices.sound.address:domain";  //   attribute:  = '0x0000'
      const string domain_devices_sound_address__bus = "domain.devices.sound.address:bus";  //   attribute:  = '0x00'
      const string domain_devices_sound_address__slot = "domain.devices.sound.address:slot";  //   attribute:  = '0x04'
      const string domain_devices_sound_address__function = "domain.devices.sound.address:function";  //   attribute:  = '0x0'
      const string domain_devices_video = "domain.devices.video";  //   element:    = ''
      const string domain_devices_video_model = "domain.devices.video.model";  //   element:    = ''
      const string domain_devices_video_model__type = "domain.devices.video.model:type";  //   attribute:  = 'cirrus'
      const string domain_devices_video_model__vram = "domain.devices.video.model:vram";  //   attribute:  = '9216'
      const string domain_devices_video_model__heads = "domain.devices.video.model:heads";  //   attribute:  = '1'
      const string domain_devices_video_address = "domain.devices.video.address";  //   element:    = ''
      const string domain_devices_video_address__type = "domain.devices.video.address:type";  //   attribute:  = 'pci'
      const string domain_devices_video_address__domain = "domain.devices.video.address:domain";  //   attribute:  = '0x0000'
      const string domain_devices_video_address__bus = "domain.devices.video.address:bus";  //   attribute:  = '0x00'
      const string domain_devices_video_address__slot = "domain.devices.video.address:slot";  //   attribute:  = '0x02'
      const string domain_devices_video_address__function = "domain.devices.video.address:function";  //   attribute:  = '0x0'
      const string domain_devices_memballoon = "domain.devices.memballoon";  //   element:    = ''
      const string domain_devices_memballoon__model = "domain.devices.memballoon:model";  //   attribute:  = 'virtio'
      const string domain_devices_memballoon_address = "domain.devices.memballoon.address";  //   element:    = ''
      const string domain_devices_memballoon_address__type = "domain.devices.memballoon.address:type";  //   attribute:  = 'pci'
      const string domain_devices_memballoon_address__domain = "domain.devices.memballoon.address:domain";  //   attribute:  = '0x0000'
      const string domain_devices_memballoon_address__bus = "domain.devices.memballoon.address:bus";  //   attribute:  = '0x00'
      const string domain_devices_memballoon_address__slot = "domain.devices.memballoon.address:slot";  //   attribute:  = '0x06'
      const string domain_devices_memballoon_address__function = "domain.devices.memballoon.address:function";  //   attribute:  = '0x0'


      xmlinterface::tXmlInterface instance;
      instance.parse( xmlTestOut );
      instance.create( domain_name, "ubuntu32" );
      instance.create( domain_uuid, "8aa438a7-07da-f904-6c53-96adf2c6188b" );
      instance.create( domain_memory, "4194304" );
      instance.create( domain_currentMemory, "4194304" );
      instance.create( domain_vcpu, "4" );
      instance.create( domain_os, "" );
      instance.create( domain_os_type, "hvm" );
      instance.create( domain_os_type__arch, "i686" );
      instance.create( domain_os_type__machine, "pc-1.0" );
      instance.create( domain_os_boot, "" );
      instance.create( domain_os_boot__dev, "hd" );
      instance.create( domain_features, "" );
      instance.create( domain_features_acpi, "" );
      instance.create( domain_features_apic, "" );
      instance.create( domain_features_pae, "" );
      instance.create( domain_clock, "" );
      instance.create( domain_clock__offset, "utc" );
      instance.create( domain_on_poweroff, "destroy" );
      instance.create( domain_on_reboot, "restart" );
      instance.create( domain_on_crash, "restart" );
      instance.create( domain_devices, "" );
      instance.create( domain_devices_emulator, "/usr/bin/kvm" );
      instance.create( domain_devices_disk, "" );
      instance.create( domain_devices_disk__type, "file" );
      instance.create( domain_devices_disk__device, "disk" );
      instance.create( domain_devices_disk_driver, "" );
      instance.create( domain_devices_disk_driver__name, "qemu" );
      instance.create( domain_devices_disk_driver__type, "raw" );
      instance.create( domain_devices_disk_source, "" );
      instance.create( domain_devices_disk_source__file, "/home/qemu/ubuntu32/ubuntu32.raw" );
      instance.create( domain_devices_disk_target, "" );
      instance.create( domain_devices_disk_target__dev, "vda" );
      instance.create( domain_devices_disk_target__bus, "virtio" );
      instance.create( domain_devices_disk_address, "" );
      instance.create( domain_devices_disk_address__type, "pci" );
      instance.create( domain_devices_disk_address__domain, "0x0000" );
      instance.create( domain_devices_disk_address__bus, "0x00" );
      instance.create( domain_devices_disk_address__slot, "0x05" );
      instance.create( domain_devices_disk_address__function, "0x0" );
      instance.create( domain_devices_controller, "" );
      instance.create( domain_devices_controller__type, "virtio-serial" );
      instance.create( domain_devices_controller__index, "0" );
      instance.create( domain_devices_controller_address, "" );
      instance.create( domain_devices_controller_address__type, "pci" );
      instance.create( domain_devices_controller_address__domain, "0x0000" );
      instance.create( domain_devices_controller_address__bus, "0x00" );
      instance.create( domain_devices_controller_address__slot, "0x08" );
      instance.create( domain_devices_controller_address__function, "0x0" );
      instance.create( domain_devices_interface, "" );
      instance.create( domain_devices_interface__type, "bridge" );
      instance.create( domain_devices_interface_mac, "" );
      instance.create( domain_devices_interface_mac__address, "52:54:00:6c:77:77" );
      instance.create( domain_devices_interface_source, "" );
      instance.create( domain_devices_interface_source__bridge, "br0" );
      instance.create( domain_devices_interface_model, "" );
      instance.create( domain_devices_interface_model__type, "virtio" );
      instance.create( domain_devices_interface_address, "" );
      instance.create( domain_devices_interface_address__type, "pci" );
      instance.create( domain_devices_interface_address__domain, "0x0000" );
      instance.create( domain_devices_interface_address__bus, "0x00" );
      instance.create( domain_devices_interface_address__slot, "0x03" );
      instance.create( domain_devices_interface_address__function, "0x0" );
      instance.create( domain_devices_interface__1, "" );
      instance.create( domain_devices_interface__1__type, "bridge" );
      instance.create( domain_devices_interface__1_mac, "" );
      instance.create( domain_devices_interface__1_mac__address, "52:54:00:01:96:02" );
      instance.create( domain_devices_interface__1_source, "" );
      instance.create( domain_devices_interface__1_source__bridge, "br1" );
      instance.create( domain_devices_interface__1_address, "" );
      instance.create( domain_devices_interface__1_address__type, "pci" );
      instance.create( domain_devices_interface__1_address__domain, "0x0000" );
      instance.create( domain_devices_interface__1_address__bus, "0x00" );
      instance.create( domain_devices_interface__1_address__slot, "0x07" );
      instance.create( domain_devices_interface__1_address__function, "0x0" );
      instance.create( domain_devices_serial, "" );
      instance.create( domain_devices_serial__type, "pty" );
      instance.create( domain_devices_serial_target, "" );
      instance.create( domain_devices_serial_target__port, "0" );
      instance.create( domain_devices_console, "" );
      instance.create( domain_devices_console__type, "pty" );
      instance.create( domain_devices_console_target, "" );
      instance.create( domain_devices_console_target__type, "serial" );
      instance.create( domain_devices_console_target__port, "0" );
      instance.create( domain_devices_input, "" );
      instance.create( domain_devices_input__type, "mouse" );
      instance.create( domain_devices_input__bus, "ps2" );
      instance.create( domain_devices_graphics, "" );
      instance.create( domain_devices_graphics__type, "vnc" );
      instance.create( domain_devices_graphics__port, "-1" );
      instance.create( domain_devices_graphics__autoport, "yes" );
      instance.create( domain_devices_sound, "" );
      instance.create( domain_devices_sound__model, "ich6" );
      instance.create( domain_devices_sound_address, "" );
      instance.create( domain_devices_sound_address__type, "pci" );
      instance.create( domain_devices_sound_address__domain, "0x0000" );
      instance.create( domain_devices_sound_address__bus, "0x00" );
      instance.create( domain_devices_sound_address__slot, "0x04" );
      instance.create( domain_devices_sound_address__function, "0x0" );
      instance.create( domain_devices_video, "" );
      instance.create( domain_devices_video_model, "" );
      instance.create( domain_devices_video_model__type, "cirrus" );
      instance.create( domain_devices_video_model__vram, "9216" );
      instance.create( domain_devices_video_model__heads, "1" );
      instance.create( domain_devices_video_address, "" );
      instance.create( domain_devices_video_address__type, "pci" );
      instance.create( domain_devices_video_address__domain, "0x0000" );
      instance.create( domain_devices_video_address__bus, "0x00" );
      instance.create( domain_devices_video_address__slot, "0x02" );
      instance.create( domain_devices_video_address__function, "0x0" );
      instance.create( domain_devices_memballoon, "" );
      instance.create( domain_devices_memballoon__model, "virtio" );
      instance.create( domain_devices_memballoon_address, "" );
      instance.create( domain_devices_memballoon_address__type, "pci" );
      instance.create( domain_devices_memballoon_address__domain, "0x0000" );
      instance.create( domain_devices_memballoon_address__bus, "0x00" );
      instance.create( domain_devices_memballoon_address__slot, "0x06" );
      instance.create( domain_devices_memballoon_address__function, "0x0" );
      instance.save();

      LOGT_INFO( "" );
   }





} // end of ns examples




