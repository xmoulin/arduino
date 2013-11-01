
/* 
Utilisation du capteur Ultrason HC-SR04 
http://itechnofrance.wordpress.com/2013/03/12/utilisation-du-module-ultrason-hc-sr04-avec-larduino/
*/

// définition des broches utilisées 
int trig = 12; 
int echo = 11; 
long lecture_echo; 
long cm;

void setup() 
{ 
  pinMode(trig, OUTPUT); 
  digitalWrite(trig, LOW); 
  pinMode(echo, INPUT); 
  Serial.begin(9600); 
}

void loop() 
{ 
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  lecture_echo = pulseIn(echo, HIGH); 
  cm = lecture_echo / 58; 
  Serial.print("Distance en cm : "); 
  Serial.println(cm); 
  delay(1000); 
}
