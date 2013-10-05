#include <Piwi.h>

/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor 
 
 This example code is in the public domain.


 Cote PI faire:
  ~/depotGit/rcswitch-pi $ sudo ./sendRev B 1 1
 */

// digital pin 2 has a pushbutton attached to it. Give it a name:
int remotereceiver = 2;
String octetcode;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  //Serial.begin(115200);
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(remotereceiver, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  delay(22);
  // read the input pin:
  for(int code = 0; code < 7; code++)
  {
    octetcode = octetcode + digitalRead(remotereceiver);
  }
    
   Serial.println(octetcode);
   octetcode = "";

   

}
