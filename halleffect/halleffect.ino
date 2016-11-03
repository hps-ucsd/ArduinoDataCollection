#include <LiquidCrystal.h>
#include <SPI.h>

/*
Arduino Hall Effect Sensor Project
by Arvind Sanjeev
Please check out  http://diyhacking.com for the tutorial of this project.
DIY Hacking
*/

LiquidCrystal lcd(12, 11, 5, 4, 3, 6);
 volatile byte fifth_revolutions;
 unsigned int rpm;
 unsigned long timeold;
 void setup()
 {
   Serial.begin(115200);
   attachInterrupt(0, magnet_detect, RISING);//Initialize the intterrupt pin (Arduino digital pin 2)
   fifth_revolutions = 0;
   rpm = 0;
   timeold = 0;
   lcd.begin(16, 2);
 }
 void loop()//Measure RPM
 {
   if (fifth_revolutions >= 5 ) { 
     rpm = 60.0 * 1000.0 /(millis() - timeold);
     timeold = millis();
     fifth_revolutions = 0;
     Serial.println(rpm,DEC);
     lcd.setCursor(0,0);
     lcd.print("                ");
     lcd.setCursor(0,0);
     lcd.print(rpm);
   }
 }
 void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
   fifth_revolutions++;
   Serial.println("detect");
 }
