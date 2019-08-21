//Utilisation dela librairie Bounces 2 de Tomas Fredericks
//https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

#include <Keyboard.h>
#include <Mouse.h>

#define NUM_BUTTONS 2
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {6, 2};
//Attention, c est querty, voir dans la doc de la lib Keyboard pour passer en azerty ou alors faire la correspondance.
const char BUTTON_LETTER[NUM_BUTTONS] = {'q','b'};

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
    // If it fell, flag the need to toggle the LED
    if ( buttons[i].fell() ) {
      Keyboard.write(BUTTON_LETTER[i]);
    }
  }
}
