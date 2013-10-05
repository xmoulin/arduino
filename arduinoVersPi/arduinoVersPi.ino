/*
  Faire communiquer le raspberry et l'arduino
  http://blog.oscarliang.net/connect-raspberry-pi-and-arduino-usb-cable/
 */
 void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.println("Hello Pi");
  delay(2000);
}
