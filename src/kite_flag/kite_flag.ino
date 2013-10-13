// Author: Rasmus Stougaard

//#define DEBUG_MAIN 1

#include <LiquidCrystal.h>
#include "UserInterface.h"
#include "Horn.h"
#include "Motor.h"

#include "common.h"

// select the pins used on the LCD shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

UserInterface ui(&lcd);

Horn horn(HORN);
Motor motor(FLAG_MOTOR);

// boot time
long epoch; 

void setup()
{
  epoch = millis();
      
  Serial.begin(9600);
  Serial.println("setup done");
}

long getCycleTime() {
  long elapsed_time = millis() - epoch;
  return elapsed_time % ((TRANSITION_TIME_MINUTES + ui.getHeatTimeMinutes()) * MS_PER_MINUTE);
}

long getHeatEndingSoonTime() {
  return (TRANSITION_TIME_MINUTES + ui.getHeatTimeMinutes() - HEAT_ENDING_SOON_TIME_MINUTES) *  MS_PER_MINUTE;
}



int getTimeout(long timestamp, int timeout_ms) {
  return ! (timestamp % timeout_ms);
}

void fireTimedEvents(long timestamp) {

  static long lastTimestamp = 0;
  // run the countdown
  if(lastTimestamp != timestamp) {
    lastTimestamp = timestamp;

    if(getTimeout(timestamp, 1000)){
      // Serial.println("Updating display (1 second timeout)");
      ui.setTime(getCycleTime());
      ui.updateDisplay();
    }

    if(getTimeout(timestamp, 3000)){
      Serial.println("3 second timeout");
    }

    if(getTimeout(timestamp, 5000)){
      Serial.println("5 second timeout");
    }

  }
}




/*
  on startup:
  run motor until the red flag is showing
  
  loop:
  show red for TRANSITION_TIME_MINUTES minutes
  show green for HEAT_TIME minutes
  show yellow for (HEAT_TIME_MINUTES - HEAT_ENDING_SOON_TIME_MINUTES) minutes
*/
void loop()
{
//  fireTimedEvents(millis());
  horn.demo();
  return;
  long cycle_time_ms = getCycleTime();
  
  ui.pollButtons();
  ui.setTime(cycle_time_ms);
  ui.updateDisplay();

  
  // no heat in progress
  if (cycle_time_ms < (TRANSITION_TIME_MINUTES * MS_PER_MINUTE)) {
#ifdef DEBUG_MAIN
    Serial.println("show NO_HEAT_FLAG");
#endif
    motor.red();
  } else {
    // HEAT_ENDING for heat ending
    if (cycle_time_ms < getHeatEndingSoonTime()) {
#ifdef DEBUG_MAIN
      Serial.println("show HEAT_IN_PROGESS_FLAG");
#endif
      motor.green();
    } else {
#ifdef DEBUG_MAIN
      Serial.println("show HEAT_ENDING_SOON_FLAG");
#endif
      motor.yellow();
    }
    
  }
}
