/*
  Scan all pin
*/

//int outPins[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
//int outPins[] = {1, 2, 3, 4, 5, 6};
//int outPins[] = {3};

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);    //Pour XIno RF 115200 coté arduino, 9600 coté raspberry pour lecture
  Serial.println();
  Serial.println("Setup");
  // initialize the digital pin as an output.
  // Loop to set pins HIGH
  for (int i = 3; i < 6; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }

  //pinMode(outPins[0], INPUT);
  delay(1000);

}

// the loop routine runs over and over again forever:
void loop() {
  Serial.println("In the loop");
  //printPin(outPins[0]);
  printPins();
  delay(5000);
}

void printPins() {
  for (int j = 3; j < 6; j++)
  {
    printPin(j);  // read input value;
  }
}

void printPin(int a) {
    int val = digitalRead(a);  // read input value;
    Serial.print("PIN ");
    Serial.print(a);
    Serial.print(" value=");
    Serial.println(val);
}

