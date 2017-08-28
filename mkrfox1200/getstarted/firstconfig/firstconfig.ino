/*
  Init
  https://www.arduino.cc/en/Tutorial/SigFoxFirstConfiguration
*/

#include <SigFox.h>
#include <ArduinoLowPower.h>

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);

  while (!Serial) {};

  if (!SigFox.begin()) {
    Serial.println("Shield error or not present!");
    return;

  }
}

// the loop function runs over and over again forever
void loop() {
  String version = SigFox.SigVersion();
  String ID = SigFox.ID();
  String PAC = SigFox.PAC();

  // Display module informations
  Serial.println("MKRFox1200 Sigfox first configuration");
  Serial.println("SigFox FW version " + version);
  Serial.println("ID  = " + ID);
  Serial.println("PAC = " + PAC);

  Serial.println("");

  Serial.println("Register your board on https://backend.sigfox.com/activate with provided ID and PAC");

  delay(100);

  // Send the module to the deepest sleep
  SigFox.end();                      // wait for a second
  delay(5000);
}
