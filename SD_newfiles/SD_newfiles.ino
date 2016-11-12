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

/* For use in getFileName() */
#define FILE_NAME_SIZ 10
#define CTR_INDEX 3
#define NUM_SIZ 2
#define DECIMAL 10

/*
 * Variable declarations.
 */
 
/* IMU */ 
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified( 54321 );
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified( 20 );
float ax_sensVal, ay_sensVal, az_sensVal, gx_sensVal, gy_sensVal, gz_sensVal;
sensors_vec_t orientation;

/* LCD */
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

/* Hall Effect */
const int hall_effect = 2;
int hall_effect_state = 0, last_state = 0;
unsigned long timecurrent = 0, timeold = 0;
float rpm = 0;

float dt=125;
const int chipSelect = 4;
int firstiteration=1;

/* Stores the datafile name */
char fileName[FILE_NAME_SIZ] = { 'l', 'o', 'g',
                                 'x', 'x', '.', 't', 'x', 't', '\0' };
/* Declaration for the new helper function */
void getFileName();

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void setup(void)
{
  /* Sets the baud rate for serial monitor */
  Serial.begin(115200);
  Serial.println("start");

  /* Assign pin to read data from hall effect */  
  pinMode(hall_effect, INPUT);
  Serial.println("more");
  /* Initialize lcd */
  lcd.begin(16, 2);
  Serial.println("more");
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
  Serial.println("more");
  /* Setup SD card */
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  Serial.println(fileName);
  getFileName();
  Serial.println(fileName);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void loop(void)
{

  timecurrent = millis();
  
  /* Finding RPM */
  
  hall_effect_state = digitalRead(hall_effect);
  if ((hall_effect_state != last_state) && hall_effect_state == LOW)
  {
    //Serial.println("detect");

    rpm = 60000 / (timecurrent - timeold);
    
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
  Serial.print(ax_sensVal); Serial.print("\t");
  Serial.print(ay_sensVal); Serial.print("\t");
  Serial.print(az_sensVal); Serial.print("\t");
  Serial.print(gx_sensVal); Serial.print("\t");
  Serial.print(gy_sensVal); Serial.print("\t");
  Serial.print(gz_sensVal); Serial.print("\t");
  Serial.println();
  //Serial.print("     ");    Serial.println(rpm);

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
  File dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile)
  {
    if(firstiteration)
    {
      dataFile.println("X\tY\tZ\tX\tY\tZ\tRPM\tTIME");
    }
    dataFile.print(ax_sensVal); dataFile.print("\t");
    dataFile.print(ay_sensVal); dataFile.print("\t");
    dataFile.print(az_sensVal); dataFile.print("\t");
    dataFile.print(gx_sensVal); dataFile.print("\t");
    dataFile.print(gy_sensVal); dataFile.print("\t");
    dataFile.print(gz_sensVal); dataFile.print("\t");
    dataFile.print(rpm); dataFile.print("\t");
    dataFile.println(timecurrent);

    dataFile.close();
  }
  
 if(firstiteration == 1)
  firstiteration = 0;
}

/*
 * Helper function used to build the fileName to use.
 */
void getFileName() {

  int index = CTR_INDEX;              /* The index to insert the count */
  char count[NUM_SIZ] = { '0', '0' }; /* Initalize to "00" */
  int num = 0;                        /* Use to temporarily hold the number */
  File dataFile;

  /* Check if filecounter.txt exists */
  if ( SD.exists( "counter.txt" ) ) {

    /* Open the file */
    dataFile = SD.open( "counter.txt", FILE_READ );

    /* Read the two numbers from the data file */
    count[0] = dataFile.read();
    count[1] = dataFile.read();

    /* Close the file */
    dataFile.close();
  }

  /* Add the number to the fileName */
  fileName[index++] = count[0];
  fileName[index] = count[1];

  /* Conver the number read to an int */
  num += (count[0] - '0') * DECIMAL;
  num += count[1] - '0';

  /* Increment the count */
  num++;

  /* Put the number back into count */
  count[0] = (char) (num / DECIMAL + '0');
  count[1] = (char) (num % DECIMAL + '0');

  /* Remove the old filecounter.txt */
  if ( SD.exists( "counter.txt" ) ) {
    SD.remove( "counter.txt" );
  }

  /* Write the count to filecounter.txt and close the file */
  dataFile = SD.open( "counter.txt", FILE_WRITE );
  dataFile.println( count );
  dataFile.close();
}
