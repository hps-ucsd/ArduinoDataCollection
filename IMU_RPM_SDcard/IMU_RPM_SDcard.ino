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
#include <SD.h>

/*
 * Variable declarations.
 */
 
/* IMU */ 
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified( 54321 );
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified( 20 );
float ax_sensVal, ay_sensVal, az_sensVal, gx_sensVal, gy_sensVal, gz_sensVal;
sensors_vec_t orientation;

/* LCD */
//LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

/* Hall Effect */
const int hall_effect = 2;
int hall_effect_state = 0, last_state = 0;
int timecurrent = 0, timeold = 0;
float rpm = 0;

float dt=125;
const int chipSelect = 4;
int firstiteration=1;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void setup(void)
{
  /* Sets the baud rate for serial monitor */
  Serial.begin(115200);

  /* Assign pin to read data from hall effect */  
  pinMode(hall_effect, INPUT);
  
  /* Initialize lcd */
 // lcd.begin(16, 2);

   /* Enable auto-ranging */
  gyro.enableAutoRange( true );
  
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

  /* Setup SD card */
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void loop(void)
{
  unsigned long timecurrent = millis();
  Serial.print(timecurrent); Serial.print(" ");
  
  /* Finding RPM */
  hall_effect_state = digitalRead(hall_effect);
  if ((hall_effect_state != last_state) && hall_effect_state == LOW)
  {
    //Serial.println("detect");  
    rpm = 20000 / (timecurrent - timeold);
    
    Serial.print(rpm);
    //Serial.print("  ---  "); Serial.print(timecurrent);
    //Serial.print("  ---  "); Serial.print(timeold); 

    timeold = timecurrent;
    Serial.println();
  } 

  last_state = hall_effect_state;
/**************************************************************/
  /* Reading values from IMU */
  
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
/**************************************************************/
  /* Printing */
  
  /* Print in serial monitor */
  Serial.print("ax: ");     Serial.print(ax_sensVal);
  Serial.print(" ay: ");    Serial.print(ay_sensVal);
  Serial.print(" az: ");    Serial.print(az_sensVal);
  Serial.print(" -- ");
  Serial.print("gx: ");     Serial.print(gx_sensVal);
  Serial.print(" gy: ");    Serial.print(gy_sensVal);
  Serial.print(" gz: ");    Serial.print(gz_sensVal);
  //Serial.println();
  Serial.print("     ");    Serial.println(rpm);

  /* Print on lcd */
  /*
  lcd.setCursor( 0,0 );
  lcd.print("X:"); lcd.print( ax_sensVal ); lcd.print(" ");
  lcd.print("Y:"); lcd.print( ay_sensVal ); lcd.print(" ");
  lcd.setCursor( 0,1 );
  lcd.print("Z:"); lcd.print( az_sensVal ); lcd.print(" ");
  lcd.println( "    accel" );
  */

  /* Print on SD card */
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile)
  {
    if(firstiteration)
    {
      dataFile.println("ACCEL X    Y    Z    GYRO X    Y    Z    RPM  TIME");
    }
    dataFile.print("      ");
    dataFile.print(ax_sensVal, 4.2);  dataFile.print(" ");
    dataFile.print(ay_sensVal, 4.2);  dataFile.print(" ");
    dataFile.print(az_sensVal, 4.2);  dataFile.print("         ");
    dataFile.print(gx_sensVal, 4.2);  dataFile.print(" ");
    dataFile.print(gy_sensVal, 4.2);  dataFile.print(" ");
    dataFile.print(gz_sensVal, 4.2);  dataFile.print(" ");
    dataFile.print(rpm);              dataFile.print(" ");
    dataFile.println(timecurrent);

    dataFile.close();
  }
 if(firstiteration == 1)
  firstiteration = 0;
}

