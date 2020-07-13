//Ordre de PIN : Rouge/Bleu
int myPinsLed1[] = {D5, D7};
int myPinsLed2[] = {D2, D4};
int myPinsLed1Value[] = {HIGH,HIGH};

void setup()
{
  Serial.begin(115200);

  //Init des PIN out pour tous les PIN utilisés
  for (int i = 0; i< 2;i ++ ){
    pinMode(myPinsLed1[i],OUTPUT);
    pinMode(myPinsLed2[i],OUTPUT);
  }
  update();
  Serial.println("Init OK");
}

// This function updates the LED outputs.
void update()
{
  for (int i = 0; i<2; i++){
    digitalWrite(myPinsLed1[i],myPinsLed1Value[i]);
    digitalWrite(myPinsLed2[i],myPinsLed1Value[i]);
  }
}

void loop()
{
  Serial.println("Rouge");
  setColorToRed();
  update();
  delay(5000);
  Serial.println("Violet");
  setColorToPink();
  update();
  delay(5000);
  Serial.println("Bleu");
  setColorToBlue();
  update();
  delay(5000);
}

void setColorToRed() {
  myPinsLed1Value[0] = LOW;
  myPinsLed1Value[1] = HIGH;
}

void setColorToBlue() {
  myPinsLed1Value[0] = HIGH;
  myPinsLed1Value[1] = LOW;
}

void setColorToPink() {
  myPinsLed1Value[0] = LOW;
  myPinsLed1Value[1] = LOW;
}
