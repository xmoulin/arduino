/*
  Sonde de l'open space
 */
#include <DHT22.h>
#include <XMNData.h>
#include <aJSON.h>
#include <MemoryFree.h>
#include <Timer.h>
//Utile pour lumiere
//#include <math.h>

//Pour temp et humidite
dht DHT;
// TempEtHumidite - DHT22
#define DHT22_PIN 6
// Lumiere - Grove Light Sensor
#define LUMIERE_PIN 1
// Son - Grove Sound Sensor
#define SON_PIN 2

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
//Un timer
Timer t;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Initialisation des sondes");
  Serial.print("Humidity and temperature DHT22 - LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  delay(1000);//Wait rest of 1000ms recommended delay before
  //Appel la methode setSon tous les 2 secondes.
  t.every(100, getSon);
  t.every(5000, captureEtEnvoi);
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

  //Serial.print("freeMemory()=");
  //Serial.println(freeMemory());
  Serial.print("Iteration ");
  Serial.println(iteration);
  xmnData = new XMNData(iteration);
  setCapteurLumiere();  
  setCapteurTemperatureHumidite();
  setSon();  
  Serial.println("Flux JSON=");
  char* buf = xmnData->getJSON();
  Serial.println(buf);
  free(buf);
  delete xmnData;
}

//Entre 0 et 80db sur l'IHM
void getSon()
{
  iterationSon++;
  int sonInstantane = analogRead(SON_PIN);
  Serial.print("Son=");
  Serial.println(sonInstantane);
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
  Serial.print("the analog read data is ");
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
		Serial.println("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.println("Time out error,\t"); 
		break;
    default: 
		Serial.println("Unknown error,\t"); 
		break;
  }
  // DISPLAY DATA
  Serial.print("DHT.humidity:");
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.print("DHT.temperature:");
  Serial.println(DHT.temperature, 1);
  //On sauvegarde les valeurs
  xmnData->setTemperature(DHT.temperature);
  xmnData->setHumidity(DHT.humidity);


}




