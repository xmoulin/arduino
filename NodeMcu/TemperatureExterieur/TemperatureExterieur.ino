/*
    Sonde de temperature exterieur (temperature / humidité)

*/

#include <ESP8266WiFi.h>
#include <Timer.h>
#include "DHT22adafruit.h"
#define DHTPIN 5     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "Wifi_Moulin";
const char* password = "BaptisteEtPauline";

const char* host = "192.168.0.47";
#define HOST_NAME    "192.168.0.47"

const int httpPort =  1880;

#define PIECE         "ext" //"salon" //chambre
#define DHT22_PIN 6


#define REQ_PREFIX    "POST /toMQTT\r\n" \
    "Host: " HOST_NAME "\r\n" \
    "Accept: *" "/" "*\r\n" \
    "Content-Length: " 

#define REQ_SUFFIX    "\r\n" \
    "Content-Type: application/x-www-form-urlencoded\r\n" \
    "Connection: close\r\n\r\n" 


//Temps d'attente avant la loop.
const int DELAY = 50;

//Un timer
Timer t;

void setup() {
  Serial.begin(9600);
  delay(10);

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

  //Init des pin
  pinMode(DHT22_PIN, INPUT_PULLUP);

  //Init des timers
  t.every(2000, sendTemperature);
}

void loop(void) {
  delay(DELAY);
  t.update();
}

void sendTemperature() {


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  String data = "temperature=";
  data = data + t + "&humidity=" + h + "&t=/sensors/" + PIECE;
  Serial.print("data=");
  Serial.println(data);

  //callServeur(data);
}



//Appel du serveur pour dire que รงa sonne
void callServeur(String data) {
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("Requesting URL: ");
  Serial.println(data);

  Serial.print("Full HTTP stack");
  Serial.println(REQ_PREFIX + String(data.length()) + REQ_SUFFIX + data);
  
  // This will send the request to the server
  client.print(REQ_PREFIX + String(data.length()) + REQ_SUFFIX + data);
  
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
  delay(20000);
}

