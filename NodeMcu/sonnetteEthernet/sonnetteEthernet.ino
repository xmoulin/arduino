/*
  Web client

  This sketch connects to a website (http://www.google.com)
  using an Arduino Wiznet Ethernet shield.

  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13

  created 18 Dec 2009
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe, based on work by Adrian McEwen

*/

#include <SPI.h>
#include <Ethernet.h>
#include <Timer.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192,168,0,47);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)
const int httpPort =  1880;

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 98);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

//PIN de la sonnette
const int PIN_SONNETTE = 5;

//Temps d'attente avant la loop.
const int DELAY = 50;

//Un timer
Timer t;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // disable SD card if one in the slot
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }

  // print your local IP address:
  Serial.println();
  Serial.print("Arduino IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();

  // give the Ethernet shield a second to initialize:
  delay(1000);

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
  //if (val == LOW) {
  //PAPA: ligne a supprimer et remplacer par ton if. Supprime aussi le delay(30000)
  if (true) {delay(30000);
    Serial.println("on sonne!!!");
    callServeur();
    
  }
}

//Appel du serveur pour dire que รงa sonne
void callServeur() {
  Serial.print("connecting to ");
  Serial.println(server);
  // if you get a connection, report back via serial:
  if (client.connect(server, httpPort)) {
    Serial.println("connected!");
    // Make a HTTP request:
    client.println("GET /sonnette HTTP/1.1");
    client.println("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
    Serial.println("Appel reussi");
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }

  // On attend un retour 5 secondes.
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

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  delay(2000);
}

