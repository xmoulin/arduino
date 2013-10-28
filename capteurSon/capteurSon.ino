/* YourDuino Electronic Brick Test
   Sound Sensor
   terry@yourduino.com 
 http://arduino-info.wikispaces.com/Brick-SoundSensor  
Je reçois 1024 qd pas de son, 0 si du son.
 */

/*-----( Declare Constants / Pin Numbers )-----*/
#define  SOUND_PIN  A2
#define  LED_PIN    13

/*-----( Declare Variables )-----*/

int  soundValue  =0;

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
//  pinMode(LED_PIN, OUTPUT);  // declare the ledPin as an OUTPUT
  Serial.begin(9600);        // initialize serial communication with computer
}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  soundValue = analogRead(SOUND_PIN);    // read the value from the sensor
   
  if (soundValue < 100)
  {
   Serial.println(soundValue, DEC);          // send it to the computer (as ASCII digits) 
  }
  /*
  if (SoundValue > 50)
  {  
    digitalWrite(LED_PIN, HIGH);  // turn the ledPin on
    delay(SoundValue*5);                  // stop the program for some time
  }
  
  digitalWrite(LED_PIN, LOW);   // turn the ledPin off
  */
  //delay(200);                  // stop the program for some time

}/* --(end main loop )-- */

/* ( THE END ) */
