/*
    Mesure de temperature exterieur

*/
#include <ESP8266WiFi.h>
#include "DHT22adafruit.h"
// Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

//Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <DallasTemperature.h>

//30 minutes
const int sleepSeconds = 60*30;

const char* ssid     = "Wifi_Moulin";
const char* password = "BaptisteEtPauline";

#define HOST_NAME    "192.168.0.47"
const char* host = "192.168.0.47";
const int httpPort =  1880;

//Temps d'attente avant la loop.
const int DELAY = 1000;

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

float previousTemperaturePiscine = 0;
int nbrRetry = 0;

#define REQ_PREFIX    "POST /toMQTT\r\n" \
    "Host: " HOST_NAME "\r\n" \
    "Accept: *" "/" "*\r\n" \
    "Content-Length: " 

#define REQ_SUFFIX    "\r\n" \
    "Content-Type: application/x-www-form-urlencoded\r\n" \
    "Connection: close\r\n\r\n" 

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

  sensors.begin();
  sensors.setResolution(sondeTemperaturePiscine, 12);

  delay(DELAY);
}

void loop(void) {
  sendTemperature();
  //period between posts, set at 30 minutes
  Serial.printf("Sleeping deep for %i seconds...", sleepSeconds);
  //https://github.com/adafruit/ESP8266-Arduino
  ESP.deepSleep(sleepSeconds * 1000000);
  delay(100);
  //delay(5000);
}

//Envoi de la donnee
void sendTemperature() {
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  float temperature = getTemperature(sondeTemperaturePiscine);

  String data = "temperature=";
  data = data + temperature + "&t=/sensors/" + "piscine";
  Serial.print("data=");
  Serial.println(data);
  callServeur(data);
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
       tempC = previousTemperaturePiscine;
    }
  }
  else
  {
    nbrRetry = 0;
    Serial.print("C=");
    Serial.println(tempC);
    previousTemperaturePiscine = tempC;
  }
  return tempC;
}// End printTemperature

//Appel du serveur pour envoyee les donnees
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
  delay(2000);
  
}

