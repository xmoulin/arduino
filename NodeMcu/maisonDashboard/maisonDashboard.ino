/**
 * Pensez a selection, dans la catégorie esp8266, la carte LOLIN(Wemos)D1 R2 & mini
 */
#include <ESP8266WiFi.h> 
//Librairie ici (elle se trouve dans le gestionnaire de librairie): https://github.com/knolleary/pubsubclient
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <FastLED.h>
//Conf de l'Objet
#include "maisonDashboard_config.h"

/************ RUBAN LED ******************/
// How many leds are in the strip?
#define NUM_LEDS 18
// Data pin that led data will be written out over
#define DATA_PIN 5
// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

/************ LED CO2 / Humidity ******************/
//Ordre de PIN : Rouge/Bleu
int myPinsLedCo2[] = {D5, D7};
int myPinsLedHumidity[] = {D2, D4};
int myPinsLedCo2Value[] = {HIGH,HIGH};
int myPinsLedHumidityValue[] = {HIGH,HIGH};
bool color_set = LOW;

///Cloud Connecting
int led_nb_connecting = 0;
unsigned long time_connecting = 0;
int delay_connecting = 100; //in ms

// ledRubanMaisonInit
int led_nb_pairing1 = NUM_LEDS;

/************ MQTT - Global State (you don't need to change this!) ******************/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient wifiClient;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

//new lib MQTT (Exit Adafruit from this commit : 16/06/2020)
PubSubClient client(wifiClient);
const char* mqtt_server = MY_MQTT_SERVER;

/************ TEMP - Global value ******************/
float tempSalon =0;
float tempHaut =0;
float tempExt =0;
int co2 = 0;
boolean isSystemeInitialise = false; // Une fois qu'on a reçu 3 valeurs salon, haut, ext, on  peut dire que le systeme est initialisé. On pourrait faire des topics persistent MQTT, mais abonnement qu a un topic global...
//Etat utile au ruban de LED
String etatMaison = "Init";
//Mode de fonctionnement du dashboard : demo, start, stop
String dashBoardMode = "start";

/*************************** Sketch Code ************************************/
void setup() {
  Serial.begin(115200);
  delay(10);
  randomSeed(micros());

  Serial.println(F("Cloud Switch INIT"));
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  //On lance les LED lors de l'init. a voir si le call HTTP et MQTT ne fait pas trop lagguer la chose, sinon mettre en fin de setup.
  ledManagement();

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(topiccallback);

  if (!client.connected()) {
    reconnect();
  }

  //Init des PIN out pour tous les PIN utilisés
  for (int i = 0; i< 2;i ++ ){
    pinMode(myPinsLedCo2[i],OUTPUT);
    pinMode(myPinsLedHumidity[i],OUTPUT);
  }
  updateLed();
  Serial.println("Setup OK");

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      //subscribe
      client.subscribe(SENSORS_TOPIC_ALL_SUB);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  ledManagement();
}

/**********************************************/
/************ MQTT MANAGEMENT ******************/
/**********************************************/
void topiccallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  
  String temperatureAsString = root[String("temperature")].as<String>();
  Serial.print("Temperature is: ");
  Serial.println(temperatureAsString);
  feadANewTemperatureValue(topic,temperatureAsString);

  String co2AsString = root[String("co2")].as<String>();
  Serial.print("CO2 is: ");
  Serial.println(co2AsString);
  feadANewCO2Value(topic,co2AsString);

  String humidityAsString = root[String("humidity")].as<String>();
  Serial.print("Humidity is: ");
  Serial.println(humidityAsString);
  feadANewHumidityValue(topic,humidityAsString);
  
  String modeAsString = root[String("mode")].as<String>();
  Serial.print("mode is: ");
  Serial.println(modeAsString);
  feadANewModeValue(topic,modeAsString);
  
  logAllValue();
  manageSystemInitialisation();
  updateLed();
  updateLedRuban();
  //Apres avoir positionner les etat, si on est en stop, il faut les surcharger
  manageSystemMode();
}


