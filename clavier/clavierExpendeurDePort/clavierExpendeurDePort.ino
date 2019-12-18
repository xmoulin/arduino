/**
   HARDWARE
   - Utilisation d'expendeur de port CD74HC4067, ce qui va complexifier le code mais permet sur 5 ports de mettre 16 boutons
   -- https://fr.aliexpress.com/item/32724398868.html?spm=a2g0s.9042311.0.0.27426c37MVRE2u
   - Cartes ATmega32U4
   -- https://fr.aliexpress.com/item/32768308647.html?spm=a2g0s.9042311.0.0.27426c37Ygy5wa

   PORTS
   - 4 = Mux 1 - SIG : la lecture de la valeur Analogique issue de l'expendeur
   - 6, 7, 8, 9 = Mux 1 - Port sur l'expendeur pour évaluer lequel des 16 boutons est concerné (comptage en bianire sur les 4 ports pour faire 16 possibilités)
   - 10 = Mux 2 - SIG : la lecture de la valeur Analogique issue de l'expendeur
   - 18, 19, 20, 21 = Mux 1 - Port sur l'expendeur pour évaluer lequel des 16 boutons est concerné (comptage en bianire sur les 4 ports pour faire 16 possibilités)

   LIBRAIRIE
   - Voir dans le code au dessus de chaque librairie
 **/

//Librairies utilisées
//https://github.com/mikedotalmond/Arduino-Mux-CD74HC4067
#include <Mux.h>
//Librairie patché pour répondre au besoin du mex
//TODO lien a donner
#include <ButtonDebounce.h>
//Rechercher KeyboardAzertyFr dans le gestionnaire de librairie. Keyboard.h est natif mais gère seulement le qwerty
#include <KeyboardAzertyFr.h>

//Positionnement du debounceDelay
#define DEBOUNCE_DELAIS 250
Mux mux1;
Mux mux2;
//Ajoute des logs SERIAL si true
boolean debug = false;
//PIN possible sur ATMEGA32U4 : 21,20,19,18,15,14,16,10,9,8,7,6,5,4,1,0

//Mapping entre index du bouton (0 à 15) et valeur qui sera tapée sur le clavier
const String BUTTON_LETTER_MUX1[16] = {"azertyfonctionne", "AZERTYUIOP maj egalement", "abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "0123456789", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p"};
const String BUTTON_LETTER_MUX2[16] = {"2azertyfonctionne", "2AZERTYUIOP maj egalement", "2abcdefghijklmnopqrstuvwxyz", "2ABCDEFGHIJKLMNOPQRSTUVWXYZ", "20123456789", "2f", "2g", "2h", "2i", "2j", "2k", "2l", "2m", "2n", "2o", "2p"};
//Structure pour stocker le debounce sur un bouton
ButtonDebounce b1  = ButtonDebounce(DEBOUNCE_DELAIS);
ButtonDebounce BUTTON_BUDOUNCE_MUX1[16] = {b1, b1, b1, b1, b1, b1, b1, b1, b1, b1, b1, b1, b1, b1, b1, b1};
ButtonDebounce BUTTON_BUDOUNCE_MUX2[16] = {b1, b1, b1, b1, b1, b1, b1, b1, b1, b1, b1, b1, b1, b1, b1, b1};
int counter = 0;

void setup() { // initialize the buttons' inputs:

  mux1.setup(9, 8, 7, 6, A6); // initialise on setup. La PIN en dernière possition est celle de lecture de la valeur
  mux2.setup(21, 20, 19, 18, A10); // initialise on setup. La PIN en dernière possition est celle de lecture de la valeur

  Serial.begin(9600);
  KeyboardAzertyFr.begin();
}

void loop() {
  for (int i = 0; i < 16; i++)  {
    //MUX 1
    //On update la valeur interne du bouton i en gerant le debounce
    BUTTON_BUDOUNCE_MUX1[i].update(mux1.read(i));
    if (BUTTON_BUDOUNCE_MUX1[i].state() == HIGH) {
      if (debug) {
        Serial.print("Clicked MUX 1, bouton ");
        Serial.println(i);
      }
      KeyboardAzertyFr.print(BUTTON_LETTER_MUX1[i]);
    }
    //MUX 2
    //On update la valeur interne du bouton i en gerant le debounce
    BUTTON_BUDOUNCE_MUX2[i].update(mux2.read(i));
    if (BUTTON_BUDOUNCE_MUX2[i].state() == HIGH) {
      if (debug) {
        Serial.print("Clicked MUX 2, bouton ");
        Serial.println(i);
      }
      KeyboardAzertyFr.print(BUTTON_LETTER_MUX2[i]);
    }

    delay(10);
  }
}
