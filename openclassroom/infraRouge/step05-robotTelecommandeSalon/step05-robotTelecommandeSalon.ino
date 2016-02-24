/*
 * Pilotage de servomoteur par télécommande Infrarouge
 * by Nanomaitre pour OpenClassRooms
 * Ce programme déplace un servo-moteur en fonction
 * des touches appuyées d'une télécommande
 * Ce code est dans le domaine public
 */

#include "Servo.h" // on ajoute la bibliothèque Servo
const int pinServo = 2; // constante de pin pour le servomoteur
Servo monServo; // on définit l'objet Servo nommé monServo
//tableau qui stocke les codes : 5 codes de 65 valeurs
// /!\ Attention il correspond aux valeurs de ma télécommande
//Pour la vôtre, il faut utiliser le programme "Décodeur infrarouge"
//et créer ce tableau !
const unsigned int codes[5][65] = {
  0, 8680, 4300, 580, 500, 560, 500, 600, 1560, 600, 480, 560, 520, 580, 480, 560, 520, 580, 500, 580, 1580, 560, 1580, 560, 520, 580, 1560, 600, 1560, 560, 1580, 580, 1580, 560, 1580, 580, 1580, 560, 1600, 560, 1580, 560, 520, 580, 480, 600, 480, 580, 500, 560, 520, 560, 520, 580, 480, 600, 480, 560, 1600, 560, 1580, 580, 1580, 560, 0,
  0, 8680, 4280, 580, 520, 580, 500, 580, 1560, 600, 480, 580, 500, 580, 480, 600, 480, 600, 480, 580, 1560, 600, 1560, 580, 500, 580, 1560, 600, 1560, 580, 1560, 600, 1560, 580, 1560, 600, 480, 600, 480, 580, 500, 580, 480, 600, 480, 580, 500, 580, 1580, 580, 480, 600, 1560, 580, 1560, 600, 1560, 580, 1560, 600, 1560, 580, 1580, 580, 0,
  0, 8680, 4300, 560, 520, 580, 480, 580, 1580, 580, 500, 580, 500, 580, 480, 600, 480, 600, 480, 580, 1560, 600, 1560, 580, 500, 580, 1560, 600, 1560, 580, 1560, 600, 1560, 580, 1560, 600, 480, 600, 1560, 580, 1560, 600, 480, 600, 480, 580, 500, 580, 480, 600, 480, 580, 1580, 580, 480, 600, 480, 600, 1560, 580, 1560, 600, 1560, 580, 0,
  0, 8680, 4300, 600, 480, 580, 480, 580, 1580, 560, 520, 560, 520, 560, 500, 560, 520, 580, 500, 560, 1600, 580, 1560, 560, 520, 560, 1580, 600, 1560, 560, 1580, 580, 1580, 560, 1580, 580, 1580, 560, 520, 580, 480, 580, 500, 600, 480, 560, 520, 580, 1560, 600, 480, 560, 520, 560, 1580, 560, 1600, 560, 1600, 560, 1580, 560, 1580, 600, 0,
  0, 8680, 4300, 560, 520, 560, 500, 580, 1580, 560, 520, 560, 520, 560, 500, 560, 520, 560, 520, 560, 1580, 580, 1580, 560, 520, 560, 1580, 560, 1600, 560, 1580, 560, 1600, 580, 1560, 560, 520, 560, 520, 560, 1580, 600, 480, 560, 520, 560, 500, 580, 1580, 600, 480, 580, 1560, 600, 1560, 580, 500, 580, 1560, 600, 1560, 580, 1560, 600, 0
};
//Tableau des noms pour chaque code
String noms[5] = {"90 deg", "179 deg", "0 deg", "Vers 179 deg", "Vers 0 deg"};
const int pin = 8; // pin de lecture du TSOP
const int nbMax = 32; // nombre maximum de lectures
const int temporisation = 20; // temporisation de lecture des signaux pulsés
unsigned int tableau[nbMax * 2] = {0}; // tableau pour stocker les temps lus
int posServo = 90; // variable de position du servomoteur
int ajServo = 0; // variable de déplacement du servomoteur
int nbCode = 0; // numéro de code en cours
unsigned long tpsDep = millis(); // initialisation du temps
void setup() {
  monServo.attach(pinServo); // attache l'objet monServo à pinServo
  pinMode(pin, INPUT); // mode INPUT pour le pin de lecture
  Serial.begin(9600); //initialisation communication série.
}

