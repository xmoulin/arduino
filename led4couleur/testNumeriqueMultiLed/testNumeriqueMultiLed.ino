//Ordre de PIN : Rouge/Bleu
int myPinsLedCo2[] = {D5, D7};
int myPinsLedHumidity[] = {D2, D4};
int myPinsLedCo2Value[] = {HIGH,HIGH};
int myPinsLedHumidityValue[] = {HIGH,HIGH};
void setup()
{
  Serial.begin(115200);

  //Init des PIN out pour tous les PIN utilisés
  for (int i = 0; i< 2;i ++ ){
    pinMode(myPinsLedCo2[i],OUTPUT);
    pinMode(myPinsLedHumidity[i],OUTPUT);
  }
  updateLed();
  Serial.println("Init OK");
}

// This function updates the LED outputs.
void updateLed()
{
  for (int i = 0; i<2; i++){
    digitalWrite(myPinsLedCo2[i],myPinsLedCo2Value[i]);
    digitalWrite(myPinsLedHumidity[i],myPinsLedHumidityValue[i]);
  }
}

void loop()
{
  Serial.println("Rouge");
  setColorToRed(myPinsLedCo2Value);
  updateLed();
  delay(5000);
  Serial.println("Violet");
  setColorToPink(myPinsLedCo2Value);
  updateLed();
  delay(5000);
  Serial.println("Bleu");
  setColorToBlue(myPinsLedCo2Value);
  updateLed();
  delay(5000);
  Serial.println("Rouge Humidity");
  setColorToRed(myPinsLedHumidityValue);
  updateLed();
  delay(5000);
  Serial.println("Violet Humidity");
  setColorToPink(myPinsLedHumidityValue);
  updateLed();
  delay(5000);
  Serial.println("Bleu Humidity");
  setColorToBlue(myPinsLedHumidityValue);
  updateLed();
  delay(5000);
}

void setColorToRed(int ledValue[]) {
  ledValue[0] = LOW;
  ledValue[1] = HIGH;
}

void setColorToBlue(int ledValue[]) {
  ledValue[0] = HIGH;
  ledValue[1] = LOW;
}

void setColorToPink(int ledValue[]) {
  ledValue[0] = LOW;
  ledValue[1] = LOW;
}
