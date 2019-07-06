// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html
// 0x28, 0xFF, 0x52, 0x5E, 0x81, 0x16, 0x05, 0x23 -> 1
// 0x28, 0xFF, 0xCD, 0x38, 0x81, 0x16, 0x05, 0x62 -> 2
// 0x28, 0xFF, 0xA8, 0x5F, 0xA1, 0x16, 0x05, 0x23 -> 3 - Ancien Ballon
// 0x28, 0xFF, 0x12, 0x1B, 0xA1, 0x16, 0x04, 0x61 -> 4 - nouveau Ballon
// 0x28, 0xFF, 0x67, 0xEE, 0xA0, 0x16, 0x04, 0x85 -> Piscine
#include <OneWire.h>

OneWire  ds(4);  // Connect your 1-wire device to pin 7

void setup(void) {
  Serial.begin(9600);
  discoverOneWireDevices();
}

void discoverOneWireDevices(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  
  Serial.print("Looking for 1-Wire devices...\n\r");
  while(ds.search(addr)) {
    Serial.print("\n\rFound \'1-Wire\' device with address:\n\r");

    for( i = 0; i < 8; i++) {
      Serial.print("0x");
      if (addr[i] < 16) {
        Serial.print('0');
      }
      Serial.print(addr[i], HEX);

      if (i < 7) {
        Serial.print(", ");
      }
    }
    if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.print("CRC is not valid!\n");
        return;
    }
  }
  Serial.print("\n\r\n\rThat's it.\r\n");
  ds.reset_search();
  return;
}

void loop(void) {
  // nothing to see here
  //discoverOneWireDevices();
  //delay(10000);
}
