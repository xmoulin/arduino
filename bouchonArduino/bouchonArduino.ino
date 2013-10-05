/*
  Un bouchon qui retourne ce qui est ecrit sur le port.
  http://ugvdrone.wordpress.com/2013/05/08/code-echo-ino/
 */
 
void setup(){
  Serial.begin(9600);
}
void loop(){
  if (Serial.available()){
    Serial.write(Serial.read());
    Serial.write("\n");
  }
}
