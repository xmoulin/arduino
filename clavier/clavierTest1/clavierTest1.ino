//Test 1 pour s assurer que ça fonctionne
//Doit afficher un 'x' toutes les 10 secondes
#include <Keyboard.h>
 
void setup() { // initialize the buttons' inputs:
  
  Serial.begin(9600);
  Keyboard.begin();
}
 
void loop() {
  Keyboard.write('x');
  delay(10000);
}
