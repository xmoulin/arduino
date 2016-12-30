/* YourDuino.com Example: Multiple DS18B20 Temperature Sensors
   Displayed on 4x20 character LCD display
   
   DS18B20 Pinout (Left to Right, pins down, flat side toward you)
  - Left   = Ground
  - Center = Signal (Pin 10):  (with 3.3K to 4.7K resistor to +5 or 3.3 )
  - Right  = +5 or +3.3 V
   
   terry@yourduino.com 
   https://arduino-info.wikispaces.com/MultipleTemperatureSensorsToLCD
   */

/*-----( Import needed libraries )-----*/
// Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

//Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <DallasTemperature.h>

/*-----( Declare Constants and Pin Numbers )-----*/
// Data wire is plugged into port 7 on the Arduino (can be changed)
#define ONE_WIRE_BUS 7   


/*-----( Declare objects )-----*/
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass address of our oneWire instance to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/*-----( Declare Variables )-----*/
// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://arduino-info.wikispaces.com/Brick-Temperature-DS18B20#Read%20individual


 // WP 1

 // 0x28, 0xFF, 0x52, 0x5E, 0x81, 0x16, 0x05, 0x23 -> 1
// 0x28, 0xFF, 0xCD, 0x38, 0x81, 0x16, 0x05, 0x62 -> 2

DeviceAddress Probe01 = { 0x28, 0xFF, 0x52, 0x5E, 0x81, 0x16, 0x05, 0x23 }; // "1"
DeviceAddress Probe02 = { 0x28, 0xFF, 0xCD, 0x38, 0x81, 0x16, 0x05, 0x62 }; // "2"
DeviceAddress Probe03 = { 0x28, 0xFF, 0x52, 0x5E, 0x81, 0x16, 0x05, 0x23 }; // "1" Again for test
DeviceAddress Probe04 = { 0x28, 0xFF, 0xCD, 0x38, 0x81, 0x16, 0x05, 0x62 }; // "2" Again for test


void setup()   /****** SETUP: RUNS ONCE ******/
{

  Serial.begin(9600);

//------- Initialize the Temperature measurement library--------------
  sensors.begin();
  // set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(Probe01, 12);
  sensors.setResolution(Probe02, 12);
  sensors.setResolution(Probe03, 10);
  sensors.setResolution(Probe04, 10);

}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  Serial.println("------loop------");

  sensors.requestTemperatures(); // Send the command to get temperatures

  Serial.print("1: ");
  displayTemperature(Probe01);  
  
  Serial.print("2: ");
  displayTemperature(Probe02);  
  
  Serial.print("3: ");
  displayTemperature(Probe03);  
  
  Serial.print("4: ");
  displayTemperature(Probe04);  
  
  delay(4000);

}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/
void displayTemperature(DeviceAddress deviceAddress)
{

float tempC = sensors.getTempC(deviceAddress);

   if (tempC == -127.00) // Measurement failed or no device found
   {
    Serial.println("Temperature Error");
   } 
   else
   {
   Serial.print("C=");
   Serial.println(tempC);
   }
}// End printTemperature

//*********( THE END )***********
