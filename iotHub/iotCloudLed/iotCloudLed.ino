#include <FastLED.h>

// How many leds are in the strip?
#define NUM_LEDS 10

// Data pin that led data will be written out over
#define DATA_PIN 5

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

//Switch
#define buttonPin 15

//###### Global var #####
// Cloud state
int cloud_state = 0; // 0 : not connected,f 1 : pairing1 (green), 2 : pairing2 (green),  3: cloud connecting (clockwise blue), 4: cloud connected (blue), 5 : warning (blink red), 6 : cloud disconnecting (anti clockwise, red) 
bool color_set = LOW;

///Cloud Connecting
int led_nb_connecting = 0;
unsigned long time_connecting = 0;
int delay_connecting = 100; //in ms

// Switch Pairing
int led_nb_pairing1 = NUM_LEDS;
int led_nb_pairing2 = NUM_LEDS;

// Cloud warning
unsigned long time_warning = 0;
int delay_warning = 500; //in ms

//##### Debounce ####
// Variables will change:
int buttonState = 0;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
    delay(2000);
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    pinMode(buttonPin, INPUT_PULLUP);

    Serial.begin(9600);
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {
  
  int reading = digitalRead(buttonPin);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    lastButtonState = reading;
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      // only change cloud state if the new button state is HIGH
      if (buttonState == HIGH) {
        cloud_state = (cloud_state + 1) % 7;
        Serial.println(cloud_state);
        color_set = LOW;
      }
    }
  }
  switch (cloud_state) {
    case 0:
      Cloud_not_connected();
      break;
    case 1:
      Switch_pairing();
      break;
    case 2:
      Switch_pairing2();
      break;
    case 3:
      Cloud_connecting();
      break;
    case 4:
      Cloud_connected();
      break;  
    case 5:
      Cloud_warning();
      break;
    case 6:
      Cloud_disconnecting();
      break;  
    default:
      // statements
      cloud_state = 0;
      break;
  }
}

void Cloud_connecting(){
    if (time_connecting + delay_connecting< millis()){      
      led_nb_connecting = mod(led_nb_connecting + 1, NUM_LEDS);
      
      leds[led_nb_connecting] = CRGB::Blue;
      leds[mod(led_nb_connecting -2 , NUM_LEDS)] = CRGB::Black;
      // Show the leds
      FastLED.show();
      time_connecting = millis();
    }    
}

void Cloud_disconnecting(){
    if (time_connecting + delay_connecting< millis()){      
      led_nb_connecting = mod(led_nb_connecting - 1, NUM_LEDS);
      
      leds[led_nb_connecting] = CRGB::Green;
      leds[mod(led_nb_connecting + 2 , NUM_LEDS)] = CRGB::Black;
      // Show the leds
      FastLED.show();
      time_connecting = millis();
    }    
}

void Switch_pairing(){
  if (time_connecting + delay_connecting< millis()){      
      leds[led_nb_pairing1] = CRGB::Green;
      leds[mod(led_nb_pairing1 - 2 , NUM_LEDS)] = CRGB::Black;
      led_nb_pairing1 = mod(led_nb_pairing1 + 1, NUM_LEDS);
      
      leds[led_nb_pairing2] = CRGB::Green;
      leds[mod(led_nb_pairing2 + 2 , NUM_LEDS)] = CRGB::Black;
      led_nb_pairing2 = mod(led_nb_pairing2 - 1, NUM_LEDS);
      
      // Show the leds
      FastLED.show();
      time_connecting = millis();
    }
}

void Switch_pairing2(){
  if (time_connecting + delay_connecting< millis()){      
      if (led_nb_pairing1 < NUM_LEDS/2){
        leds[led_nb_pairing1 % NUM_LEDS] = CRGB::Green;
        leds[(8-led_nb_pairing1) % NUM_LEDS] = CRGB::Green;
      }
      if (led_nb_pairing1 == NUM_LEDS-1){
        leds[led_nb_pairing1 % NUM_LEDS] = CRGB::Green;
      }
      if (led_nb_pairing1 > 0){
        leds[mod(led_nb_pairing1 - 2 , NUM_LEDS)] = CRGB::Black;
        leds[mod(8-led_nb_pairing1 + 2 , NUM_LEDS)] = CRGB::Black;
      }
      led_nb_pairing1 = mod(led_nb_pairing1 + 1, NUM_LEDS);
      
      // Show the leds
      FastLED.show();
      time_connecting = millis();
    }
}


void Cloud_not_connected(){
  if (!color_set){
    for (int i = 0; i< NUM_LEDS;i ++ )
    {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    color_set = HIGH;
  }
}

void Cloud_connected(){
  if (!color_set){
    Serial.println("connected");
    for (int i = 0; i< NUM_LEDS;i ++ )
    {
      leds[i] = CRGB::Blue;
    }
    FastLED.show();
    color_set = HIGH;
    }
}

void Cloud_warning(){
  if (time_warning + delay_warning < millis()){
    for (int i = 0; i< NUM_LEDS;i ++ )
    {
      leds[i] = CRGB::Red;
    }
  }
  else {
    for (int i = 0; i< NUM_LEDS;i ++ )
    {
      leds[i] = CRGB::Black;
    }
  }
  if (time_warning + 2*delay_warning < millis()){
    time_warning = millis();
  }
  
  FastLED.show();
}

int mod( int x, int y ){
   return x<0 ? ((x+1)%y)+y-1 : x%y;
}
