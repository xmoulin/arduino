#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//variables
int mode = 0; //mode dans le programme
int taille = 0; // taille de la séquence en cours
int posTest = 0; // position de la réponse
int sequence[50]; //tableau de la séquence (50 max)
int duree = 250; //temps pour l'allumage des LED
int score = 0;
int maxScore = 0;
//constantes
const int pinLed[4] = {2, 3, 4, 5}; //tableau de constantes pour les pins des LED
const char pinSon = 8; //constante pour le pin du buzzer
const int freqSon[4] = {261, 329, 392, 523}; //tableau de constantes des fréquences de sons
const int pinBouton = A0; //constantes pour la lecture du CAN0
const int BOUTON_1_PIN = 9;
const int BOUTON_2_PIN = 10;
const int BOUTON_3_PIN = 11;
const int BOUTON_4_PIN = 12;

void setup() {
  Serial.begin(9600);
  Serial.println("SETUP");
  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Bonjour!");

  //initialisation des nombres aléatoires
  randomSeed(analogRead(1));
  //paramétrage des pins LED et pin buzzer en mode OUTPUT
  for (int l = 2; l < 6; l++) {
    pinMode(l, OUTPUT);
    digitalWrite(l, LOW);
  }
  pinMode(pinSon, OUTPUT);
  pinMode(BOUTON_1_PIN, INPUT);
  pinMode(BOUTON_2_PIN, INPUT);
  pinMode(BOUTON_3_PIN, INPUT);
  pinMode(BOUTON_4_PIN, INPUT);

  Serial.println("SETUP OK");
}

void loop() {
  //les actions sont réalisées en fonction du mode en cours
  switch (mode) { //on regarde le mode
    case 0: //mode de démarrage
      score = 0;
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
        sonLumSansDelay(b, duree); //on joue la note du joueur
        compare(b); // appel de la fonction de validation
      }
      break;
  }
}

//-----------------------fonctions--------------------

//fonction de son et lumière pour le démarrage d'une nouvelle séquence
void start() {
  lcd.clear();
  lcd.print("Initialisation");
  afficheScore();
  for (int l = 0; l < 5; l++) {
    sonLum(l, 300);
  }

  lcd.clear();
  lcd.print("Ecoute bien.");
  delay(600);
}

//fonction pour de son et lumière pour valider une suite de réponses
void valide() {
  lcd.clear();
  lcd.print("Bravo!");
  score++;
  afficheScore();
  for (int k = 0; k < 2; k++) {
    for (int l = 0; l < 5; l++) {
      sonLum(l, 30);
    }
  }
  delay(200);
}

//fonction de son et lumière pour invalider une suis de réponses
void nonValide() {
  lcd.clear();
  lcd.print("Nonnnnnnnn!");
  afficheScore();
  for (int k = 0; k < 2; k++) {
    for (int l = 4; l > -1; l--) {
      sonLum(l, 70);
    }
  }
  delay(1000);
}

//fonction qui allume une LED (l) en jouant le son associé pendant une durée (d)
void sonLum(int l, int d) {
  sonLumSansDelay(l, d);
  delay(d);
}
void sonLumSansDelay(int l, int d) {
  digitalWrite(pinLed[l], HIGH);
  tone(pinSon, freqSon[l], d);
  delay(d);
  digitalWrite(pinLed[l], LOW);
}
//fonction de tirage aléatoire d'une séquence
void augmenteSequence() {
  sequence[taille] = random(16) / 4; //permet un tirage mieux réparti
  afficheScore();
  if (taille < 50) taille++;

}

//On affiche la 2eme ligne du LCD
void afficheScore() {
  if (score > maxScore) maxScore = score;
  lcd.setCursor(0, 1);
  lcd.print("score=");
  lcd.print(score);
  lcd.print(" max=");
  lcd.print(maxScore);
}

//fonction qui joue la séquence en cours
void joueSequence() {
  //en utilisant une boucle et la fonction sonLum()
  for (int l = 0; l < taille; l++) {
    sonLum(sequence[l], duree);
  }
  lcd.clear();
  lcd.print("A toi!");
  afficheScore();
}

//fonction qui teste l'état des boutons et retourne le numéro de bouton
int testBouton() {
  int bouton = -1;
  int val = digitalRead(BOUTON_1_PIN);  // read input value;
  int valDelay = 20;
  delay(valDelay); //cf http://arduino.cc/en/Tutorial/AnalogInputPins have to wait a little before reading another value
  if (val == HIGH) {         // check if the input is HIGH (button released)
    bouton = 0;
    Serial.println("bouton 0");
  } else {
    val = digitalRead(BOUTON_2_PIN);  // read input value
    delay(valDelay); //cf http://arduino.cc/en/Tutorial/AnalogInputPins have to wait a little before reading another value
    if (val == HIGH) {         // check if the input is HIGH (button released)
      bouton = 1;
      Serial.println("bouton 1");
    } else {
      val = digitalRead(BOUTON_3_PIN);  // read input value
      delay(valDelay); //cf http://arduino.cc/en/Tutorial/AnalogInputPins have to wait a little before reading another value
      if (val == HIGH) {         // check if the input is HIGH (button released)
        bouton = 2;
        Serial.println("bouton 2");
      } else {
        val = digitalRead(BOUTON_4_PIN);  // read input value
        delay(valDelay); //cf http://arduino.cc/en/Tutorial/AnalogInputPins have to wait a little before reading another value
        if (val == HIGH) {         // check if the input is HIGH (button released)
          Serial.println("bouton 3");
          bouton = 3;
        }
      }
    }
  }
  //Serial.print("bouton=");
  //Serial.println(bouton);
  return bouton;
}

//fonction qui compare la réponse avec la séquence en cours
void compare (int b) {
  //soit on n'est pas au bout de la séquence et on continue avec l'appui suivant, soit  on est au bout de la séquence et on appelle la fonction valide() et on retourne au mode 1, soit le joueur a fait une erreur et on appelle la fonction nonValide() et on retourne au mode 0
  if (sequence[posTest] == b) {
    if (posTest + 1 < taille) {
      posTest++;
    }
    else {
      valide();
      mode = 1;
    }
  }
  else {
    nonValide();
    mode = 0;
  }
  //On ne passe pas à la suite tant que le bouton n'est pas relaché
  while (testBouton() != -1) {}
}

