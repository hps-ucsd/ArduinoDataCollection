#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);

int leftButtonInput = 2;
int rightButtonInput = 4;


void setup() {
  AFMS.begin();
   // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor->setSpeed(255);
  myMotor->run(RELEASE);
}

void loop() {
  while(digitalRead(leftButtonInput) == HIGH){
    myMotor->run(FORWARD);
  }
  while(digitalRead(rightButtonInput) == HIGH){
    myMotor->run(BACKWARD);
  }
  myMotor->run(RELEASE);
}
