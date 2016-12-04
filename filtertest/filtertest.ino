#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <SPI.h>
#include <math.h>
#include <Filters.h>

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified( 54321 );
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified( 20 );
float ax_sensVal, ay_sensVal, az_sensVal, gx_sensVal, gy_sensVal, gz_sensVal;
sensors_vec_t orientation;

float filterFrequency = 1.0;

FilterOnePole filterX( LOWPASS, filterFrequency);
FilterOnePole filterY( LOWPASS, filterFrequency);
FilterOnePole filterZ( LOWPASS, filterFrequency);

void setup(void)
{
  Serial.begin(115200);
  Serial.println("start");

   /* Enable auto-ranging */
  gyro.enableAutoRange( true );
  Serial.println("more");
  /* Initialize the gyro and accel */
  if( !gyro.begin() ) {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println( "Ooops, no L3GD20 detected ... Check your wiring!" );
    while ( 1 );
  }
  if( !accel.begin() ) {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println( "Ooops, no LSM303 detected ... Check your wiring!" );
    while ( 1 );
  }
}

void loop(void)
{
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

  filterX.input(ax_sensVal);
  filterY.input(ay_sensVal);
  filterZ.input(az_sensVal);

   /* Print in serial monitor */
  Serial.print(ax_sensVal); Serial.print("\t");
  Serial.print(ay_sensVal); Serial.print("\t");
  Serial.print(az_sensVal); Serial.print("\t");
  //Serial.print(gx_sensVal); Serial.print("\t");
  //Serial.print(gy_sensVal); Serial.print("\t");
  //Serial.print(gz_sensVal); Serial.print("\t");
  Serial.println();
}

