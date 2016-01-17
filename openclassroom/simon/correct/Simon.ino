//variables
int mode = 0; //mode dans le programme
int taille = 0; // taille de la séquence en cours
int posTest = 0; // position de la réponse
int sequence[50]; //tableau de la séquence (50 max)
int duree = 250; //temps pour l'allumage des LED
//constantes
const int pinLed[4] = {2, 3, 4, 5}; //tableau de constantes pour les pins des LED
const char pinSon = 8; //constante pour le pin du buzzer
const int freqSon[4] = {261, 329, 392, 523}; //tableau de constantes des fréquences de sons
const int bouton = A0; //constantes pour la lecture du CAN0

void setup() {
  //initialisation des nombres aléatoires
  randomSeed(analogRead(1));
  //paramétrage des pins LED et pin buzzer en mode OUTPUT
  for (int l=2;l<6;l++){
    pinMode(l,OUTPUT);
    digitalWrite(l,LOW);
  }
  pinMode(pinSon, OUTPUT);
}

void loop() {
  //les actions sont réalisées en fonction du mode en cours
  switch (mode) { //on regarde le mode
    case 0: //mode de démarrage
      start(); // appel de la fonction de démarrage
      taille = 0; //on met la séquence à 0
      posTest = 0; //on recommence au début
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
        delay(100); //petite attente
        sonLum(b,duree); //on joue la note du joueur
        compare(b); // appel de la fonction de validation
      }
      break;
  }
}

//-----------------------fonctions--------------------

//fonction de son et lumière pour le démarrage d'une nouvelle séquence
void start() {
  for (int l=0;l<5;l++){
    sonLum(l,300);
  }
  delay(600);
}

//fonction pour de son et lumière pour valider une suite de réponses
void valide() {
  for (int k=0;k<2;k++){
    for (int l=0;l<5;l++){
      sonLum(l,30);
    }
  }
  delay(200);
}

//fonction de son et lumière pour invalider une suis de réponses
void nonValide() {
  for (int k=0;k<2;k++){
    for (int l=4;l>-1;l--){
      sonLum(l,70);
    }
  }
  delay(200);
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
  if (taille<50) taille++;
}

//fonction qui joue la séquence en cours
void joueSequence() {
  //en utilisant une boucle et la fonction sonLum()
    for (int l=0;l<taille;l++){
    sonLum(sequence[l],duree);
  }
}

//fonction qui teste l'état des boutons et retourne le numéro de bouton
int testBouton() {
  //Cette fonction doit utiliser la fonction sonLum() et retourner le numéro du bouton appuyé ou -1 si aucun bouton n'est appuyé
  int valeur=analogRead(bouton);
  if (valeur>=1023){
    return 0;
  }
  else if (valeur>=999){
    return 1;
  }
  else if (valeur>=979){
    return 2;
  }
  else if (valeur>=959){
    return 3;
  }
  else {
    return -1;
  }
}

//fonction qui compare la réponse avec la séquence en cours
void compare (int b) {
  //soit on n'est pas au bout de la séquence et on continue avec l'appui suivant, soit  on est au bout de la séquence et on appelle la fonction valide() et on retourne au mode 1, soit le joueur a fait une erreur et on appelle la fonction nonValide() et on retourne au mode 0
  if(sequence[posTest] == b){
    if(posTest+1<taille){
      posTest++;
    }
    else {
      valide();
      mode=1;
    }
  }
  else {
    nonValide();
    mode=0;
  }
  //On ne passe pas à la suite tant que le bouton n'est pas relaché
  while (testBouton() != -1) {}
}
