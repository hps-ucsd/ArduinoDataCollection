/*******************************************************************************
                              __   __   ______   ______
                             / /  / /  / __  /  / ____/
                            / /__/ /  / /_/ /  / /___
                           / ___  /  / ____/  /___  /
                          / /  / /  / /      ____/ /
                         /_/  /_/  /_/      /_____/
                         
*******************************************************************************/

/*w
 * Included libraries.
 */
 
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <SPI.h>
#include <math.h>

/*
 * Variable declarations.
 */
 
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified( 54321 );
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified( 20 );
float ax_sensVal, ay_sensVal, az_sensVal, gx_sensVal, gy_sensVal, gz_sensVal;
sensors_vec_t orientation;
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
float dt=125;

const int hall_effect = 2;
int hall_effect_state = 0;
float timeold = 0, rpm = 0;

void setup(void)
{
  /* Sets the baud rate */
  Serial.begin(115200);

  pinMode(hall_effect, INPUT);
  
  /* Initialize lcd */
  lcd.begin(16, 2);

   /* Enable auto-ranging */
  gyro.enableAutoRange( true );
  
  /* Initialize the sensor */
  if( !gyro.begin() ) {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println( "Ooops, no L3GD20 detected ... Check your wiring!" );
    while ( 1 );
  }
  /* Initialize the sensor */
  if( !accel.begin() ) {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println( "Ooops, no LSM303 detected ... Check your wiring!" );
    while ( 1 );
  }
}

void loop(void)
{
  /*
  hall_effect_state = digitalRead(hall_effect);
  if(hall_effect_state == LOW)
  {
    rpm = (millis()-timeold)*60;
    timeold = millis();
  }
  */
  
  /* Get a new sensor event */
  sensors_event_t aEvent;
  sensors_event_t gEvent; 
  accel.getEvent( &aEvent );
  gyro.getEvent( &gEvent );

  /* Get accelerometer data */
  ax_sensVal = aEvent.acceleration.x;
  ay_sensVal = aEvent.acceleration.y;
  az_sensVal = aEvent.acceleration.z;

  /* Get gyroscope data */
  gx_sensVal = gEvent.gyro.x;
  gy_sensVal = gEvent.gyro.y;
  gz_sensVal = gEvent.gyro.z;

  /* Print in serial monitor */
  Serial.print("ax: ");     Serial.print(ax_sensVal);
  Serial.print(" ay: ");    Serial.print(ay_sensVal);
  Serial.print(" az: ");    Serial.print(az_sensVal);
  Serial.print(" -- ");
  Serial.print("gx: ");     Serial.print(gx_sensVal);
  Serial.print(" gy: ");    Serial.print(gy_sensVal);
  Serial.print(" gz: ");    Serial.print(gz_sensVal);
  Serial.println();
  //Serial.print("     ");    Serial.println(rpm);

  /* Print on lcd */
  lcd.setCursor( 0,0 );
  lcd.print("X:"); lcd.print( ax_sensVal ); lcd.print( " " );
  lcd.print("Y:"); lcd.print( ay_sensVal ); lcd.print( " " );
  lcd.setCursor( 0,1 );
  lcd.print("Z:"); lcd.print( az_sensVal ); lcd.print( " " );
  lcd.println( "    accel" );

  delay(dt); 
}