/**********************************************/
/************ Value Management ****************/
/**********************************************/
//Arrivée d'une nouvelle temperature
void feadANewTemperatureValue(char* topic, String temperature) {
  //On se base sur la 1ere lettre du topic pour savoir si c'est le salon, haut ou exterieur. Pas classe mais pour l'instant, ça fait le job
  if(topic[9]=='s') {
    tempSalon=temperature.toFloat();
  } else if(topic[9]=='h') {
    tempHaut=temperature.toFloat();
  } else if(topic[9]=='e') {
    tempExt=temperature.toFloat();
  } 
}

//Arrivée d'une nouvelle valeur de co2
void feadANewCO2Value(char* topic, String co2New) {
  if(topic[9]=='h') {
    co2=co2New.toInt();
    Serial.print("CO2 Haut: ");
    Serial.println(co2);
    ledCO2Management(co2);
  } 
}

//Arrivée d'une nouvelle valeur d'humidité
void feadANewHumidityValue(char* topic, String humidityNew) {
 if(topic[9]=='h') {
    float humidity=humidityNew.toFloat();
    Serial.print("Humidité Haut: ");
    Serial.println(humidity);
    ledHumidityManagement(humidity);
 }
}

//Arrivée d'une nouvelle valeur mode d'utilisation
void feadANewModeValue(char* topic, String modeNew) {
 if(topic[9]=='m') {
    dashBoardMode = modeNew;
    Serial.print("mode: ");
    Serial.println(modeNew);
 }
}

void ledCO2Management(int co2) {
   if (co2 >= SEUIL_CO2_DANGER) {
    Serial.println("Seuil CO2 Danger");
    setColorToRed(myPinsLedCo2Value);
  } else if (co2 >= SEUIL_CO2_WARN) {
    Serial.println("Seuil CO2 Warn");
    setColorToPink(myPinsLedCo2Value);
  } else {
    setColorToBlue(myPinsLedCo2Value);
  }
}

void ledHumidityManagement(float humidity) {
   if (humidity >= SEUIL_HUMIDITY_DANGER) {
    Serial.println("Seuil Humidity Danger");
    setColorToRed(myPinsLedHumidityValue);
  } else if (humidity >= SEUIL_HUMIDITY_WARN) {
    Serial.println("Seuil Humidity Warn");
    setColorToPink(myPinsLedHumidityValue);
  } else {
    setColorToBlue(myPinsLedHumidityValue);
  }
}
//pour verifier si on a reçu les données de toutes les sondes. A ce moment, on va pouvoir faire des comparaisons.
//On pourrait fairee du retain sur les topics histoire de ne pas avoir a faire cela.
//Pour autant, comme on debranche pas le system, on devrait pas souvent rentrer dans le cas du non init.
void manageSystemInitialisation() {
  //Systeme initialisé?
  if (tempHaut != 0 && tempExt != 0 ) {
    isSystemeInitialise = true;
    Serial.println("****** Systeme INITIALISE ****** ");
  } 
}

//Si on est en mode stop ou demo, on fait des trucs particuliers
void manageSystemMode() {
  if (dashBoardMode.equals("stop")) {
    etatMaison="Pareil";
    setAllColorToBlack();
  } else if (dashBoardMode.equals("demo")) {
    runDemoScenario();
  }
}
void logAllValue() {
  Serial.print("Temperature Salon: ");
  Serial.print(tempSalon);
  Serial.print(",Temperature Haut: ");
  Serial.print(tempHaut);
  Serial.print(",Temperature Exterieur: ");
  Serial.print(tempExt);
  Serial.print(",CO2 Haut: ");
  Serial.print(tempSalon);
  Serial.print(",mode: ");
  Serial.println(dashBoardMode);
}

/**********************************************/
/****** LED MANAGEMENT CO2/Humidity************/
/**********************************************/

// This function updates the LED outputs.
void updateLed()
{ 
    for (int i = 0; i<2; i++){
      digitalWrite(myPinsLedCo2[i],myPinsLedCo2Value[i]);
      digitalWrite(myPinsLedHumidity[i],myPinsLedHumidityValue[i]);
    }
}

