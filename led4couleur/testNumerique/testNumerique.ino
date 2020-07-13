// RGB LED - Automatic Color Cycling
// http://www.mbeckler.org/microcontrollers/rgb_led/
// Matthew L Beckler 
// matthew at mbeckler dot org

//Test avec des entree numerique sur un WemosD1
int redPin = D6;
int bluePin = D7;
int greenPin = D5;

int redIn = 0;
int greenIn = 1;
int blueIn = 2;

int redVal;
int greenVal;
int blueVal;

void setup()
{
  Serial.begin(115200);
  redVal = LOW;
  greenVal = LOW;
  blueVal = LOW;
  pinMode(redPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  update();
  Serial.println("Init OK");
}

// This function updates the LED outputs.
void update()
{
  digitalWrite(redPin, redVal);
  digitalWrite(greenPin, greenVal);
  digitalWrite(bluePin, blueVal);
}


void loop()
{
  Serial.println("Blanc");
  redVal = LOW;
  greenVal = LOW;
  blueVal = LOW;
  update();
  delay(5000);
  Serial.println("Noir");
  redVal = HIGH;
  greenVal = HIGH;
  blueVal = HIGH;
  update();
  delay(5000);
  Serial.println("Rouge");
  // start out at black (all off)
  redVal = LOW;
  greenVal = HIGH;
  blueVal = HIGH;
  update();
  delay(5000);
  Serial.println("Vert");
  redVal = HIGH;
  greenVal = LOW;
  blueVal = HIGH;
  update();
  delay(5000);
  Serial.println("Bleu");
  redVal = HIGH;
  greenVal = HIGH;
  blueVal = LOW;
  update();
  delay(5000);
  Serial.println("Violet");
  redVal = LOW;
  greenVal = HIGH;
  blueVal = LOW;
  update();
  delay(5000);
  Serial.println("Jaune");
  redVal = LOW;
  greenVal = LOW;
  blueVal = HIGH;
  update();
  delay(5000);
  Serial.println("BleuCiel");
  redVal = HIGH;
  greenVal = LOW;
  blueVal = LOW;
  update();
  delay(5000);
}
