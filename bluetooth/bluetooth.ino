char val; // variable to receive data from the serial port
int ledpin = 8; // LED connected to pin 48 (on-board LED)
void setup() {
  pinMode(ledpin, OUTPUT);  // pin 48 (on-board LED) as OUTPUT
  Serial.begin(9600);       // start serial communication at 9600bps
  Serial.print("Init OK");
}
void loop() {
  if( Serial.available() )       // if data is available to read
  {
    val = Serial.read();    // read it and store it in 'val'
    //Serial.print("valeur=");
    //Serial.println(val);
  }
  if( val == 'H' )               // if 'H' was received
  {
    digitalWrite(ledpin, HIGH);  // turn ON the LED
    Serial.println("Pin HAUT");
  } else if (val == 'L') { 
    digitalWrite(ledpin, LOW);   // otherwise turn it OFF
    Serial.println("Pin BAS");
  }
  delay(100);                    // wait 100ms for next reading
}
