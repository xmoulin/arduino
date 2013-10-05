/*
http://www.mon-club-elec.fr/pmwiki_mon_club_elec/pmwiki.php?n=MAIN.ArduinoInitiationLedsFeuxCirculation
*/

const int ROUGE_1=2; //declaration constante de broche
const int ORANGE_1=3; //declaration constante de broche
const int VERT_1=4; //declaration constante de broche
const int ROUGE_2=5; //declaration constante de broche
const int ORANGE_2=6; //declaration constante de broche
const int VERT_2=7; //declaration constante de broche 

// Configuration des broches en sortie : on configure les 6 broches utilisées en sortie à l'aide de l'instruction pinMode : 
void setup() {
  pinMode(ROUGE_1, OUTPUT); //met la broche en sortie
  pinMode(ORANGE_1, OUTPUT); //met la broche en sortie
  pinMode(VERT_1, OUTPUT); //met la broche en sortie
  pinMode(ROUGE_2, OUTPUT); //met la broche en sortie
  pinMode(ORANGE_2, OUTPUT); //met la broche en sortie
  pinMode(VERT_2, OUTPUT); //met la broche en sortie 
  
  //------- initialisation des LEDs au début = feu 1 au rouge et feu 2 au vert
  digitalWrite(ROUGE_1,HIGH); // met la broche au niveau HAUT
  digitalWrite(VERT_1,LOW); // met la broche au niveau BAS
  digitalWrite(ORANGE_1,LOW); // met la broche au niveau BAS
  digitalWrite(ROUGE_2,LOW); // met la broche au niveau BAS
  digitalWrite(VERT_2,HIGH); // met la broche au niveau HAUT
  digitalWrite(ORANGE_2,LOW); // met la broche au niveau BAS
  
  delay(5000); // pause de 5 secondes
}

//on réalise la séquence d'allumage / extinction des 6 LEDs de façon à obtenir la simulatin du fonctionnement de 2 feux tricolores : 
void loop() {
  //--- le feu 2 passe à l'orange
  digitalWrite(VERT_2,LOW); // éteint vert feu 2
  digitalWrite(ORANGE_2,HIGH); // allume orange feu 2
  
  delay (2000); // pause 2 secondes
  
  //--- le feu 2 passe au rouge
  digitalWrite(ORANGE_2,LOW); // éteint orange feu 2
  digitalWrite(ROUGE_2,HIGH); // allume rouge feu 2
  
  delay (1000); // pause courte 1 seconde
  
  //--- le feu 1 passe au vert
  digitalWrite(ROUGE_1,LOW); // éteint rouge feu 1
  digitalWrite(VERT_1,HIGH); // allume vert feu 1
  
  delay (5000); // pause longue 5 secondes
  
  //--- le feu 1 passe à l'orange
  digitalWrite(VERT_1,LOW); // éteint vert feu 1
  digitalWrite(ORANGE_1,HIGH); // allume orange feu 1
  
  delay (2000); // pause 2 secondes
  
  //--- le feu 1 passe au rouge
  digitalWrite(ORANGE_1,LOW); // éteint orange feu 1
  digitalWrite(ROUGE_1,HIGH); // allume rouge feu 1
  
  delay (1000); // pause courte 1 seconde
  
  //--- le feu 2 passe au vert
  digitalWrite(ROUGE_2,LOW); // éteint rouge feu 2
  digitalWrite(VERT_2,HIGH); // allume vert feu 2
  
  delay (5000); // pause longue 5 secondes
}

