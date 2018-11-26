/*
    Mesure de temperature exterieur
    - Deepsleep (GPIO16 needs to be tied to RST to wake from deepSleep.)

    25/11/2018
    - MQTT
    - OTA
    - Change capteur de DHT22 à BME280
    - Ajout capteur de pluie
*/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Wifi_Moulin"
#define WLAN_PASS       "BaptisteEtPauline"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "192.168.0.47"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    ""
#define AIO_KEY         ""

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_BME280 bme; // I2C

//30 minutes
const int sleepSeconds = 60 * 30;
//const int sleepSeconds = 10;
/****************************** Feeds ***************************************/

// Setup a feed called 'sensorTopic' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish sensorTopic = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/sensors/ext");
//Adafruit_MQTT_Publish sensorTopic = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/test/ext");

/*************************** Sketch Code ************************************/
// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT Temp EXT"));

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
  Serial.println(F("BME280 test"));

  bool status;
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  delay(500);
}

uint32_t x = 0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  float h = bme.readHumidity();
  // Read temperature as Celsius (the default)
  float t = bme.readTemperature();
  //Pressure
  float p = bme.readPressure() / 100.0F;

  // Rain - read the sensor on analog A0:
  int sensorReading = analogRead(A0);
  Serial.print("sensorReading=");
  Serial.println(sensorReading);
  ///On fait qu'un petit nombre soit une petite pluie
  int rain= map(sensorReading, 0, 1024, 1024, 0);

  // Now we can publish stuff!
  //Surrement plus simple que de passer par un String pour finir en tableau mais bon..
  char dataChar[100];
  String data = "{\"temperature\":\"";
  data = data + t + "\",\"humidity\":\"" + h + "\",\"pressure\":\"" + p + "\",\"rain\":\"" + rain + "\",\"t\":\"/sensors/ext\"}";
  //{"temperature":"24.40","humidity":"69.30","co2"="1887"&"t":"/sensors/haut"}

  data.toCharArray(dataChar, 99);

  Serial.print(F("Sending data:"));
  Serial.println(dataChar);
  if (! sensorTopic.publish(dataChar)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  delay(1000);
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
    if(! mqtt.ping()) {
    mqtt.disconnect();
    }
  */

  //period between posts, set at 30 minutes
  //Pin 16  à RST pour que cela fonctionne
  Serial.printf("Sleeping deep for %i seconds...", sleepSeconds);
  //https://github.com/adafruit/ESP8266-Arduino
  ESP.deepSleep(sleepSeconds * 1000000);
  delay(100);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
