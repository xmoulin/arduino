// pHRead.ino

// Constants:-
const byte pHpin = A0;    // Connect the sensor's Po output to analogue pin 0.

// Variables:-
float Po;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Po = 1023 - analogRead(pHpin);  // Read and reverse the analogue input value from the pH sensor.
    Po = map(Po, 0, 1023, 0, 14);   // Map the '0 to 1023' result to '0 to 14'.
    Serial.println(Po);             // Print the result in the serial monitor.
    delay(1000);                    // Take 1 reading per second.
}
