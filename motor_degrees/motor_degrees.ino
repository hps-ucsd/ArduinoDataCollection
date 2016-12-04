 #include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);

int leftButtonInput = 2;
int rightButtonInput = 4;

int leftState = LOW, leftLast = LOW;
int rightState = LOW, rightLast = LOW;


void setup() {
  AFMS.begin();
   // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor->setSpeed(255);
  myMotor->run(RELEASE);
}

void loop() {
  leftState = digitalRead(leftButtonInput); 
  rightState = digitalRead(rightButtonInput);
  
  if(leftState == HIGH && leftState != leftLast){
    myMotor->run(FORWARD);
    delay(50);
    myMotor->run(RELEASE);
  }
  if(rightState == HIGH && rightState != rightLast){
    myMotor->run(BACKWARD);
    delay(50);
    myMotor->run(RELEASE);
  }
  myMotor->run(RELEASE);
  delay(100);
}
