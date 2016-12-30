// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html
// 0x28, 0xFF, 0x52, 0x5E, 0x81, 0x16, 0x05, 0x23 -> 1
// 0x28, 0xFF, 0xCD, 0x38, 0x81, 0x16, 0x05, 0x62 -> 2
#include <OneWire.h>

OneWire  ds(7);  // Connect your 1-wire device to pin 3

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
}