void setColorToRed(int ledValue[]) {
  ledValue[0] = LOW;
  ledValue[1] = HIGH;
}

void setColorToBlue(int ledValue[]) {
  ledValue[0] = HIGH;
  ledValue[1] = LOW;
}

void setColorToPink(int ledValue[]) {
  ledValue[0] = LOW;
  ledValue[1] = LOW;
}

void setColorToBlack(int ledValue[]) {
  ledValue[0] = HIGH;
  ledValue[1] = HIGH;
}

void setAllColorToBlack() {
    setColorToBlack(myPinsLedCo2Value);
    setColorToBlack(myPinsLedHumidityValue);
}
/**********************************************/
/**************** Demo ************************/
/**********************************************/
void runDemoScenario(){
    Serial.println("---- DEMO START ----");
    //Commence par les humidité et Co2
    String previousEtatMaison = etatMaison;
    etatMaison="Pareil";
    privateRunDemoLed(100);
    setAllColorToBlack();
    updateLed();
    delay(2000);
    ledHumidityManagement(SEUIL_HUMIDITY_WARN);
    updateLed();
    delay(2000);
    ledHumidityManagement(SEUIL_HUMIDITY_DANGER);
    updateLed();
    delay(2000);
    setAllColorToBlack();
    ledCO2Management(SEUIL_CO2_WARN);
    updateLed();
    delay(2000);
    ledCO2Management(SEUIL_CO2_DANGER);
    updateLed();
    delay(2000);
    setAllColorToBlack();
    updateLed();
    //Gestion de la barre de LED
    etatMaison="TresChaud";
    privateRunDemoLed(5000);
    etatMaison="Chaud";
    privateRunDemoLed(5000);
    etatMaison="Pareil";
    privateRunDemoLed(5000);
    etatMaison="Frais";
    privateRunDemoLed(5000);
    etatMaison="TresFrais";
    privateRunDemoLed(5000);
    etatMaison=previousEtatMaison;
    dashBoardMode="start";
    Serial.println("---- DEMO STOP ----");
}
//Gere le ruban de LED, passe en parametre le temps que doit durer la demo
void privateRunDemoLed(int demoDuration){
      unsigned long demoStartTime = millis();
      color_set = LOW;
      while(millis()- demoStartTime <= demoDuration) {
        ledManagement();
        delay(10);
      }
}

/**********************************************/
/********** Ruban LED MANAGEMENT **************/
/**********************************************/

void updateLedRuban() {
  //Si pas initialisé
  if (!isSystemeInitialise) {
    etatMaison="Init";
  } else {
    //Si temperature exterieur  > temperature maison + 3
    if (tempExt > tempHaut + 3) {
      Serial.println("MAISON - Tres frais dans la maison");
      etatMaison="TresFrais";
    } else if (tempExt > tempHaut + 1) {
      Serial.println("MAISON - frais dans la maison");
      etatMaison="Frais";
    } else if (tempExt > tempHaut - 1 ) {
      Serial.println("MAISON - presque pareil");
      etatMaison="Pareil";
    } else if (tempExt > tempHaut - 3) {
      Serial.println("MAISON - chaud dedans");
      etatMaison="Chaud";
    } else {
      Serial.println("MAISON - tres chaud dedans");
      etatMaison="TresChaud";
    }
  }
  color_set = LOW;
}

void ledManagement() {
  if (etatMaison.equals("TresFrais")) {
    ledRubanMaisonFrais();
  }
  else if (etatMaison.equals("Frais")) {
    Fire2012();    
  }
  else if (etatMaison.equals("Pareil")) {
    ledRubanMaisonNeutre();
  }
  else if (etatMaison.equals("Chaud")) {
    pacifica_loop();
  }
  else if (etatMaison.equals("TresChaud")) {
    ledRubanMaisonChaud();
  }
  else { //Init
    ledRubanMaisonInit();
  }
}

