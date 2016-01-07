/*
Petit clin d'oeil mr Lukas
Nanomaître 2015
*/
// constantes de fréquences
#define DON 33
#define REB 35
#define REN 37
#define MIB 39
#define MIN 41
#define FAN 44
#define SOB 46
#define SON 49
#define LAB 52
#define LAN 55
#define SIB 58
#define SIN 62

// tableau pour la mélodie
int part[50][3] = {
  DON, 2, 100,
  DON, 2, 100,
  DON, 2, 100,
  LAB, 1, 75,
  MIB, 2, 25,
  DON, 2, 100,
  LAB, 1, 75,
  MIB, 2, 25,
  DON, 2, 200,
  SON, 2, 100,
  SON, 2, 100,
  SON, 2, 100,
  LAB, 2, 75,
  MIB, 2, 25,
  SIN, 1, 100,
  LAB, 1, 75,
  MIB, 2, 25,
  DON, 2, 200,
  DON, 3, 100,
  DON, 2, 100,
  DON, 3, 25,
  SIN, 2, 25,
  DON, 3, 25,
  0, 0, 75,
  SIN, 2, 50,
  SIB, 2, 100,
  SIB, 1, 100,
  SON, 2, 25,
  SOB, 2, 25,
  SON, 2, 25,
  0, 0, 75,
  SOB, 2, 50,
  FAN, 2, 100,
  SON, 1, 100,
  SIB, 1, 100,
  SON, 1, 75,
  MIB, 2, 25,
  DON, 2, 100,
  LAB, 1, 75,
  MIB, 2, 25,
  DON, 2, 200,
  -1
};
int pinSon = 3; // pin de connection du haut-parleur
int tempo = 120; // variable du tempo
int duree = 0; // variable de durée de note
unsigned long tempsDep; // variable de temps de départ
int p = 0; // variable de position dans le tableau de mélodie
void setup() {
  pinMode(pinSon,OUTPUT); 
  tempsDep = millis(); // initialisation du temps de départ
}

void loop() {
  joue(); // appel de la fonction pour jouer la mélodie
}

//fonction de lecture de la mélodie
void joue() {
  unsigned long tempsAct = millis();
  if (tempsAct - tempsDep >= duree) {
    if (part[p][0] != -1) { // test de fin de tableau
      noTone(pinSon);
      delay(10); // délai pour l'attaque
      // la fréquence est calculée en fonction des fréquences de base
      // et de l'octave définit dans le tableau
      int frequence = part[p][0] * pow(2, part[p][1] + 1);
      // la durée de la note est calculée comme en musique
      duree = 1000 / (tempo / 60) * (float(part[p][2]) / 100);
      if (frequence > 0) {
        tone (pinSon, frequence);
      }
      p++; //incrémentation de la position dans le tableau
    }
    else { 
      noTone(pinSon);
      p=0;// retour au début du tableau
      duree=1000;// attente avant répétition
    }
    tempsDep=tempsAct;
  }
}
