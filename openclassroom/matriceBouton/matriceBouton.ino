/*
 * Coffre-Fort by Nanomaitre 2015
 * LED connectée au pin 13 avec résitance 220Ω
 * matrice de boutons poussoirs 2x2 :
 * lignes : pins 2 et 3
 * colonnes : pins 4 et 5
 */
char code[5] = "DUUD"; //chaine de caractère pour stocker la combinaison
char codeSaisi[5] = "UUUU";//chaine de caractère pour stocker la saisie
char codeActuel[5] = "UUUU"; // tableau de char pour stocker l'état des boutons
int essai = 3; // variable pour stocker le nombre d'essais
boolean ok = 0; // boolean pour stocker si code trouvé ou non
boolean etatLED = 0; // boolean pour gérer le clignotement
int pinLED = 13; // variable pour stocker le pin de la LED
int ligne[2] = {2, 3}; //tableau pour stocker les pins des lignes
int colonne[2] = {4, 5}; //tableau pour stocker les pins des colonnes
unsigned long tpsDep = millis(); //initialisation du temps de départ pour clignotement

void setup() {
  //mise en OUTPUT et LOW des lignes et INPUT des colonnes
  for (int i = 0; i < 2; i++) {
    pinMode(ligne[i], OUTPUT);
    digitalWrite(ligne[i], LOW);
    pinMode(colonne[i], INPUT);
  }
  pinMode(pinLED, OUTPUT); //mise du pin de LED en mode OUTPUT
}

void loop() {

  if (!ok && essai > 0) { // si code non trouvé et essai restants
    test(); // appel de la fonction de lecture de matrice
    if (!relache()) { // si un appui est lu
      while (!relache()) { // on répète tant qu'un bouton au moins est appuyé
        test(); //lecture de la matrice
      }
      compare(); //au relâchement, on compare avec la combinaison
    }
  }
  else if (ok) { // sinon, teste si la combinaison est trouvée
    digitalWrite(pinLED, HIGH); //on allume la LED
  }
  else { //sinon (nombre d'essai à 0
    unsigned long tpsAct = millis(); //temps actuel
    if (tpsAct - tpsDep > 100) { //si différence > 100 ms
      etatLED = !etatLED; //on inverse l'état de la LED
      digitalWrite(pinLED, etatLED); //affichage de l'état de la LED
      tpsDep = tpsAct; // réinitialisatin du temps de départ.
    }
  }
}

//fonction de test des état des boutons de la matrice
void test() {
  int pos = 0; //position dans le tableau de char
  
  for (int l = 1; l >= 0; l--) { //boucle qui parcourt le tableau des lignes
    digitalWrite(ligne[l], HIGH); //on place la ligne en HIGH
    for (int c = 1; c >= 0; c--) { //boucle qui parcourt le tableau des colonnes
      boolean etat = digitalRead(colonne[c]); //on lit le résultat
      if (etat) { // si etat==1
        codeActuel[pos] = 'D'; //on écrit D dans le tableau de char
      }
      else { //sinon
        codeActuel[pos] = 'U'; //on écrit U dans le tableau de char
      }
      pos++; //on passe à la position suivante
    }
    digitalWrite(ligne[l], LOW); //on place la ligne en LOW
  }
  if (!relache()) { //on teste si l'appui est toujours actif
    // si oui on met à jour la position saisie
    for (int t = 0; t < 4; t++)
      codeSaisi[t] = codeActuel[t];
  }
  delay(100); //petite attente
}

//fonction de comparaison entre position saisie et combinaison
void compare() {
  for (int i = 0; i < 4; i++) { // on parcours les deux tableaux
    if (code[i] != codeSaisi[i]) { // si différence
      essai--; // on enlève un essai
      return; //on sort du test
    }
  }
  //si on arrive là c'est que le code correspond
  ok = 1; //donc on ouvre le coffre
}

//fonction de test si tout relaché
boolean relache() {
  //on parcourt l'état de la matrice
  for (int i = 0; i < 4; i++) {
    if (codeActuel[i] == 'D') //si une touche est enfoncée
      return 0; //on renvoie 0
  }
  //sinon
  return 1;//on renvoie 1
}

