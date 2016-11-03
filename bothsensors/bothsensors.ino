/*******************************************************************************
                              __   __   ______   ______
                             / /  / /  / __  /  / ____/
                            / /__/ /  / /_/ /  / /___
                           / ___  /  / ____/  /___  /
                          / /  / /  / /      ____/ /
                         /_/  /_/  /_/      /_____/
                         
*******************************************************************************/

/*
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
float dt = 500;
LiquidCrystal lcd( 12, 11, 7, 6, 5, 4 );
volatile byte fifth_revolutions;
unsigned int rpm;
unsigned long timeold;
float ax_sensVal, ay_sensVal, az_sensVal, gx_sensVal, gy_sensVal, gz_sensVal;
float filterVal = 0.5;
float ax_smoothedVal = 0, ay_smoothedVal = 0, az_smoothedVal = 0, gx_smoothedVal = 0, gy_smoothedVal = 0, gz_smoothedVal = 0;
float roll, pitch;
sensors_vec_t orientation;

/* For calculating velocity; Note: timeold (above) is also used */
unsigned long timecur;
float elapsedsec;
float vx = 0, vy = 0, vz = 0;

/*
 * This function sets up the arduino.
 */

void setup( void ) {

  /* Sets the baud rate */
  Serial.begin( 9600 );

  ///* Initialize the intterrupt pin (Arduino digital pin 2) */
  //attachInterrupt( 0, magnet_detect, RISING );
  //fifth_revolutions = 0;
  //rpm = 0;
  //timeold = 0;
  //lcd.begin( 16, 2 );

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

  /* Initialize variable(s) used to calculate velocity */
  timeold = millis();
}

/*
 * This function is the main progrm execution loop.
 */

void loop( void )  {

  //if (fifth_revolutions >= 5 ) {
  //   rpm = 60.0 * 1000.0 /( millis() - timeold );
  //   timeold = millis();
  //   fifth_revolutions = 0;
  //   
  //   Serial.println( rpm,DEC );
  //   lcd.setCursor( 0,0 );
  //   lcd.print( "                " );
  //   lcd.setCursor( 0,0 );
  //   lcd.print( rpm );
  //}
   
  /* Get a new sensor event */
  sensors_event_t aEvent;
  sensors_event_t gEvent; 
  accel.getEvent( &aEvent );
  gyro.getEvent( &gEvent );

  /* calculate and filter accelerometer data */
  ax_sensVal = aEvent.acceleration.x; ax_smoothedVal = smooth( ax_sensVal, filterVal, ax_smoothedVal );
  ay_sensVal = aEvent.acceleration.y; ay_smoothedVal = smooth( ay_sensVal, filterVal, ay_smoothedVal );
  az_sensVal = aEvent.acceleration.z; az_smoothedVal = smooth( az_sensVal, filterVal, az_smoothedVal );

  /* Calculate and filter gyroscope data */
  gx_sensVal = gEvent.gyro.x;         gx_smoothedVal = smooth( gx_sensVal, filterVal, gx_smoothedVal );
  gy_sensVal = gEvent.gyro.y;         gy_smoothedVal = smooth( gy_sensVal, filterVal, gy_smoothedVal );
  gz_sensVal = gEvent.gyro.z;         gz_smoothedVal = smooth( gz_sensVal, filterVal, gz_smoothedVal );

  /* Get roll and pitch */
  roll = orientation.roll;
  pitch = orientation.pitch;

  /* Adjust acceleration values to account for acceleration*/
  ax_smoothedVal = ax_smoothedVal - (9.8*sin(90-pitch));
  ay_smoothedVal = ay_smoothedVal - (9.8*sin(90-roll));
  
  /* Determine the elapsed time in seconds */
  timecur = millis();
  elapsedsec = ( timecur - timeold ) / 1000.0;
  timeold = timecur;

  /* Calculate velocity [m/s] */
  vx = vx + ax_smoothedVal * elapsedsec;
  vy = vy + ay_smoothedVal * elapsedsec;

  /* Subtract (or add) 9.8 m/s^2 to account for the acceleration in the z-axis */
  vz = vz + ( az_smoothedVal - 9.8 ) * elapsedsec;

  /* Display the results */
  //Serial.print("ax: ");     
  Serial.print(ax_smoothedVal);
  //Serial.print(" ay: ");    
  Serial.print(ay_smoothedVal);
 //Serial.print(" az: ");    
  Serial.print(az_smoothedVal);
  //Serial.print(" -- ");
  //Serial.print("gx: ");     
  Serial.print(gx_smoothedVal);
  //Serial.print(" gy: ");    
  Serial.print(gy_smoothedVal);
  //Serial.print(" gz: ");    
  Serial.print(gz_smoothedVal);
  //Serial.print(" -- ");
  //Serial.print("vx: ");     
  Serial.print(vx);
  //Serial.print(" vy: ");    
  Serial.print(vy);
  //Serial.print(" vz: ");    
  Serial.print(vz);
  //Serial.print(" -- ");
  //Serial.print("roll: ");   
  Serial.print(roll);
  //Serial.print(" pitch: "); 
  Serial.print(pitch);
  Serial.println();
  
 // Serial.print("ax: %f  ay: %f  az: %f  --  gx: %f  gy: %f  gz: %f  --  vx: %f  vy: %f  vz: %f\n",
   // ax_smoothedVal, ay_smoothedVal, az_smoothedVal,
   // gx_smoothedVal, gy_smoothedVal, gz_smoothedVal,
   // vx, vy, vz);

  //lcd.setCursor( 0,1 );
  //Serial.print( ax_smoothedVal ); Serial.print( " " );
  //Serial.print( ay_smoothedVal ); Serial.print( " " );
  //Serial.print( az_smoothedVal ); Serial.print( " " );
  //lcd.setCursor( 0,1 );
  //Serial.print( gx_smoothedVal ); Serial.print( "  " );
  //Serial.print( gy_smoothedVal ); Serial.print( "  " );
  //Serial.print( gz_smoothedVal ); Serial.print( "  " );
  //Serial.println( " " );
  
  delay( dt );
}

/*
 * This function is a low pass filter for smoothing data.
 */

float smooth( float sensVal, float filterVal, float smoothedVal ) {

  smoothedVal = ( sensVal * ( 1 - filterVal ) ) + ( smoothedVal * filterVal );
  return (float) smoothedVal;
}

///*
// *This function is called whenever a magnet/interrupt is detected by the arduino
// */
//
//void magnet_detect()
//{
//  fifth_revolutions++;
//  Serial.println( "detect" );
//}
