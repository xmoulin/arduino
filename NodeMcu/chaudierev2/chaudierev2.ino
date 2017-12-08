/**
  PIN
  - PIN_THERMOSTAT = 5; //activation du chauffage de la chaudiere
  - PIN_ONE_WIRE_BUS = 4; //sonde temperature
  - PIN_BALLON_CMD_CHAUFFE = 9; //cmd pour annuler le chauffage du ballon
  - PIN_BALLON_MNT_CHAUFFE = 10; //cmd pour monitorer si le ballon chauffe
 
  1)
    Detecte quand le thermostat declenche
    Envoi l'info que sur changement d'etat
    Quand on a du 24V sur le fil d'alim, active le photocoupler, et passe à l'état bas la PIN_THERMOSTAT
    PIN_THERMOSTAT == LOW -> on ne chauffe pas
    PIN_THERMOSTAT == HIGH -> on chauffe
    isChauffageUp : boolean gardant le dernier état, pas defaut, il est à Haut).

  2)
    Recupere la temperature des arrivées/departs du chauffage par le sol

  3)
    Recupere la temperature du ballon d'eau chaude.
*/
#include <ESP8266WiFi.h>
#include <Timer.h>

/*-----( Import needed libraries )-----*/
// Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

//Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <DallasTemperature.h>

const char* ssid     = "Wifi_Moulin";
const char* password = "BaptisteEtPauline";

const char* host = "192.168.0.47";
const int httpPort =  1880;

//
//THERMOSTAT
//
//derniere etat de lecture
int lastReadEtatChaudiere = HIGH;

//PIN de la thermostat
const int PIN_THERMOSTAT = 5;

//timer du thermostat
Timer timer_thermostat;


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

DeviceAddress chauffageSol01 = { 0x28, 0xFF, 0x52, 0x5E, 0x81, 0x16, 0x05, 0x23 }; // "1"
DeviceAddress chauffageSol02 = { 0x28, 0xFF, 0xCD, 0x38, 0x81, 0x16, 0x05, 0x62 }; // "2"
DeviceAddress ballon01 = { 0x28, 0xFF, 0xA8, 0x5F, 0xA1, 0x16, 0x05, 0x23 }; // "3"

//timer du thermostat
Timer timer_temperatureChauffageSol;
Timer timer_temperatureBallon;

float previousTemperatureDepart = 0;
float previousTemperatureRetour = 0;
float previousTemperatureBallon = 0;

int nbrRetry = 0;

//Temps d'attente avant la loop.
const int DELAY = 50;

//2 minutes
const int sleepSeconds = 60 * 2;

//
//BALLON
//
const int PIN_BALLON_CMD_CHAUFFE = 1; //cmd pour annuler le chauffage du ballon
const int PIN_BALLON_MNG_CHAUFFE = 3; //cmd pour monitorer si le ballon chauffe
Timer timer_pilotageBallon;
Timer timer_monitoringBallon;
boolean test = true;


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

  Serial.print("IP:");
  Serial.println(WiFi.localIP());

  Serial.println();
  //Init des pin
  pinMode(PIN_THERMOSTAT, INPUT_PULLUP);

  delay(500);
  //On force la soumission lors du premier tour de boucle.
  lastReadEtatChaudiere = !digitalRead(PIN_THERMOSTAT);  // read input value;
  Serial.print("lastReadEtatChaudiere init=");
  Serial.println(lastReadEtatChaudiere);

  //------- Initialize the Temperature measurement library--------------
  sensors.begin();
  // set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(chauffageSol01, 12);
  sensors.setResolution(chauffageSol02, 12);
  sensors.setResolution(ballon01, 12);


  //Init des timers
  timer_thermostat.every(1000, checkThermostat);
  timer_temperatureChauffageSol.every(30000, checkEvolutionTemperatureChauffageSol);
  timer_temperatureChauffageSol.every(5 * 60 * 1000, sendTemperatureChauffageSol); //5 minutes
  //timer_temperatureChauffageSol.every(20 * 1000, sendTemperatureChauffageSol); //20 secondes pour les tests
  timer_temperatureBallon.every(4 * 60 * 1000, sendTemperatureBallon); //4 minutes

  Serial.println("Setup OK");

}

void loop(void) {
  timer_thermostat.update();
  timer_temperatureChauffageSol.update();
  timer_temperatureBallon.update();
  //Serial.printf("Sleeping deep for %i seconds...", sleepSeconds);
  //https://github.com/adafruit/ESP8266-Arduino
  //ESP.deepSleep(sleepSeconds * 1000000);

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
    String data = "/chaudiere?chauffe=";
    data = data + val; //+ "&humidity=" + h
    callServeur(data);
    //On envoi aussi la temperature de l'eau sur chaque changement d'etat.
    sendTemperatureChauffageSol();
  }
}

//On verifie l'evolution de la temperature par le sol. S'il a bougée de + ou - 1 degree, on envoit
void checkEvolutionTemperatureChauffageSol() {
  float temperatureDepart = getTemperature(chauffageSol01);
  float temperatureRetour = getTemperature(chauffageSol02);
  if (temperatureDepart > (previousTemperatureDepart + 1) || temperatureDepart < (previousTemperatureDepart - 1) || temperatureRetour > (previousTemperatureRetour + 1) || temperatureRetour < (previousTemperatureRetour - 1 )) {
    Serial.println("Ecart de 1° de temperature, on archive");
    previousTemperatureDepart = temperatureDepart;
    previousTemperatureRetour = temperatureRetour;
    sendTemperatureChauffageSol();
  }
}

//On envoie la temperature du chauffage par le sol
void sendTemperatureChauffageSol() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  float temperatureDepart = getTemperature(chauffageSol01);
  float temperatureRetour = getTemperature(chauffageSol02);
  String data = "/chaudiere?temperatureDepart=";
  data = data + temperatureDepart + "&temperatureRetour=" + temperatureRetour;
  callServeur(data);
}

//On envoie la temperature du chauffage ballon
void sendTemperatureBallon() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  float temperature = getTemperature(ballon01);
  String data = "/ballon?temperature=";
  data = data + temperature;
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
      if (deviceAddress == chauffageSol01) {
        tempC = previousTemperatureDepart;
      } else if (deviceAddress == chauffageSol02) {
        tempC = previousTemperatureRetour;
      } else {
        tempC = previousTemperatureBallon;
      }
    }
  }
  else
  {
    nbrRetry = 0;
    Serial.print("C=");
    Serial.println(tempC);
  }
  return tempC;
}// End printTemperature

//Appel du serveur
void callServeur(String url) {
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

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




