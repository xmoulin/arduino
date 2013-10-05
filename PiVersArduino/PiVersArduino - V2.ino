/*
  Faire communiquer le raspberry et l'arduino
  http://blog.oscarliang.net/connect-raspberry-pi-and-arduino-usb-cable/
 */
const int ledPin = 13;
//Declaration du tableau de port à initialiser - de 0 a 13 par defaut, il ne sont pas initialise
int myInitPort[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void setup(){
  Serial.begin(9600);
}

void loop(){
  if (Serial.available())  {
     light(Serial.read() - '0');
  }
  delay(500);
}

void light(int pin){
  //Si port pas encore initialisé, je l'initialise pas
  if (myInitPort[pin] == 0) {
    Serial.println("initalisation du port "+pin);
    pinMode(pin, OUTPUT);
    //On passe a 1 le port pour ne plus rerentrer dans cette partie
    myInitPort[pin] = 1;
  }
  
  for (int i = 0; i < 3; i++)  {
    digitalWrite(pin, HIGH);
    delay(500);
    digitalWrite(pin, LOW);
    delay(500);
  }
}
