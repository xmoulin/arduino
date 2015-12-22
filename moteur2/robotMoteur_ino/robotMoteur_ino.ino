// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

AF_DCMotor motor_g(4);
AF_DCMotor motor_d(3);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("motor_g test!");

  // turn on motor
  motor_g.setSpeed(200);
  motor_d.setSpeed(200);

  motor_g.run(RELEASE);
  motor_d.run(RELEASE);
}

void loop() {
  uint8_t i;

  Serial.print("tick");

  motor_g.run(FORWARD);
  motor_d.run(FORWARD);

  for (i =100; i < 255; i++) {
    motor_g.setSpeed(i);
    motor_d.setSpeed(i);
    delay(10);
  }

  for (i = 255; i != 0; i--) {
    motor_g.setSpeed(i);
    motor_d.setSpeed(i);
    delay(10);
  }

  Serial.print("tock");

  motor_g.run(BACKWARD);
  motor_d.run(BACKWARD);

  for (i = 100; i < 255; i++) {
    motor_g.setSpeed(i);
    motor_d.setSpeed(i);
    delay(10);
  }

  for (i = 255; i != 0; i--) {
    motor_g.setSpeed(i);
    motor_d.setSpeed(i);
    delay(10);
  }


  Serial.print("tech");
  motor_g.run(RELEASE);
  delay(1000);
}

