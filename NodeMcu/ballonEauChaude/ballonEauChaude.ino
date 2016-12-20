int analogPin = 3;     // potentiometer wiper (middle terminal) connected to analog pin 3
// outside leads to ground and +5V
long val = 0;           // variable to store the value read

void setup()
{
  Serial.begin(9600);          //  setup serial
  Serial.println("Init OK");
}

void loop()
{
  val = analogRead(analogPin);    // read the input pin
  float volt = mapfloat(val, 0, 1023, 0, 5);
  Serial.print("lecture=");             // debug value
  Serial.print(val);             // debug value
  Serial.print(" - volt=");             // debug value
  Serial.println(volt);             // debug value
  delay(1000);
}

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

