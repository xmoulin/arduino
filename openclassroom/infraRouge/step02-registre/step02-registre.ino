/*
 * Pour que notre programme soit plus efficace, il nous faut gérer deux points :

Le temps réel écoulé entre chaque lecture (et donc chaque incrémentation) ;
La vitesse de lecture du pin (donc la vitesse que la fonction digitalRead(pin)  prend pour s'exécuter.
Juste avant de calculer le temps écoulé entre chaque lecture, il faut savoir que la fonction digitalRead(pin) est une fonction qui comporte plusieurs instructions. Chaque instruction demande un certain temps d’exécution par la carte Arduino. Pour lire l'état d'un pin, on peut utiliser une méthode plus proche du langage machine (le langage de l'Arduino) : la lecture des registres.

Je vous ai déjà parlé des registres, mais sans vous en donner une explication réelle. Il faut savoir que chaque information de l'Arduino est contenue dans des cases mémoires, chacune avec des adresses (une position dans la mémoire de microprocesseur) définies et fixes. Faire appel à un registre (en lecture ou en écriture) revient à aller directement lire ou écrire dans cette case mémoire ! On gagne du temps mais le programme est beaucoup moins lisible.

Voici donc un programme qui compare, sur 10 mesures, le temps d'exécution de la fonction digitalRead(), et le temps de  lecture directe du registre qui contient le pin 8 :


Ce programme ne fonctionne qu'avec la lecture du pin 8 (bien qu'on n'en affiche pas le résultat) !

Vous obtenez une série de mesures qui vous permettent de comparer les vitesses d'exécution. Voici un exemple de résultat que j'obtiens :

******
8 8 8 8 4 4 8 8 8 8 
4 0 4 4 4 4 4 0 0 4

La première ligne correspond au temps mis en microsecondes pour exécuter la fonction digitalRead() .

La seconde ligne correspond au temps mis en mircrosecondes pour lire directement le registre (en extrayant le résultat correspondant au pin 8).

On remarque que la fonction digitalRead(pin) est souvent deux fois plus lente que la lecture directe des registres. Cela peut paraître faible comme différence, mais il faut savoir que pour la lecture des temps d'un code de télécommande, c'est important !

La fonction micros() est elle même gourmande en temps, ce qui explique parfois la différence entre les résultats. De plus elle ne réalise des mesures que par tranche de 4µs (c'est dû au paramétrage du timer 0 de l'Arduino), d'où ces résultats multiples de 4.
 */

const int pin = 8; // constante de pin lu

void setup() {
  Serial.begin(9600); //initialisation communication série
  pinMode(pin, INPUT); // mise en mode input du pin
}

void loop() {
  calcule(); //appel de la fonction calcule
  delay(500); // délai pour l'affichage
}

void calcule() {
  int tableau[10][2]; //tableau de récupération des valeurs
  for (int t = 0; t < 10; t++) { //boule pour 10 mesures
    unsigned long tpsDep, tpsFin; // variables pour les temps
    tpsDep = micros(); // temps de départ
    digitalRead(pin); // fonction digitaRead()
    tpsFin = micros(); // temps de fin
    tableau[t][0] = tpsFin - tpsDep; // on met le résultat dans le tableau

    tpsDep = micros(); // temps départ
    boolean n = PINB & B00000001;  // lecture du registre
    tpsFin = micros(); // temps de fin
    tableau[t][1] = tpsFin - tpsDep; // on met le résultat dans le tableau
  }

  //affichage du tableau des résultats
  Serial.println("******");
  for (int i = 0; i < 2; i++) {
    for (int t = 0; t < 10; t++) {
      Serial.print(tableau[t][i]);
      Serial.print("\t");
    }
    Serial.println();
  }
  Serial.println();
}