void ledRubanMaisonInit() {
  if (time_connecting + delay_connecting< millis()){      
      //Serial.println("*-*-*-* LED - ledRubanMaisonInit");

      if (led_nb_pairing1 < NUM_LEDS/2){
        leds[led_nb_pairing1 % NUM_LEDS] = CRGB::Green;
        leds[(NUM_LEDS-led_nb_pairing1) % NUM_LEDS] = CRGB::Green;
      }
      if (led_nb_pairing1 == NUM_LEDS-1){
        leds[led_nb_pairing1 % NUM_LEDS] = CRGB::Green;
      }
      if (led_nb_pairing1 > 0){
        leds[mod(led_nb_pairing1 - 2 , NUM_LEDS)] = CRGB::Black;
        leds[mod(NUM_LEDS-led_nb_pairing1 + 2 , NUM_LEDS)] = CRGB::Black;
      }
      led_nb_pairing1 = mod(led_nb_pairing1 + 1, NUM_LEDS);
      
      // Show the leds
      FastLED.show();
      time_connecting = millis();
    }
}

void ledRubanMaisonNeutre() {
  if (!color_set) {
    Serial.println("*-*-*-* LED - ledRubanMaisonNeutre");
    for (int i = 0; i < NUM_LEDS; i ++ )
    {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    color_set = HIGH;
  }
}

void ledRubanMaisonChaud() {
  if (!color_set) {
    Serial.println("*-*-*-* LED - ledRubanMaisonChaud");
    for (int i = 0; i < NUM_LEDS; i ++ )
    {
      leds[i] = CRGB::Blue;
    }
    FastLED.show();
    color_set = HIGH;
  }
}

void ledRubanMaisonFrais() {
  if (!color_set) {
    Serial.println("*-*-*-* LED - ledRubanMaisonFrais");
    for (int i = 0; i < NUM_LEDS; i ++ )
    {
      leds[i] = CRGB::Red;
    }
    FastLED.show();
    color_set = HIGH;
  }
}

int mod( int x, int y ) {
  return x < 0 ? ((x + 1) % y) + y - 1 : x % y;
}

///////////////////
//// PACIFIQUE ////
///////////////////
 
CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

void pacifica_loop()
{
   if (time_connecting + delay_connecting < millis()) {

      // Increment the four "color index start" counters, one for each wave layer.
      // Each is incremented at a different speed, and the speeds vary over time.
      static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
      static uint32_t sLastms = 0;
      uint32_t ms = GET_MILLIS();
      uint32_t deltams = ms - sLastms;
      sLastms = ms;
      uint16_t speedfactor1 = beatsin16(3, 179, 269);
      uint16_t speedfactor2 = beatsin16(4, 179, 269);
      uint32_t deltams1 = (deltams * speedfactor1) / 256;
      uint32_t deltams2 = (deltams * speedfactor2) / 256;
      uint32_t deltams21 = (deltams1 + deltams2) / 2;
      sCIStart1 += (deltams1 * beatsin88(1011,10,13));
      sCIStart2 -= (deltams21 * beatsin88(777,8,11));
      sCIStart3 -= (deltams1 * beatsin88(501,5,7));
      sCIStart4 -= (deltams2 * beatsin88(257,4,6));
    
      // Clear out the LED array to a dim background blue-green
      fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));
    
      // Render each of four layers, with different scales and speeds, that vary over time
      pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
      pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
      pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
      pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));
    
      // Add brighter 'whitecaps' where the waves lines up more
      pacifica_add_whitecaps();
    
      // Deepen the blues and greens a bit
      pacifica_deepen_colors();
     // Show the leds
      FastLED.show();
      time_connecting = millis();
   }
}

// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps()
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors()
{
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}

/////////////
/// FIRE ////
/////////////

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 100

void Fire2012()
{
  if (time_connecting + delay_connecting < millis()) {
    // Array of temperature readings at each simulation cell
    static byte heat[NUM_LEDS];
  
    // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      //if( gReverseDirection ) {
      //  pixelnumber = (NUM_LEDS-1) - j;
      //} else {
        pixelnumber = j;
      //}
      leds[pixelnumber] = color;
    }

    // Show the leds
    FastLED.show();
    time_connecting = millis();
  }
}
