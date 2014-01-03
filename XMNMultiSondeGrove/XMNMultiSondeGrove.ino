#include <Servo.h>

/*
  Sonde de l'open space
 */
#include <DHT22.h>
#include <XMNData.h>
//#include <aJSON.h>
#include <MemoryFree.h>
#include <Timer.h>
#include <AirQuality.h>
#include <Arduino.h>
//Utile pour le NFC
#include <SPI.h>
#include <MFRC522.h>

//Pour temp et humidite
dht DHT;
// TempEtHumidite - DHT22
#define DHT22_PIN 6
// Lumiere - Grove Light Sensor
#define LUMIERE_PIN 1
// Son - Grove Sound Sensor
#define SON_PIN 2

//AirQuality
AirQuality airqualitysensor;

//Mes variables globales
const boolean isComToPi = true;
const int DELAY = 50;
//Nombre d'iteration globale
unsigned int iteration=0;
//Nombre d'iteration avant prise de lumiere/temperature/humidité
unsigned int iterationSon=0;
XMNData *xmnData;
unsigned int soundInstantane=0;
//Constante MAX du son - on initialise le soundMin au max afin de pouvoir le recalibrer simplement
const int SOUND_MAX = 80;
unsigned int sonMin=SOUND_MAX;
unsigned int sonMax=0;
unsigned int sonMoy=0;
// sauvegarde des derniere valeur comme ça si incoherence on redonne les dernieres captures
XMNData *previousXmnData;

//Un timer
Timer t;

/* NFC
 ----------------------------------------------------------------------------- Nicola Coppola
 * Pin layout should be as follows:
 * Signal     Pin              Pin               Pin
 *            Arduino Uno      Arduino Mega      MFRC522 board
 * ------------------------------------------------------------
 * Reset      9                5                 RST
 * SPI SS     10               53                SDA
 * SPI MOSI   11               52                MOSI
 * SPI MISO   12               51                MISO
 * SPI SCK    13               50                SCK
 *
 * The reader can be found on eBay for around 5 dollars. Search for "mf-rc522" on ebay.com. 
 * https://github.com/miguelbalboa/rfid/tree/master/examples
 */
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.
byte previousNfcUID[7];
byte nfcUID[7];
int nfcUIDLength;

