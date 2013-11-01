/*
  Sonde Temperature et Humidite
  Plus
  Thermometre qui va dans l'eau
 */
 

#include <OneWire.h>
#include <DallasTemperature.h>
#include <dht.h>

//Pour temp et humidite
dht DHT;
// Data wire is plugged into port 2 on the Arduino - Temp eau
#define ONE_WIRE_BUS 2
// TempEtHumidite
#define dht_dpin A0 //no ; here. Set equal to channel sensor is on

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

boolean isComToPi = true;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  Serial.println("Humidity and temperature\n\n");
  delay(1000);//Wait rest of 1000ms recommended delay before


  // Start up the library
  sensors.begin();
}

void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));  
    //This is the "heart" of the program.
  DHT.read11(dht_dpin);

  float temperature = DHT.temperature;
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(temperature); 
  Serial.println("C  ");
  delay(5000);//Don't try to access too frequently... in theory
  //should be once per two seconds, fastest,
  //but seems to work after 0.8 second.
}


void log(String text)
{

}
