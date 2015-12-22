/**************************************************
 * Simple pass-through serial flash programmer 
 * programming the ESP8266 by an Arduino
 * 
 * This resembles the comfortable DTS controlled 
 * programming mode one have with an FTDI or similiar
 * serial connection cable, where no manual reset of
 * the ESP is needed to upload code and run it.
 * Unfortunately there is no RTS/DTS control on the
 * Arduino Serial library, so we solely rely on timing.
 *
 * If the esptool does not wait or retry long enough, 
 * you have to press the reset button
 *
 * Transmissions from the ESP are passed without any
 * modification.
 *
 * XMN: Patch issu de https://github.com/itead/ITEADLIB_Arduino_WeeESP8266/issues/36
 * SoftwareSerial mySerial(3, 2), so:
 * 
 * pin3 (software RX) --> esp8266 TX
 * pin2 (software TX) --> esp8266 RX,
 ***************************************************/


/*
* connection table:
 * ESP8266  Cactus Micro Rev2
 * GPIO0    12
 * ENABLE   13
 * RX       TX
 * TX       RX
 * GND      GND
 * 
 */
#include "ESP8266.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // uno sw RX(pin3) --> esp8266 TX, uno sw TX(pin2) --> esp8266 RX
ESP8266 wifi(mySerial);

int program_pin = 12;
int enable_pin = 13;

void setup()
{
  mySerial.begin(115000);
  Serial.begin(115000);
  pinMode(enable_pin, OUTPUT);
  pinMode(program_pin, OUTPUT);
  digitalWrite(program_pin, LOW);
  digitalWrite(enable_pin,HIGH);

  //while(!Serial);

  Serial.println("ESP8266 programmer ready.");
}

void loop()
{
  // pass data from ESP to host, if any
  while(mySerial.available())
  {
    Serial.write((uint8_t)mySerial.read());
  }

  // pass data from host to ESP, if any
  if(Serial.available())
  {
    while(Serial.available())
    {
      mySerial.write((uint8_t)Serial.read());
    }
  }
}


