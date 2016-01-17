/*
 * Menu avec écran LCD
 * Ce programme propose la navigation dans un menu simple
 * à l'aide de deux boutons (sur pin 8 et 9)
 * et d'un écran LCD compatible HD44780
 * Deux items : LED 13 et Lecture des CAN
 * Le menu LED 13 allume, éteint, ou fait clignoter la LED 13
 * Le menu Lecture des CAN, lit les valeurs des CAN 0,1,2 et 3 successivement
 * 
 * Nanomaitre 2015 pour OpenClassRooms
 * Ce programme est dans le domaine public
 */

#include <LiquidCrystal.h> // on importe la bibliothèque pour LCD
LiquidCrystal monEcran(12, 11, 5, 4, 3, 2); // on crée un objet LCD nommé monEcran en mode 4 bits
const int bout1 = 8; //constante du pin bouton 1
const int bout2 = 9; //constante du pin bouton 2
int posMenu = 0; //variable de position dans menu principal
int posSousMenu[2] = {0, 0}; // tableau pour stocker les positions de chaque sous-menu
int modeLed = 0;//variable du mode de la LED 13 (0=OFF, 1=ON, 2=Clignote)
boolean etatLed = 0; //état de la LED 13 (0=LOW, 1=HIGH)
int nCAN = 0; //numéro de CAN actuel
String ligne1[2] = {"LED 13 :        ", "Analogique :    "}; //tableau de titre du menu principal
String ligne2 = "OFF";//chaîne pour la ligne 2 (dépend du menu 1)
unsigned long tpsDep = millis(); //temps départ pour clignotement

void setup() {
  //configuration des pins
  pinMode(bout1, INPUT);
  pinMode(bout2, INPUT);
  pinMode(13, OUTPUT);
  //initialisation de l'écran LCD
  monEcran.begin(16, 2); //on initialise la communication avec 16 colonnes et 2 lignes
  monEcran.clear(); // on efface l'écran
}

void loop() {
  navigation(); //appel de la fonction pour naviguer dans les menus
  affichage(); //appel de la fonction pour rafraîchir l'affichage LCD
  allume(); //appel de la fonction pour gérer la LED 13
}

//fonction de navigation dans le menu
void navigation() {
  //création de variables d'état pour les boutons
  boolean etatBout1 = digitalRead(bout1);
  boolean etatBout2 = digitalRead(bout2);
  //Boucle pour naviguer dans les menus
  if (etatBout1 || etatBout2) { //si l'un des deux boutons est appuyé
    if (etatBout1) { // si bouton 1
      posMenu = (posMenu + 1) % 2; //on change le menu principal (2 positions)
    }
    if (etatBout2) { // si bouton 2
      switch (posMenu) { //l'action dépend du menu 1
        case 0: //si menu LED
          modeLed = (modeLed + 1) % 3; //on change le mode de la LED
          break;
        case 1 : //si menu CAN
          nCAN = (nCAN + 1) % 5; //on change le CAN lu
          break;
      }
    }
    delay(200); //attente pour éviter les répétitions
  }
}

//fonction de rafraîchissement de l'écran LCD
void affichage() {
  monEcran.setCursor(0, 0); // on positionne le curseur en haut à gauche
  monEcran.print(ligne1[posMenu]); // on écrit le menu de la ligne 1
  monEcran.setCursor(0, 1); // on passe à la ligne suivante
  //définition du menu pour la LED 13
  String mLED[3] = {
    "OFF             ",
    "ON              ",
    "Clignote        "
  };
  switch (posMenu) { // en fonction du menu 1
    case 0: // si menu LED
      ligne2 = mLED[modeLed]; //titre pris dans tableau mLED
      break;
    case 1: // si menu CAN
      readCAN(); // appel de la construction de l'affichage
      break;
  }
  monEcran.print(ligne2); // on affiche la ligne du menu 2
}

// fonction de construction de l'affichage de lecture de CAN
void readCAN () {
  int val = analogRead(nCAN); // on lit le CAN concerné
  ligne2 = "CAN "; //on initialise la chaîne
  ligne2 += String(nCAN); //on ajoute le numéro du CAN actuel
  ligne2 += " : "; //pour présentation
  ligne2 += String(val); // on ajoute la valeur lue du CAN actuel
  ligne2 += "         "; // on ajoute des espaces pour effacer l'affichage précédent
  //la ligne de menu est construite
}

//fonction qui gère la LED 13
void allume() {
  unsigned long tpsAct = millis(); //variable du temps actuel
  if (tpsAct - tpsDep > 200) { // si plus de 200ms
    switch (modeLed) { // on agit en fonction du mode actuel
      case 0: // si OFF
        etatLed = 0; // on éteint la LED
        break;
      case 1: // si ON
        etatLed = 1; // on allume la LED
        break;
      case 2: // si Clignote
        etatLed = !etatLed; //on inverse l'état de la LED
        break;
    }
    digitalWrite(13, etatLed); //on positionne la LED dans l'état concerné
    tpsDep = tpsAct; //on initialise le temps de départ
  }
}

