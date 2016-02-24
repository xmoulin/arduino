/*
Nous allons donc modifier notre programme de décodage pour obtenir des résultats plus proche de la réalité :

Utilisation de la lecture directe du registre attaché au pin 8 ;
Temporisation de la lecture sur une période de 25µs ajustable.
Voici le code proposé 

Nous avons maintenant une lecture un peu plus fiable des codes envoyés par la télécommande.

Mais que peut-on faire de ces codes ?

Et bien nous allons les exploiter ! Et pour y arriver, nous allons procéder en deux temps :

D'abord la construction d'un programme qui lit et reconnaît les boutons, et qui nous affiche la succession de temps à prendre en compte.
Ensuite, grâce à d'habiles copier/coller, nous construirons un autre programme qui lui utilisera ces codes pour piloter un servo-moteur (par exemple ;)) grâce aux même boutons de la télécommande.
 */

const int pin = 8; // pin de lecture du TSOP
const int nbMax = 64; // nombre de lectures
const int temporisation=25;
unsigned int tableau[nbMax][2]; // tableau pour stocker les valeurs hautes et basses

void setup() {
  pinMode(pin, INPUT); // mode INPUT pour le pin de lecture
  Serial.begin(9600); //initialisation communication série.
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
    while (PINB & B00000001) { // tant que état pulsé
      tableau[t][0]++; // on incrémente la valeur du tableau
      delayMicroseconds(temporisation);
    }
    while (!(PINB & B00000001)) { // puis tant que état non pulsé
      tableau[t][1]++; // on incrémente l'autre valeur du tableau
      delayMicroseconds(temporisation);
    }
  }
  delay(200);
}

// fonction d'affichage du résultat
void affichage() {
  Serial.println("*");
  for (int i = 0; i < 2; i++) { // boucle pour état Haut, puis état Bas
    for (int t = 0; t < nbMax; t++) { //lecture des valeurs
      Serial.print(tableau[t][i]*temporisation); //affichage en microsecondes
      Serial.print ("\t"); //tabulation pour présentation
      tableau[t][i]=0; //effacement de la valeur pour prochaine lecture
    }
    Serial.println(); //saut de ligne
  }
}
