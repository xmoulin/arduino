//Association de N lettre à N bouton.
//Utilisation de bouton poussoirs simples.

//Utilisation dela librairie Bounces 2 de Tomas Fredericks
//https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

#include <Keyboard.h>
#include <Mouse.h>
#define NUM_BUTTONS 3

boolean debug = false;
//PIN possible sur ATMEGA32U4 : 21,20,19,18,15,14,16,10,9,8,7,6,5,4,1,0
//A eviter, ça clique au demarage: 0 et 1 ou 2 et 3
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {2,3,9};
//Attention, c est querty, voir dans la doc de la lib Keyboard pour passer en azerty ou alors faire la correspondance.
const String BUTTON_LETTER[NUM_BUTTONS] = {"a","b", "c"};

Bounce * buttons = new Bounce[NUM_BUTTONS];

void setup() { // initialize the buttons' inputs:
  
  for (int i = 0; i < NUM_BUTTONS; i++) {
      buttons[i].attach( BUTTON_PINS[i] , INPUT);       //setup the bounce instance for the current button
      buttons[i].interval(25);              // interval in ms
    }

  Serial.begin(9600);
  Keyboard.begin();
}
 
void loop() {
  for (int i = 0; i < NUM_BUTTONS; i++)  {
    // Update the Bounce instance :
    buttons[i].update();

    if (debug) {
      int buttonState = digitalRead(BUTTON_PINS[i]);
      Serial.println(buttonState);
    }
    // If it fell, flag the need to toggle the LED
    if ( buttons[i].fell() ) {
      //Keyboard.write(BUTTON_LETTER[i]); //seulement une seule lettre
      Keyboard.print(BUTTON_LETTER[i]);
   }
  }

  if (debug) {
    Serial.println("---- fin loop ----");
    delay(50);
  }
}
