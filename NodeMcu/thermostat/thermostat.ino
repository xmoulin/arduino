/*
    Detecte quand le thermostat declenche
    Envoi l'info que sur changement d'etat
    Quand on a du 24V sur le fil d'alim, active le photocoupler, et passe à l'état bas la PIN_THERMOSTAT
    PIN_THERMOSTAT == LOW -> on ne chauffe pas
    PIN_THERMOSTAT == HIGH -> on chauffe
    isChauffageUp : boolean gardant le dernier état, pas defaut, il est à Haut).
*/
#include <ESP8266WiFi.h>

const char* ssid     = "Wifi_Moulin";
const char* password = "BaptisteEtPauline";

const char* host = "192.168.0.47";
const int httpPort =  1880;

//derniere etat de lecture
int lastReadEtatChaudiere = HIGH;

//PIN de la thermostat
const int PIN_THERMOSTAT = 5;

//Temps d'attente avant la loop.
const int DELAY = 50;

//2 minutes
const int sleepSeconds = 60*2;

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
  pinMode(PIN_THERMOSTAT, INPUT_PULLUP);
  delay(500);
  //On force la soumission lors du premier tour de boucle.
  lastReadEtatChaudiere = !digitalRead(PIN_THERMOSTAT);  // read input value;
  Serial.print("lastReadEtatChaudiere init=");
  Serial.println(lastReadEtatChaudiere);
  Serial.println("Setup OK");

}

void loop(void) {
  checkThermostat();
  //Serial.printf("Sleeping deep for %i seconds...", sleepSeconds);
  //https://github.com/adafruit/ESP8266-Arduino
  //ESP.deepSleep(sleepSeconds * 1000000);
  delay(1000);

}

//On verifie l'etat de du chauffage
void checkThermostat() {
  int val = digitalRead(PIN_THERMOSTAT);  // read input value;
  Serial.print("Val loop=");
  Serial.println(val);
  if (lastReadEtatChaudiere != val) {
    Serial.println("Changement d'etat");
    if (val == LOW) {
      Serial.println("ne chauffe pas");
    } else {
      Serial.println("chauffe");
    }
    //On affecte la valeur lu à notre variable d'etat
    lastReadEtatChaudiere = val;
    String data = "chauffe=";
    data = data + val; //+ "&humidity=" + h 
    callServeur(data);
  }
}

//Appel du serveur 
void callServeur(String data) {
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/chaudiere?"+data;

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




