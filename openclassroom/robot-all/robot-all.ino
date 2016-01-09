#include <AFMotor.h>

AF_DCMotor moteurGauche(4);
AF_DCMotor moteurDroite(3);

#define PROCHE 20
#define DANGER 10
#define AFOND 255
#define PRUDENT 150
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
  moteurGauche.setSpeed(AFOND);
  moteurDroite.setSpeed(AFOND);
  avance(AFOND); //on démarre le robot
 }

void loop() {
  switch (mesure()){ //utilisation de la condition switch
    case 0: //l'obstacle est dans la zone DANGER
      Serial.println("*DANGER*");
      arret(); // on arrête le robot
      delay(200);
      recule(PRUDENT); // on le fait reculer un peu
      delay(200);
      recherche(); // on recherche la bonne voie
      break;
    case 1: //l'obstacle est dans la zone PROCHE
      Serial.println("Attention...");
      avance(PRUDENT); // on ralentit la vitesse
    default:
      avance(AFOND);
  }
}

//fonctions
int mesure(){
  //fonction qui mesure une distance avec le capteur
  unsigned long mesure = 0; // variable de mesure
  unsigned long cumul = 0; //variable pour la moyenne
  for (int t = 0; t < NBMESURE; t++) { // boucle pour effectuer les mesures
  
    digitalWrite(pinTrig, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(pinTrig, LOW); 
    mesure = pulseIn(pinEcho, HIGH); // fonction pulseIn qui attend un état haut et renvoie le temps d'attente 
    cumul+=mesure; //on cumule les mesures
    delay(50); //attente obligatoire entre deux mesures
  }
  mesure=cumul/NBMESURE; //on calcule la moyenne des mesures
  mesure=mesure/VSON;//on transforme en cm
  Serial.print("mesure=");
  Serial.println(mesure);
  if (mesure<=DANGER){//on teste si l'obstacle est dans la zone DANGER
    return 0; //si oui, on retourne le code de danger
  }
  else if (mesure>DANGER && mesure<=PROCHE){//on teste s'il est dans la zone PROCHE
    return 1; //si oui, on retourne le code de proche
  }
  return 2; // on retourne le code de sans risque
}

void recherche(){
  //fonction pour rechercher un passage
  tourneGauche(AFOND); //on positionne le robot à gauche (facultatif)
  delay (300);
  int etat=0; //variable de test de possibilité d'avancer
  while (etat!=2){ // tant que la distance n'est pas suffisante
    etat=mesure(); // on effectue la mesure
    tourneDroite(PRUDENT); //on fait tourner le robot
  }
  Serial.println("La voie est libre !");
  //retour au programme principal
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