void loop() {
  while (PINB & B00000001) // tant que aucun signal
    bougeServo(); // on gère le servomoteur
  //sinon
  lecture(); // appel de la fonction de lecture du code


}

//fonction de lecture du code
void lecture() {

  for (int t = 0; t < nbMax * 2; t += 2) { //boucle pour le nombre de lectures, progresse de 2 en 2
    while (PINB & B00000001) { // tant que état pulsé
      tableau[t] += temporisation; // on incrémente la valeur du tableau
      delayMicroseconds(temporisation); //attente
    }
    while (!(PINB & B00000001)) { // puis tant que état non pulsé
      tableau[t + 1] += temporisation; // on incrémente l'autre valeur du tableau
      delayMicroseconds(temporisation);
    }
  }
  delay(500); //délai pour ne plus rien recevoir
  compare(); // appel de la fonction de comparaison
  positionServo(); // appel de la fonction de positionnement du servomoteur
}


//fonction de comparaison
int compare () {
  boolean trouve = 0; // drapeau de commande trouvée
  for (int c = 0; c < 5; c++) { //boucle sur les codes
    int nbCor = 0; // variable de validation de comparaison
    for (int t = 0; t < nbMax * 2; t++) { // on parcourt les résultats
      int tmoins = tableau[t] - tableau[t] * 20 / 100; // valeur-20%
      int tplus = tableau[t] + tableau[t] * 20 / 100; // valeur+20%
      if (codes[c][t] > tmoins && codes[c][t] < tplus && t > 0) { //si dans la fourchette (sauf première valeur)
        nbCor++; // on valide la comparaison
      }
    }
    if (nbCor == nbMax * 2 - 1) { //si tout est validé (sauf première valeur)
      // On affiche la touche correspondante
      Serial.print("Commande ");
      Serial.print(c);
      Serial.print(" => ");
      Serial.println(noms[c]);
      nbCode = c; // on indique le code en cours
      trouve = 1; // on indique que la commande a été trouvée
    }
  }
  if (!trouve) { //si commande non trouvée
    Serial.println("Commande inconnue...");
  }
  //on vide le tableau de lecture
  for (int t = 0; t < nbMax * 2; t++) {
    tableau[t] = 0;
  }
}

//Fonction de positionnement du servomoteur
void positionServo() {
  switch (nbCode) { //en fonction du code
    case 0: // on place le servo à 90°
      ajServo = 0;
      posServo = 90;
      break;
    case 1: // on place le servo à 179°
      ajServo = 0;
      posServo = 179;
      break;
    case 2: // on place le servo à 0°
      ajServo = 0;
      posServo = 0;
      break;
    case 3: // on met la variable de déplacement à 1°
      ajServo = +1;
      break;
    case 4: // on met la variable de déplacement à -1°
      ajServo = -1; 
      break;
  }
}
//fonction de déplacement du servomoteur
void bougeServo() {
  unsigned long tpsAct = millis(); //temps actuel
  if (tpsAct - tpsDep > 50) { //si 50ms passées
    posServo += ajServo; // on déplace le servo
    tpsDep = tpsAct; //on réinitialise le temps
  }
  if (posServo <= 0 || posServo >= 179) // si servo hors limite
    ajServo = 0; // on stoppe le déplacement
  monServo.write(posServo); // on envoie la position du servomoteur
}

