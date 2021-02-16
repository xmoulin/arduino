/*
    Mesure de temperature 

*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//#include "DHT22adafruit.h"
// Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

//Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <DallasTemperature.h>

//30 minutes
const int sleepSeconds = 2000;//60*30;

const char* ssid     = "WIFI_BINOCHE";
const char* password = "binochette";

//NAS
#define HOST_NAME    "192.168.1.51"
const char* mqtt_server = HOST_NAME;
const int httpPort =  1880; //port du serveru MQTT

//Temps d'attente enter chaque tour de boucle.
const int DELAY = 50;

//Config MQTT
const char* MQTT_CLIENT_ID = "cheminee-sondeReception1"; //Doit etre unique dans toute la maison.
const char* MQTT_TEMP_TOPIC_SUB = "temperature/cheminee/+";

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient wifiClient;
PubSubClient client(wifiClient);

//
// DEFINITION DES SEUILS
//
const float SEUIL_TEMP_MIN = 25;
const float SEUIL_TEMP_MAX = 45;
const float SEUIL_PUISSANCE_MIN = 30;
const float SEUIL_PUISSANCE_MAX = 50;

//
//TEMPERATURES
//
//PIN des sondes de temperature
const int PIN_ONE_WIRE_BUS = 4;
/*-----( Declare objects )-----*/
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(PIN_ONE_WIRE_BUS);
// Pass address of our oneWire instance to Dallas Temperature.
DallasTemperature sensors(&oneWire);
DeviceAddress sondeTemperaturePiscine = { 0x28, 0xFF, 0x67, 0xEE, 0xA0, 0x16, 0x04, 0x85 }; // "1"
float previousTemperatureLu = 0;
int nbrRetry = 0;

void setup() {
  Serial.begin(9600);
  delay(10);
  //Setup Wifi
  wifiSetup();

  //Setup Sensors
  sensors.begin();
  sensors.setResolution(sondeTemperaturePiscine, 12);

  //Setup MQTT
  client.setServer(mqtt_server, 1883);
  //La methode callback sera la méthode appelée sur la reception d'un message dans le topic
  client.setCallback(topiccallback);

  if (!client.connected()) {
    reconnect();
  }
  delay(1000);
}

//On fait rien dans la loop...
//Les receptions de message se passent dans la méthode topiccallback
void loop(void) {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  //period between posts, set at 30 minutes
  //Serial.printf("Sleeping deep for %i seconds...", sleepSeconds);
  //https://github.com/adafruit/ESP8266-Arduino
  //ESP.deepSleep(sleepSeconds * 1000000);
  delay(DELAY);
  //delay(5000);
}

//Methode appelée a chaque reception de message dans le/les topic pour lesquels on est abonnée.
void topiccallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println(".");

  //Transformation du message reçu en texte vers un float.
  String temperatureString = String((char*)payload);
  float temperatureFloat = temperatureString.toFloat();
  Serial.print("temperatureFloat=");
  Serial.println(temperatureFloat);
  int puissanceVentillation;
  //Si la temperature est superieur au seuil minimum défini.
  if(temperatureFloat > SEUIL_TEMP_MIN) {
    //Si la temperature est supérieur au seuil MAX, on la fixe à la temp max histoire d'atteindre la puissance max.
    if (temperatureFloat >SEUIL_TEMP_MAX) {temperatureFloat = SEUIL_TEMP_MAX;}
    //On fait un produit en croix pour à partir de la temperature en deduire la puissance à appliquer à la ventillation
    puissanceVentillation = map(temperatureFloat, SEUIL_TEMP_MIN, SEUIL_TEMP_MAX, SEUIL_PUISSANCE_MIN, SEUIL_PUISSANCE_MAX);
  } else {
    puissanceVentillation = 0;
  }
  Serial.print("puissance sortie=");
  Serial.println(puissanceVentillation);
}

//Methode utilitaire pour se connecter au serveur MQTT
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = MQTT_CLIENT_ID;
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(MQTT_TEMP_TOPIC_SUB);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//Methode utilitaire pour se connecter au wifi
void wifiSetup() {
//Init Wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
