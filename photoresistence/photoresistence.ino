/*
http://itechnofrance.wordpress.com/2013/05/09/utilisation-dune-photo-rsistance-avec-larduino/

http://mchobby.be/wiki/index.php?title=Photo-r%C3%A9sistance

  Utilisation d’une photo-résistance 
*/

int port = A0;    
int valeur = 0;  
float vin = 0;

void setup() 
{ 
  Serial.begin(9600); 
}

void loop() 
{ 
  // Lit l’entrée analogique A0 
  valeur = analogRead(port);    
  Serial.print(valeur);  
  Serial.print(" analog,");
  // convertit l’entrée en volt 
  vin = 5 - (valeur * 5.0) / 1024.0; 
  Serial.print(" volt ");
  Serial.println(vin);  
  delay(500) ; 
}
