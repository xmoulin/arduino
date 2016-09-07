/*
  Test simplement le serial. Et seulement ça.
  Verifier la frequence de com
 */
 
// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(115200);    //Pour XIno RF 115200 coté arduino, 9600 coté raspberry pour lecture
  Serial.println();
  Serial.println("Setup");
  delay(1000);

}

// the loop routine runs over and over again forever:
void loop() {
  Serial.println("In the loop");
  delay(1000);
}
