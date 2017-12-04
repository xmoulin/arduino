/**
 * Logo SSG
 */
 
#include <SoftwareSerial.h>
#include <Minitel.h>

Minitel m;

PROGMEM const byte c[]={32,32,32,32,32,32,32,32,32,32,32,32,32,32,64,80,88,92,94,95,95,95,95,95,95,95,95,95,95,95,95,95,95,93,84,48,32,32,32,32,32,64,80,80,80,80,80,80,80,80,64,80,92,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,84,32,32,32,88,95,95,95,95,95,95,95,95,95,63,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,53,32,32,95,95,95,95,95,95,95,95,63,33,32,32,34,35,35,47,79,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,32,32,95,95,95,95,95,95,95,95,32,32,32,32,32,32,32,32,32,34,35,47,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,53,72,79,95,95,95,95,95,95,95,32,32,32,32,32,32,32,32,32,32,32,32,32,34,35,47,95,95,95,95,95,95,95,95,95,95,95,95,95,95,53,42,32,43,95,95,95,95,95,95,48,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,43,47,95,95,95,95,95,95,95,95,95,95,37,32,32,32,34,79,95,95,95,95,93,48,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,34,35,47,79,95,95,95,95,39,32,72,32,32,32,32,34,47,95,95,95,93,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,33,32,32,34,32,32,32,32,32,32,32,43,79,95,93,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,40,95,93,80,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,43,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,42,95,95,95,84,48,32,32,32,32,32,32,32,64,94,95,93,92,84,48,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,42,95,95,95,95,93,48,32,32,32,32,64,95,95,95,95,95,95,95,95,92,84,80,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,42,95,95,95,95,95,84,48,32,32,74,95,95,95,95,95,95,95,95,95,95,95,95,93,84,48,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,95,95,95,95,95,95,95,52,32,74,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,93,92,80,80,32,32,32,32,32,32,32,32,32,32,32,95,95,95,95,95,95,95,93,32,42,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,92,84,48,32,32,32,32,32,32,88,95,95,95,95,95,95,95,95,32,32,79,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,92,92,84,88,94,95,95,95,95,95,95,95,95,63,32,32,42,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,63,47,47,47,95,95,95,95,95,47,39,32,32,32,32,34,79,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,63,39,35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,34,35,35,47,47,47,47,47,47,47,47,47,47,47,35,35,35,33,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32};

void setup() {
}

void loop() {
  m.graphicMode();
  m.clearScreen();
  
  for (int i=0; i< sizeof(c); i++) {
    m.textByte(c[i]);
  }
  delay(10000);
} 

