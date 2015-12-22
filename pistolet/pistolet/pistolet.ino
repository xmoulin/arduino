/*
 photoresistance + diode
 
 utilisation de laser pour activer
*/

int port = A0;
int valeur = 0;
float vin = 0;
int nbrTouche = 7;

void setup()
{
  pinMode(7, OUTPUT); //met la broche en sortie
  pinMode(8, OUTPUT); //met la broche en sortie
  pinMode(9, OUTPUT); //met la broche en sortie
  pinMode(5, OUTPUT); //met la broche en sortie

  Serial.begin(9600);
}

void loop()
{
  // Lit l’entrée analogique A0
  valeur = analogRead(port);
  Serial.print(valeur);
  Serial.print(" analog,");
  // convertit l’entrée en volt
  vin = 5 - (valeur * 5.0) / 1024.0;
  Serial.print(" volt ");
  Serial.println(vin);
  delay(30) ;

  if (valeur > 1000) {
    Serial.print("Allume ");
    Serial.println(nbrTouche);

    digitalWrite(nbrTouche, HIGH);
    if (nbrTouche == 7)
    {
      Serial.println("Touche 1");
    } else {
      Serial.print("Stop ");
      Serial.println(nbrTouche - 1);
      digitalWrite(nbrTouche - 1, LOW);
    }
    beep(1500);
    if (nbrTouche == 9)
    {
      nbrTouche = 6;
      
      for(int i = 0; i < 20 ; i++) {
        beep(50);
        i++;
      }
      digitalWrite(9, LOW);
    }

    nbrTouche = nbrTouche + 1;
    delay(10) ;
  }
}

void beep(unsigned char delayms) {
  analogWrite(5, 40);      // Almost any value can be used except 0 and 255
  // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(5, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms
}

