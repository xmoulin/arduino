/**
  
*/
#include <ESP8266WiFi.h>
#include "PMS.h"
/*
 * Serial and Serial1 are completely different interfaces - one hasn’t got anything to do with the other!
While Serial (the only one that can be seen on Serial Monitor) communicates via the USB port and a Virtual COM protocol (hence you can actually use any baud rate - 
the speed will always be the same USB speed), Serial1 only communicates via the RX/TX pins and there the baud rate has to match the baud rate of your connected device
 * 
 */
PMS pms(Serial);
PMS::DATA data;

const char* ssid     = "Wifi_Moulin";
const char* password = "BaptisteEtPauline";

const char* host = "192.168.0.47";
const int httpPort =  1880;

//Temps d'attente avant la loop.
const int DELAY = 50;

//2 minutes
const int sleepSeconds = 60 * 2;


void setup() {
  Serial.begin(9600);
  delay(10);


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //Init des pin
  //pinMode(PIN_THERMOSTAT, INPUT_PULLUP);

  delay(500);

}

void loop(void) {
sendPoussiere();
delay(5000);
}


void sendPoussiere() {
 String url = "/poussiere?valeur=";
 if (pms.read(data))
  {
    /*Serial1.print("PM 1.0 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_1_0);

    Serial1.print("PM 2.5 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_2_5);

    Serial1.print("PM 10.0 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_10_0);

    Serial1.println();*/
    uint16_t value = data.PM_AE_UG_2_5;
    url = url + value;

  } else {
      url = url + "10";
  }

  callServeur(url);
}


//Appel du serveur
void callServeur(String url) {
  
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    
    return;
  }

  
  

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    
  }

  
  
  delay(2000);
}
