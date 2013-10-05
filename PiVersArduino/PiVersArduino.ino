/*
  Faire communiquer le raspberry et l'arduino
  http://blog.oscarliang.net/connect-raspberry-pi-and-arduino-usb-cable/
  
  Patch pour envoyer 2 chiffres et ne passer la pin en mode sortie qu'une fois.
 */
//Declaration du tableau de port à initialiser - de 0 a 13 par defaut, il ne sont pas initialise
int myInitPort[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//Valeur du port 1 ou 0 pour chacun des 13 port
int myPortValue[]={1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int incomingByte = 0;   // for incoming serial data

void setup(){
  Serial.begin(9600);
  /*pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);*/
}

void loop(){
  if (Serial.available())  {
     // read the incoming byte: le -0 est necessaire sinon il faut faire -48 ou -49 sur le chiffre recupere
     incomingByte = Serial.read() - '0';
     //say what you got:
     Serial.print("I received: ");
     Serial.print(incomingByte, DEC);
     int nbr = incomingByte;
     Serial.print(", nbr envoye=");
     Serial.println(nbr);
     //Si nbr =1 alors on attend un deuxieme
     if (nbr == 1) {
       //S'il y a encore qlq chose dans les tuyaux
       if( Serial.available() > 0) {
         incomingByte = Serial.read() - '0';
         nbr = (10 + incomingByte);
         Serial.print("2eme chiffre, donc nbr=");
         Serial.println(nbr);
       }
     }
     Serial.print("light=");
     Serial.println(nbr);
     light(nbr);
  }
  delay(500);
}

void light(int pin){
  //Si port pas encore initialisé, je l'initialise pas
  //Serial.println("Lecture de la valeur :"+pin);
  if (myInitPort[pin] == 0) {
    //Serial.println("initalisation du port :"+pin);
    pinMode(pin, OUTPUT);
    //On passe a 1 le port pour ne plus rerentrer dans cette partie
    myInitPort[pin] = 1;
  }

  //On allume si port allumé, sinon, on eteint
  Serial.println(myPortValue[pin]);
  if (myPortValue[pin] == 0) {
    digitalWrite(pin, HIGH);
    myPortValue[pin] = HIGH;
  } else {
    digitalWrite(pin, LOW);
    myPortValue[pin] = LOW;
  }
}

