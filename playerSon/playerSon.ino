/**
 * Module ISD1820
 * Enregisrteur et joueur de son
 * http://idehack.com/blog/tutoriel-arduino-le-voice-recorder-isd1820/
 */
int pinLED = 2;
int pinSpeaker = 4;
int pinBoutton = 0;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(pinLED, OUTPUT);
  pinMode(pinSpeaker, OUTPUT);
  pinMode(pinBoutton, INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() {
  int clic = digitalRead(pinBoutton);
  Serial.println(clic ); 

  if (clic == HIGH)
  {
    digitalWrite(pinLED, HIGH);
    digitalWrite(pinSpeaker, HIGH);
    
  } else {
    digitalWrite(pinLED, LOW);  
    digitalWrite(pinSpeaker, LOW);  
  }
  delay(100);
  
}
