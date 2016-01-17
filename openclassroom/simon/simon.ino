//variables
int mode = 0; //mode dans le programme
int taille = 0; // taille de la séquence en cours
int posTest = 0; // position de la réponse
int const tailleMax = 50; // taille max
int sequence[tailleMax]; //tableau de la séquence
int duree = 250; //temps pour l'allumage des LED

//constantes
//À faire => tableau de constantes pour les pins des LED
const byte pinLed[4] = {2, 3, 4, 5};
const char pinSon = 8; //constante pour le pin du buzzer
const int freqSon[5] = {261, 329, 392, 523, 900}; //tableau de constantes des fréquences de sons
//À faire => constantes pour la lecture du CAN0
const int pinCAN0 = A0;

void setup() {
  Serial.begin(9600);
  //À faire => initialisation des nombres aléatoires
  sequence[0] = random(16) / 4; 
  
  //À faire => paramétrage des pins LED et pin buzzer en mode OUTPUT
  //Param des LED
  for (int i = 0; i < 4; i++) {
    pinMode(pinLed[i], OUTPUT);
    digitalWrite(pinLed[i], LOW);
    Serial.print(pinLed[i]);
    Serial.println(" Pin LED LOW ");
    
  }
  //Init SON
  pinMode(pinSon, OUTPUT);
  //Init Resistance
  pinMode(pinCAN0, INPUT);
  Serial.println("Setup OK");
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
  Serial.println("Start");
  //À faire => en utilisant la fonction sonLum()
  //On allume suscessivement les 4 lED + Son associé
  for (int i = 0; i < 4; i++) {
    sonLum(i, duree);
  }
  allumeAll(500);
  delay(200);
}

//fonction pour de son et lumière pour valider une suite de réponses
void valide() {
  Serial.println("Valide");
  //Je trouve ça troublant de faire du son et lumiere en cas de bonne réponse, j'ajoute plutot la methode victoire() en cas de reussite de toute la suite.
}

//fonction de son et lumière pour invalider une suis de réponses
void nonValide() {
  Serial.println("Non Valide");
  //Allume rien mais fait un son specifique
  sonLum(5, 200);
  sonLum(5, 200);
  sonLum(5, 200);
}
//fonction qui allume une LED (l) en jouant le son associé pendant une durée (d)
void sonLum(int l, int d) {
  digitalWrite(pinLed[l], HIGH);
  tone(pinSon, freqSon[l], d);
  delay(d);
  digitalWrite(pinLed[l], LOW);
  delay(d);
}

//fonction qui allume puis etteint toutes les led pendant une durée (d)
void allumeAll(int d) {
  digitalWrite(pinLed[0], HIGH);
  digitalWrite(pinLed[1], HIGH);
  digitalWrite(pinLed[2], HIGH);
  digitalWrite(pinLed[3], HIGH);
  delay(d);
  digitalWrite(pinLed[0], LOW);
  digitalWrite(pinLed[1], LOW);
  digitalWrite(pinLed[2], LOW);
  digitalWrite(pinLed[3], LOW);
}

//fonction qui allume toutes les LED en jouant le son associé pendant une durée (d)
void victoire() {
  for (int i = 0; i < 32; i++) {
    sonLum(i/4, 50);
  }
}

//fonction de tirage aléatoire d'une séquence
void augmenteSequence() {
  Serial.println("Augmente sequence");
  sequence[taille] = random(16) / 4; //permet un tirage mieux réparti
  if (taille < 50) taille++;
}

//fonction qui joue la séquence en cours
void joueSequence() {
  Serial.print("Joue sequence =");
  //À faire => en utilisant une boucle et la fonction sonLum()
  for (int i = 0; i < taille; i++) {
    sonLum(sequence[i], duree);
    Serial.print(sequence[i]);
    Serial.print(',');
  }
  Serial.println("");
}

//fonction qui teste l'état des boutons et retourne le numéro de bouton
int testBouton() {
  //À faire => Cette fonction doit utiliser la fonction sonLum() et retourner le numéro du bouton appuyé ou -1 si aucun bouton n'est appuyé
  int valeur=analogRead(pinCAN0);
  int bouton=-1;
  
  if (valeur>=1015){
    Serial.println(" bouton 0");
    bouton=0;
  }
  else if (valeur>=997){
    Serial.println(" bouton 1");
    bouton=1;
  }
  else if (valeur>=970){
    Serial.println(" bouton 2");
    bouton=2;
  }
  else if (valeur>=900){
    Serial.println(" bouton 3");
    bouton=3;
  }
  //Si un bouton a été appuyé, on joue le son associé
  if(bouton != -1) {
    sonLum(bouton, duree);
    Serial.println(valeur);
  }
  delay(10);
  return bouton;
}

//fonction qui compare la réponse avec la séquence en cours
void compare (int b) {
  //soit on n'est pas au bout de la séquence et on continue avec l'appui suivant, 
  //soit  on est au bout de la séquence et on appelle la fonction valide() et on retourne au mode 1,
  //soit le joueur a fait une erreur et on appelle la fonction nonValide() et on retourne au mode 0
  
  Serial.print("compare le bouton ");
  Serial.print(b);
  Serial.print(" avec sequence[posTest]=");
  Serial.print(sequence[posTest]);
  Serial.print(" pour une taille de ");
  Serial.print(taille);
  Serial.print(" position en cours=");
  Serial.println(posTest);
   //si le bouton est appuyé pour la bonne position
  if(b==sequence[posTest]) {
    Serial.println("bon bouton");
    posTest++;
  } else {
    Serial.println("pas bon bouton");
    nonValide();
    mode=0;
  }

  if (posTest==tailleMax) {
    victoire();
    mode=0;
  }
  //Tant qu on a pas donné autant de réponse que le taille du tableau
  else if(posTest<taille ) {
    //On attend le clique suivant
  } else {
    Serial.println("fin de la serie, on incremente");
    valide();
    mode=1;
  }

}
