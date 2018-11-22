/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <SoftwareSerial.h>
#include "DHT.h"

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

//CO2
#define MH_Z19_RX 13
#define MH_Z19_TX 15
SoftwareSerial co2Serial(MH_Z19_RX, MH_Z19_TX); // define MH-Z19

//Temp & Humidité
#define DHTPIN 5     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

//30 minutes
const int sleepSeconds = 60*30;
/****************************** Feeds ***************************************/

// Setup a feed called 'sensorTopic' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish sensorTopic = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/sensors/haut");

// Setup a feed called 'onoff' for subscribing to changes.
//Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/test/onoff");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

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

  // Setup MQTT subscription for onoff feed.
  //mqtt.subscribe(&onoffbutton);
  co2Serial.begin(9600); //Init sensor MH-Z19(14)
  dht.begin();
  delay(500);
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  ///Adafruit_MQTT_Subscribe *subscription;
  //while ((subscription = mqtt.readSubscription(5000))) {
  //  if (subscription == &onoffbutton) {
  //    Serial.print(F("Got: "));
  //    Serial.println((char *)onoffbutton.lastread);
  //  }
  //}

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  Serial.println("Requesting CO2 concentration...");
  int ppm = readCO2();

  // Now we can publish stuff!
  //Surrement plus simple que de passer par un String pour finir en tableau mais bon..
  char dataChar[80];
  String data = "{\"temperature\":\"";
  data = data + t + "\",\"humidity\":\"" + h + "\",\"co2\":\"" + ppm + "\",\"t\":\"/sensors/haut\"}";
  //{"temperature":"24.40","humidity":"69.30","co2"="1887"&"t":"/sensors/haut"}

  data.toCharArray(dataChar, 79);
  
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

int readCO2() {
  // D'après le code original de | From original code https://github.com/jehy/arduino-esp8266-mh-z19-serial
  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  // command to ask for data
  byte response[9]; // for answer
 
  co2Serial.write(cmd, 9); //request PPM CO2
 
  // The serial stream can get out of sync. The response starts with 0xff, try to resync.
  while (co2Serial.available() > 0 && (unsigned char)co2Serial.peek() != 0xFF) {
    co2Serial.read();
  }
 
  memset(response, 0, 9);
  co2Serial.readBytes(response, 9);
 
  if (response[1] != 0x86)
  {
    Serial.println("Invalid response from co2 sensor!");
    return -1;
  }
 
  byte crc = 0;
  for (int i = 1; i < 8; i++) {
    crc += response[i];
  }
  crc = 255 - crc + 1;
 
  if (response[8] == crc) {
    int responseHigh = (int) response[2];
    int responseLow = (int) response[3];
    int ppm = (256 * responseHigh) + responseLow;
    return ppm;
  } else {
    Serial.println("CRC error!");
    return -1;
  }
}
