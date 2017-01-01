#include <WS2812FX.h>
#include <EEPROM.h>

#define NBRLED 24
#define LEDP 9

WS2812FX ws2812fx = WS2812FX(NBRLED, LEDP, NEO_GRB + NEO_KHZ800);

const int BOUTON_1_PIN = 12;

int compteur;
uint8_t mode;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BOUTON_1_PIN, INPUT_PULLUP);

  Serial.begin(115200);

  // lecture du dernier mode en eeprom
  EEPROM.get(0, mode);

  // initialisation leds
  ws2812fx.init();
  ws2812fx.setBrightness(255);
  ws2812fx.setSpeed(500);
  ws2812fx.setColor(255, 0, 0);
  // donnée en eeprom valide ?
  if (mode <  MODE_COUNT)
    ws2812fx.setMode(mode);
  else
    ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();
}

void loop() {
  int val = testBouton();

  ws2812fx.service();

  if (val != -1) {
    compteur++;

    // oui, on prend en compte le compteur
    Serial.print("compteur=");
    Serial.println(compteur);
    switch (compteur) {
      case 1:
        mode = FX_MODE_STATIC;
        break;
      case 2:
        mode = FX_MODE_FADE;
        break;
      case 3:
        mode = FX_MODE_FIREWORKS;
        break;
      case 4:
        mode = FX_MODE_SPARKLE;
        break;
      case 5:
        mode = FX_MODE_RUNNING_LIGHTS;
        //On remet à 0 pour passer à 1 lorsd du prochain clique
        compteur = 0;
        break;
    }
    ws2812fx.setMode(mode);
    EEPROM.put(0, mode);
  } /*else {
        // dans la seconde
        if(val > 6000) {
          Serial.print("val=");
          Serial.println(val);
          digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
          compteur++;
          delay(180); // debounce
        }*/
}




//fonction qui teste l'état des boutons et retourne le numéro de bouton
int testBouton() {
  int bouton = -1;
  int val = digitalRead(BOUTON_1_PIN);  // read input value;
  int valDelay = 20;
  delay(valDelay); //cf http://arduino.cc/en/Tutorial/AnalogInputPins have to wait a little before reading another value
  if (val == LOW) {         
    bouton = 1;
    delay(1000);
    Serial.println("bouton=1");
  }
  
  return bouton;
}
