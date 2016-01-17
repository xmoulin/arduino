//tableau des matrices
char matrices[26][10] = {
  "aaaaeaaea", //a
  "aeeaaeaae", //b
  "aaaaeeaaa", //c
  "eeaeaaeaa", //d
  "aaaaaeaaa", //e
  "aaaaaeaee", //f
  "aaeaeaaaa", //g
  "aeaaaaaea", //h
  "eaeeaeeae", //i
  "eaeeaeaae", //j
  "aeaaaeaea", //k
  "aeeaeeaaa", //l
  "aaaaaaaea", //m
  "aeeaeaaea", //n
  "aaaaeaaaa", //o
  "aaeaaeaee", //p
  "eaaeaaeea", //q
  "aaeaaeaea", //r
  "eaaeaeaae", //s
  "aaaeaeeae", //t
  "aeaaeaaaa", //u
  "aeaaeaeae", //v
  "aeaaaaaaa", //w
  "aeaeaeaea", //x
  "aeaeaeeae", //y
  "aaeeaeeaa"  //z
};
char mot[10]="salut"; //mot à épeler
int pos = 0; //variable de position dans le mot
int vitesse=500; // variable pour la vitesse en ms entre chaque matrice
char matriceAct[10]="eeeeeeeee"; //tableau de matrice en cours
unsigned long tpsDep = millis(); //initialisation du temps
byte ligne[3] = {4, 3, 2}; //stockage des pins ligne
byte colonne[3] = {5, 6, 7}; // stockage des pins colonne

void setup() {
  //initialisation des pins en OUTPUT et mise à l'état LOW ou HIGH
  for (byte l = 2; l < 8; l++) {
    pinMode(l, OUTPUT);
    if (l > 1 && l < 4)
      digitalWrite(l, LOW);
    else
      digitalWrite(l, HIGH);
  }
}

void loop() {
  unsigned long tpsAct = millis();
  if (tpsAct - tpsDep > vitesse) { //teste si temps écoulé
    char lettre=mot[pos]; //on lit la lettre du mot
    copie(byte(lettre)-97); //appel de la fonction de mise à jour de la matrice en cours
    pos = (pos + 1) % 10; //avancée du curseur de position dans le mot
    tpsDep = tpsAct; //réinitialisation du temps
  }
  affiche(); //appel de la fonction d'affichage
}

// fonction d'affichage
void affiche() {
  for (byte p = 0; p < 9; p++) { //parcours du tableau matrice
    if (matriceAct[p] == 'a') { //test si 'a'
      //allumage
      digitalWrite(ligne[p / 3], HIGH);
      digitalWrite(colonne[p % 3], LOW);
      delay(2);
      //exctinction
      digitalWrite(colonne[p % 3], HIGH);
      digitalWrite(ligne[p / 3], LOW);
    }
  }
}

//fonction de copie de tableau en fonction du code de la lettre.
void copie(byte n) {
  for (byte p = 0; p < 10; p++) {
    matriceAct[p] = matrices[n][p];
  }
}

