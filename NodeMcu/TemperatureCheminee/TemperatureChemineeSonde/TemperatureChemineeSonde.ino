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
const int DELAY = 5000;

//Config MQTT
const char* MQTT_CLIENT_ID = "cheminee-sonde1"; //Doit etre unique dans toute la maison.
const char* MQTT_TEMP_TOPIC = "temperature/cheminee/sonde1";

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient wifiClient;
PubSubClient client(wifiClient);

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
  if (!client.connected()) {
    reconnect();
  }
  delay(1000);
}

void loop(void) {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  float temperature = getTemperature(sondeTemperaturePiscine);
  Serial.println(temperature);
  
  sendTemperature(temperature);
  //period between posts, set at 30 minutes
  //Serial.printf("Sleeping deep for %i seconds...", sleepSeconds);
  //https://github.com/adafruit/ESP8266-Arduino
  //ESP.deepSleep(sleepSeconds * 1000000);
  delay(DELAY);
  //delay(5000);
}

//Envoi temperature passée en parametre sur le serveur MQTT
void sendTemperature(float temperature) {
  String payload = String(temperature);
  // Publish payload
  client.publish(MQTT_TEMP_TOPIC, (char*) payload.c_str());
  Serial.print("Send Data :");
  Serial.println(payload);
}

/*-----( Declare User-written Functions )-----*/
float getTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);

  if (tempC == -127.00) // Measurement failed or no device found
  {
    Serial.println("Temperature Error");
    nbrRetry++;
    if (nbrRetry <= 3) {
      delay(200);
      tempC = getTemperature(deviceAddress);
    } else {
      nbrRetry = 0;
      Serial.println("Temperature Error");
      //On donne la derniere valeur lue
       tempC = previousTemperatureLu;
    }
  }
  else
  {
    nbrRetry = 0;
    Serial.print("C=");
    Serial.println(tempC);
    previousTemperatureLu = tempC;
  }
  return tempC;
}// End printTemperature

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
