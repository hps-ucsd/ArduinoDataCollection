#include "HX711.h"

#define DOUT  3
#define CLK  2

HX711 scale(DOUT, CLK);

float calibration_factor = -7050;

void setup() 
{
  Serial.begin(9600);

  scale.set_scale(calibration_factor);
  scale.tare();
}

void loop() 
{
  Serial.print(scale.get_units(), 1);
  Serial.print(" lbs");
  Serial.println();
}
