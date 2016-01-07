#include <AFMotor.h>

AF_DCMotor moteurGauche(4);
AF_DCMotor moteurDroite(3);

#define PROCHE 20
#define DANGER 10
#define AFOND 200
#define PRUDENT 50
#define NBMESURE 5
#define VSON 59

//initialisation
int pinTrig = 12; //pin numérique utilisé pour le capteur ultrasons. 
int pinEcho = 11; //pin numérique utilisé pour le capteur ultrasons.

void setup() {
  Serial.begin(9600);
  pinMode(pinTrig, OUTPUT); 
  digitalWrite(pinTrig, LOW); 
  pinMode(pinEcho, INPUT); 

  // turn on motor
  moteurGauche.setSpeed(255);
  moteurDroite.setSpeed(255);

  
  avance(AFOND); //on démarre le robot
 }

void loop() {
  arret();
  delay(1000);
  avance(255);
  delay(1000);
  recule(255);
  delay(1000);
  tourneDroite(255);
  delay(1000);
  tourneGauche(255);
  delay(1000);
 
}

void arret(){
  Serial.println("Je stoppe");
  //fonction d'arrêt des moteurs
  moteurGauche.run(RELEASE);
  moteurDroite.run(RELEASE);
}

void defVitesse(int v){
  moteurGauche.setSpeed(v); //on redéfinit la vitesse
  moteurDroite.setSpeed(v); //des deux moteurs
}

void avance(int v){
  Serial.print("J'avance");
  affiche(v);
  defVitesse(v); //appel de la fonction pour définir la vitesse
  moteurGauche.run(FORWARD);
  moteurDroite.run(FORWARD);
  //fonctin de mise en route des deux moteurs dans le sens avant
  // on utilise la variable v pour le pilotage PWM
}

void recule(int v){
  Serial.print("Je recule");
  affiche(v);
  //fonctin de mise en route des deux moteurs dans le sens arrière
  // on utilise la variable v pour le pilotage PWM
 //fonction de marche arrière
  defVitesse(v);
  moteurGauche.run(BACKWARD);
  moteurDroite.run(BACKWARD);
}

void tourneDroite(int v){
  Serial.print("Je tourne a droite");
  affiche(v);
  //fonction de rotation à droite : un moteur dans un sens, l'autre dans le sens opposé
  // on utilise la variable v pour le pilotage PWM
  defVitesse(v);
  moteurGauche.run(FORWARD);
  moteurDroite.run(BACKWARD);
}

void tourneGauche(int v){
  Serial.print("Je tourne a gauche");
  affiche(v);
  //fonction de rotation à gauche : un moteur dans un sens, l'autre dans le sens opposé
  // on utilise la variable v pour le pilotage PWM
  defVitesse(v);
  moteurGauche.run(BACKWARD);
  moteurDroite.run(FORWARD);
}
void affiche(int v){
  //fonction complémentaire d'affichage
  if (v==AFOND){
    Serial.println(" a fond !");
  }
  else{
    Serial.println(" prudemment...");
  }
}

