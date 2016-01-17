/*
 * 'Jouez avec Simon' par Nanomaitre
 * Ce code est dans le domaine public
 */
//variables
int mode = 0; //mode dans le programme
int taille = 0; // taille de la séquence en cours
int posTest = 0; // position de la réponse
int sequence[50]; //tableau de la séquence (50 max)
int duree = 250; //temps pour l'allumage
//constantes
const char pinLed[4] = {2, 3, 4, 5}; //tableau de constantes pour les pins des LED
const char pinSon = 8; //constante pour le pin du buzzer
const int freqSon[4] = {261, 329, 392, 523}; //tableau de constantes des fréquences de sons
const int valeursBoutons[4] = {1020, 1000, 975, 950}; //constantes pour la lecture du CAN0

void setup() {
  randomSeed(analogRead(3)); //initialisation des nombres aléatoires
  // paramétrage des pins LED et pin buzzer
  for (int l = 0; l < 4; l++) {
    pinMode(pinLed[l], OUTPUT);
    digitalWrite(pinLed[l], LOW);
  }
  pinMode (pinSon, OUTPUT);
}

void loop() {
  //les actions sont réalisées en fonction du mode en cours
  switch (mode) { //on regarde le mode
    case 0: //mode de démarrage
      start(); // appel de la fonction de démarrage
      taille = 0; //on met la séquence à 0
      mode = 1; // on peut passer au mode suivant
      break;
    case 1: //mode de tirage d'une nouvelle séquence
      augmenteSequence(); //appel de la fonction pour allonger la séquence
      mode = 2; // on peut passer au mode suivant
      break;
    case 2: // mode d'écoute de la séquence
      joueSequence(); // on joue la séquence en cours
      posTest = 0; //on initialise la position de réponse
      mode = 3; // on peut passer au mode suivant
      break;
    case 3: // mode de réponse du joueur
      int b = testBouton(); // on appelle la fonction de lecture des boutons
      if (b != -1) { //si bouton appuyé
        compare(b); // appel de la fonction de validation
      }
      break;
  }
}

//-----------------------fonctions--------------------

//fonction de son et lumière pour le démarrage d'une nouvelle séquence
void start() {
  for (int l = 0; l < 4; l++) {
    sonLum(l, 50); //on appelle la fonction d'allumage avec son
  }
  delay(500); //attente
}

//fonction pour de son et lumière pour valider une suite de réponses
void valide() {
  for (int t = 0; t < 4; t++) {
    sonLum(3, 50);
  }
  delay(500);
}

//fonction de son et lumière pour invalider une suis de réponses
void nonValide() {
  for (int t = 0; t < 4; t++) {
    sonLum(0, 50);
  }
  delay(500);
}

//fonction qui allume une LED (l) en jouant le son associé pendant une durée (d)
void sonLum(int l, int d) {
  digitalWrite(pinLed[l], HIGH);
  tone(pinSon, freqSon[l], d);
  delay(d);
  digitalWrite(pinLed[l], LOW);
  delay(d);
}

//fonction de tirage aléatoire d'une séquence
void augmenteSequence() {
  sequence[taille] = random(16) / 4; //permet un tirage mieux réparti
  if (taille < 50) taille++;
}

//fonction qui joue la séquence en cours
void joueSequence() {
  for (int n = 0; n < taille; n++) {
    sonLum(sequence[n], duree);
  }
}

//fonction qui teste l'état des boutons et retourne le numéro de bouton
int testBouton() {
  int val = analogRead(0); //lecture du CAN0;
  if (val) { //si une valeur est perçue
    for (int n = 0; n < 4; n++) { //on parcours les valeurs possibles
      if (val > valeursBoutons[n]) {// en fonction du résultat
        sonLum(n, duree); //on allume la LED correspondante et on joue un son
        delay(50); //on attend un peu pour éviter les répétitions
        return n; // on renvoie le numéro de bouton appuyé
      }
    }
  }
  return -1; //si rien appuyé, on renvoie -1
}

//fonction qui compare la réponse avec la séquence en cours
void compare (int b) {
  if (b == sequence[posTest] && posTest < taille) { //si correspondance
    posTest++; //on passe à l'appui suivant
    if (posTest == taille) { // si on a atteind la fin de séquence
      valide(); //son et lumière de validation
      mode = 1; // on allonge la séquence
      return; //retour à la loop()
    }
    return; //retour à la loop()
  }
  //si échec
  nonValide(); //petite musique et lumière
  mode = 0; //on rejoue la séquence
}
