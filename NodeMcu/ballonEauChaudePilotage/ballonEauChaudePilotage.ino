

#define B_POUSSOIR 7
#define RELAIS 12
#define RELAY_ON 0
#define RELAY_OFF 1

//Déclaration des variables
int memoireBp = LOW; //Mémoire du bouton poussoir
int etatBp = LOW; //Etat actuel du PB
int etatRelais = LOW; //Etat du relais
 
void setup() {
  Serial.begin(9600);          //  setup serial
  Serial.println("Init OK");
  //On initialise les entrées/sorties
  pinMode(B_POUSSOIR, INPUT);
  pinMode(RELAIS, OUTPUT);
  digitalWrite(RELAIS, etatRelais); //On écrit l'état du relais
 
}
 
void loop() {
  //Récupération de l'état actuel du BP
  etatBp = digitalRead(B_POUSSOIR);
 
  //Si l'état est différent de la mémoire et qu'il est à HIGH (Front montant)
  if(etatBp != memoireBp && etatBp == HIGH) {
     Serial.println("Appuyer!");
    //On inverse l'état du relais
    if(etatRelais == HIGH) {
      etatRelais = RELAY_ON;
      Serial.println("Etat bas - relais fermé (courant passe pas)");
 
    } else {
      etatRelais = RELAY_OFF;
      Serial.println("Etat Haut - courant passe");
    }
 
    //On écrit l'état du relais
    digitalWrite(RELAIS, etatRelais); 
    delay(100);
  }  
  //On conserve l'état du bouton
  memoireBp = etatBp;
}

