/*
    Sonnette

*/

#include <ESP8266WiFi.h>
#include <Timer.h>

const char* ssid     = "Wifi_Moulin";
const char* password = "BaptisteEtPauline";

const char* host = "192.168.0.47";
const int httpPort =  1880;

//PIN de la sonnette
const int PIN_SONNETTE = 5;

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
  pinMode(PIN_SONNETTE, INPUT_PULLUP);

  //Init des timers
  t.every(100, checkSonnette);
}

void loop(void) {
  delay(DELAY);
  t.update();
}

//On verifie l'etat de la sonnette
//Si qlq'un sonne, on appelle callServeur.
void checkSonnette() {
  int val = digitalRead(PIN_SONNETTE);  // read input value;
  if (val == LOW) {
    Serial.println("on sonne!!!");
    callServeur();
  }
}


//Appel du serveur pour dire que ça sonne
void callServeur() {
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/sonnette";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
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

