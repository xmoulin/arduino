const int pin = 8; // pin de lecture du TSOP
const int nbMax = 32; // nombre de lectures
int tableau[nbMax][2]; // tableau pour stocker les valeurs hautes et basses

void setup() {
  pinMode(pin, INPUT); // mode INPUT pour le pin de lecture
  Serial.begin(9600); //initialisation communication série
}

void loop() {
  if (digitalRead(pin)) { //si état haut capté
    lecture(); // appel de la fonction de lecture du code
    affichage(); // appel de la fonction d'affichage du résultat
  }
}

//fonction de lecture du code
void lecture() {
  for (int t = 0; t < nbMax; t++) { //boucle pour le nombre de lectures
    while (digitalRead(pin)) { // tant que état pulsé
      tableau[t][0]++; // on incrémente la valeur du tableau
    }
    while (!digitalRead(pin)) { // puis tant que état non pulsé
      tableau[t][1]++; // on incrémente l'autre valeur du tableau
    }
  }
}

// fonction d'affichage du résultat
void affichage() {
  for (int i = 0; i < 2; i++) { // boucle pour état Haut, puis état Bas
    for (int t = 0; t < nbMax; t++) { //lecture des valeurs
      Serial.print(tableau[t][i]); //affichage
      Serial.print ("\t"); //tabulation pour présentation
    }
    Serial.println(); //saut de ligne
  }
}

