/*
 * Read MZ-Z19 CO2 concentration using UART interface and publish value on Domoticz with HTTP request
 * Lecture de la concentration en CO2 d'un capteur MH-Z19 via le port série et plublication de la mesure sur un serveur Domoticz requete HTTP
 * projetsdiy.fr - diyprojects.io (december 2017) 
 */
//#include <Wire.h>
#include <SoftwareSerial.h>
//#include <WiFi.h>
//#include <HTTPClient.h>
// Pour une carte ESP8266 | For ESP8266 development board
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

 //1000 * 60 * 10
#define INTERVAL 600000 
#define MH_Z19_RX 13
#define MH_Z19_TX 15
 
long previousMillis = 0;
 
SoftwareSerial co2Serial(MH_Z19_RX, MH_Z19_TX); // define MH-Z19
 
// Parametres WIFI - WiFi settings
#define wifi_ssid "Wifi_Moulin_IoT"
#define wifi_password "XavierEtCaroline"
 
// Paramètres HTTP Domoticz - HTTP Domoticz settings
const char* host = "192.168.0.47";
const int   port = 1880;
#define IDX_mhz19   26
 
HTTPClient http;
 
void setup() {
  Serial.begin(115200);
 
  // Connexion au réseau WiFi, connexion aux sondes
  // Start WiFi connexion and probes
  setup_wifi();           
 
  // Démarrer la communication série avec le MH-Z19 - Start UART communication with MZ-Z19 sensor
  unsigned long previousMillis = millis();
  co2Serial.begin(9600); //Init sensor MH-Z19(14)
}
 
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < INTERVAL)
    return;
 
  previousMillis = currentMillis;
  Serial.print("Requesting CO2 concentration...");
  int ppm = readCO2();
  if (ppm != -1) {
    Serial.println("  PPM = " + String(ppm));
    String url = "/co2?capteurId=haut&co2=";
    url += String(ppm); 
    sendToNas(url);
  }
}
 
//Connexion au réseau WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
 
  WiFi.begin(wifi_ssid, wifi_password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connexion OK ");
  Serial.print("=> Addresse IP : ");
  Serial.print(WiFi.localIP());
}
 
 
void sendToNas(String url){
  Serial.print("Connecting to ");
  Serial.println(host);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  http.begin(host,port,url);
  int httpCode = http.GET();
    if (httpCode) {
      if (httpCode == 200) {
        String payload = http.getString();
        Serial.println("NAS response "); 
        Serial.println(payload);
      }
    }
  Serial.println("closing connection");
  http.end();
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
