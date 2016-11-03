#include <stdio.h>

const int buttonPin = 2;     // the number of the pushbutton pin
const int led_1 = 13;      // the number of the LED pin
const int led_2 = 12;
const int led_3 = 11;
const int led_4 = 10;
const int led_5 = 9;

int x = 0;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);
  pinMode(led_5, OUTPUT);
  
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() 
{
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH)
    {
      x++;
      if (x == 32)
        x = 0;
      delay(200);
    }

  int n = x;

  if (n>=16)
  {
    digitalWrite(led_5, HIGH);
    n -= 16;
  }
  else
    digitalWrite(led_5, LOW);  
  if (n>=8) 
  {
    digitalWrite(led_4, HIGH);
    n -= 8;
  }
  else 
    digitalWrite(led_4, LOW);
  if (n>=4) 
  {
    digitalWrite(led_3, HIGH);
    n -= 4;
  }
  else 
    digitalWrite(led_3, LOW);
  
  if (n>=2) 
  {
    digitalWrite(led_2, HIGH);
    n -= 2;
  }
  else 
    digitalWrite(led_2, LOW);
  if (n>=1) 
  {
    digitalWrite(led_1, HIGH);
    n -= 1;
  }
  else 
    digitalWrite(led_1, LOW);

}
