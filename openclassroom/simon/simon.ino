//variables
int mode = 0; //mode dans le programme
int taille = 0; // taille de la séquence en cours
int posTest = 0; // position de la réponse
int sequence[50]; //tableau de la séquence (50 max)
int duree = 250; //temps pour l'allumage des LED
//constantes
//À faire => tableau de constantes pour les pins des LED
const byte pinLed[4] = {2, 3, 4, 5};
const char pinSon = 8; //constante pour le pin du buzzer
const int freqSon[4] = {261, 329, 392, 523}; //tableau de constantes des fréquences de sons
//À faire => constantes pour la lecture du CAN0
const int pinCAN0 = A0;


void setup() {
  //À faire => initialisation des nombres aléatoires
  sequence[0] = random(16) / 4; 
  
  //À faire => paramétrage des pins LED et pin buzzer en mode OUTPUT
  //Param des LED
  for (int i = 0; i < 5; i++) {
    pinMode(pinLed[i], OUTPUT);
    digitalWrite(pinLed[i], LOW);
  }
  //Init SON
  pinMode(pinSon, OUTPUT);
  //Init Resistance
  pinMode(pinCAN0, INPUT);

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
  //À faire => en utilisant la fonction sonLum()
  //On allume suscessivement les 4 lED + Son associé
  for (int i = 0; i < 5; i++) {
    sonLum(i, duree);
  }
}

//fonction pour de son et lumière pour valider une suite de réponses
void valide() {
  //Si gagné, on fait 3 fois le tour des led rapidement
  for (int i = 0; i < 13; i++) {
    sonLum(i / 4, 20);
  }
}

//fonction de son et lumière pour invalider une suis de réponses
void nonValide() {
  //Si Perdu, on fait le tour des led a l'envers lentement
  for (int i = 4; i <= 0; i--) {
    sonLum(i, 500);
  }
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
  //À faire => en utilisant une boucle et la fonction sonLum()
  for (int i = 0; i <= taille; i++) {
    sonLum(sequence[taille], duree);
  }
}

//fonction qui teste l'état des boutons et retourne le numéro de bouton
int testBouton() {
  //À faire => Cette fonction doit utiliser la fonction sonLum() et retourner le numéro du bouton appuyé ou -1 si aucun bouton n'est appuyé
  int valeur=analogRead(pinCAN0);
  int bouton=-1;
  Serial.print(valeur); //affichage de la valeur lue par le CAN 0
  if (valeur>=1023){
    Serial.println(" bouton A");
    bouton=1;
  }
  else if (valeur>=999){
    Serial.println(" bouton B");
    bouton=2;
  }
  else if (valeur>=979){
    Serial.println(" bouton C");
    bouton=3;
  }
  else if (valeur>=959){
    Serial.println(" bouton D");
    bouton=4;
  }
  //Si un bouton a été appuyé, on joue le son associé
  if(bouton != -1) {
    sonLum(bouton, duree);
  }
  delay(10);
  return bouton;
}

//fonction qui compare la réponse avec la séquence en cours
void compare (int b) {
  //À faire => soit on n'est pas au bout de la séquence et on continue avec l'appui suivant, 
  //soit  on est au bout de la séquence et on appelle la fonction valide() et on retourne au mode 1,
  //soit le joueur a fait une erreur et on appelle la fonction nonValide() et on retourne au mode 0
  
  Serial.print("compare");
  Serial.print(b);
  Serial.print(" avec sequence[posTest]=");
  Serial.print(sequence[posTest]);
  Serial.print(" pour une taille de ");
  Serial.println(taille);

  //Tant qu on a pas donné autant de réponse que le taille du tableau
  if(posTest<taille) {
     //si le bouton est appuyé pour la bonne position
    if(b==sequence[posTest]) {
      posTest++;
    } else {
      nonValide();
      mode=0;
    }
  } else {
    valide();
    mode=1;
  }

}
