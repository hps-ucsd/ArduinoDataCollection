const int hall_effect = 2;
int hall_effect_state = 0, last_state = 0;
int timecurrent = 0, timeold = 0;
float rpm = 0;

void setup() {
  Serial.begin(115200);
  pinMode(hall_effect, INPUT);
  Serial.println("start");
}

void loop() {
  hall_effect_state = digitalRead(hall_effect);
  if ((hall_effect_state != last_state) && hall_effect_state == LOW)
  {
    //Serial.println("detect");
    
    timecurrent = millis();
    rpm = 60000 / (timecurrent - timeold);
    
    Serial.print(rpm);
    //Serial.print("  ---  "); Serial.print(timecurrent);
    //Serial.print("  ---  "); Serial.print(timeold); 

    timeold = timecurrent;
    Serial.println();
  } 

  last_state = hall_effect_state;
}