void setup(void)
{
  // Activation du moduule RF
  pinMode(8, OUTPUT);    // initialize pin 8 to control the radio
  digitalWrite(8, HIGH); // select the radio
  Serial.begin(115200);    //Pour XIno RF 115200 coté arduino, 9600 coté raspberry pour lecture
  Serial.println("SETUP - Initialisation des sondes");
  Serial.print("SETUP - Humidity and temperature DHT22 - LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println("SETUP - Air Quality - Init");
  airqualitysensor.init(14);
  //Pour le NFC
  Serial.println("SETUP - NFC - Init");
  //SPI.begin();                        // Init SPI bus
  //mfrc522.PCD_Init();        // Init MFRC522 card
  Serial.println("SETUP - Scan PICC to see UID and type...");

  Serial.println("SETUP - delay puis init timer");
  delay(1000);//Wait rest of 1000ms recommended delay before

  t.every(100, getSon);
  //t.every(100, checkNFCCard);
  //t.every(5000, resetNfcPreviousValue);
  t.every(5000, captureEtEnvoi);
  //Initilisation de la structure capturant les valeurs de la derniere iteration;
  previousXmnData = new XMNData(0);
}

void loop(void)
{ 
  delay(DELAY);//Don't try to access too frequently... in theory
  //should be once per two seconds, fastest,
  //but seems to work after 0.8 second.
   t.update();
}


//Capture, aggrege et envoi
void captureEtEnvoi()
{
  //Si on arrive a 64.000 iteration, on repart a 1
  iteration++;
  if (iteration == 64000) {
    iteration = 1;
  }

  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
  Serial.print("captureEtEnvoi - Iteration ");
  Serial.println(iteration);
  xmnData = new XMNData(iteration);
  setCapteurLumiere();  
  setCapteurTemperatureHumidite();
  setSon();  
  setGaz();
  Serial.print("Lumiere=");
  Serial.println(xmnData->getLumiere());
  //On applique les seuils pour ne pas avoir des points hors du gabarit de la courbe
  xmnData->appliquerSeuil(previousXmnData);
  Serial.print("Lumiere=");
  Serial.println(xmnData->getLumiere());
  Serial.println("Flux JSON=");
  char* buf = xmnData->getJSON();
  Serial.println(buf);
  //free(buf);
  delete previousXmnData;
  previousXmnData = xmnData;
}

//Entre 0 et 80db sur l'IHM
void getSon()
{
  iterationSon++;
  int sonInstantane = analogRead(SON_PIN);
  //Serial.print("Son=");
  //Serial.println(sonInstantane);
  //Positionne le max
  if (sonInstantane >= sonMax) sonMax = sonInstantane;
  //Positionne le min
  if (sonInstantane <= sonMin) sonMin = sonInstantane;
  //Positionne le moy : par addition, une division sera faite au moment de calculer la moyenne en se bassant sur la variable iterationSon
  sonMoy = sonMoy + sonInstantane;
}

//Entre 0 et 80db sur l'IHM
void setSon()
{
  xmnData->setSonMin(sonMin);  
  xmnData->setSonMoy(sonMoy/iterationSon);
  xmnData->setSonMax(sonMax);
  //Reinit des valueurs min et max sauvegardée
  sonMin=SOUND_MAX;
  sonMax=0;
  sonMoy=0;
  iterationSon=0;
}

//Entre 0 et 200Lux sur l'IHM
void setCapteurLumiere()
{
  int sensorValue = analogRead(LUMIERE_PIN);
  int lux=sensorValue/5;
  if (lux > 200) lux = 200;
  Serial.print("setCapteurLumiere - the analog read data is ");
  Serial.print(sensorValue);
  Serial.print(",the sensor resistance is ");
  Serial.println(lux);
  xmnData->setLumiere(lux);
}

//Humidité entre 10 et 90% sur l'IHM
//Temperature entre 5 et 35°C sur l'IHM
void setCapteurTemperatureHumidite()
{
  int chk = DHT.read22(DHT22_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
		//Serial.println("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.println("setCapteurTemperatureHumidite - Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.println("setCapteurTemperatureHumidite - Time out error,\t"); 
		break;
    default: 
		Serial.println("setCapteurTemperatureHumidite - Unknown error,\t"); 
		break;
  }
  // DISPLAY DATA
  float temperature = DHT.temperature;
  float humidity = DHT.humidity;
  Serial.print("setCapteurTemperatureHumidite - DHT.humidity:");
  Serial.print(temperature, 1);
  Serial.print(",\t");
  Serial.print("DHT.temperature:");
  Serial.println(humidity, 1);

  //On sauvegarde les valeurs
  xmnData->setTemperature(temperature);
  xmnData->setHumidity(humidity);
}

//Entre 0 et 4 sur l'IHM
void setGaz()
{
  int current_quality=airqualitysensor.slope();
  if (current_quality >= 0)// if a valid data returned.
  {
    if (current_quality==0)
      Serial.println("setGaz - High pollution! Force signal active");
    else if (current_quality==1)
      Serial.println("setGaz - High pollution!");
    else if (current_quality==2)
      Serial.println("setGaz - Low pollution!");
    else if (current_quality ==3)
      Serial.println("setGaz - Fresh air");
  }
  xmnData->setGaz(current_quality);
}
ISR(TIMER2_OVF_vect)
{
  if(airqualitysensor.counter==122)//set 2 seconds as a detected duty
  {
    airqualitysensor.last_vol=airqualitysensor.first_vol;
    airqualitysensor.first_vol=analogRead(A0);
    airqualitysensor.counter=0;
    airqualitysensor.timer_index=1;
    PORTB=PORTB^0x20;
  }
  else
  {
    airqualitysensor.counter++;
  }
}

////////////////////////////////
///           NFC
////////////////////////////////
//Check si une carte NFC est présente. 
//Si oui, stock son numero dans le prochain packet à envoyer au raspberryPi
//Si plusieurs carte dans les meme delais de X secondes, 
void checkNFCCard()
{
         // Look for new cards
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
               //Serial.println("PICC_IsNewCardPresent");
               return;
        }

        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial()) {
               //Serial.println("PICC_ReadCardSerial");
               return;
        }

        // Dump debug info about the card. PICC_HaltA() is automatically called.
        //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
        	
        int nfcUIDLength = mfrc522.uid.size;
        //Serial.print("mfrc522.uid.size=");
        //Serial.println(mfrc522.uid.size);
        
        for (byte count = 0; count < mfrc522.uid.size; count++) {
	  nfcUID[count]=mfrc522.uid.uidByte[count];
	}

        if (!isSameNFCCard()) {       
          Serial.print("NFC - Card UID:");
      	  for (byte i = 0; i < nfcUIDLength; i++) {
      		Serial.print(nfcUID[i] < 0x10 ? " 0" : " ");
      		Serial.print(nfcUID[i], HEX);
      	  } 
  	  Serial.println();
          copyActualNfcToPreviousNfc();
        }
}

//Verification si on est toujours sur la meme carte NFC
boolean isSameNFCCard() {
  for (byte i = 0; i < 7; i++) {
    if (previousNfcUID[i] != nfcUID[i]) { 
      Serial.print("NFC - isSameNFCCard KO");
      return false;
    }
  } 
  return true;
}

//Copie des données de la carte sur la zone memoire utile a stoquer les valeurs du precedent tour de boucle
void copyActualNfcToPreviousNfc() {
        Serial.println("NFC - nouvelle carte, on memorise");
        for (byte i = 0; i < 7; i++) {
	  previousNfcUID[i]=nfcUID[i];
	}
}

//Suppression des données sauvegardees, on accepte de nouveau n'importe quelle carte
void resetNfcPreviousValue() {
        Serial.println("NFC - reset NFC");
	for (byte i = 0; i < 7; i++) {
		previousNfcUID[i] = 0;
	} 
}

