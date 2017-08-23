// Author: Rasmus Stougaard

#define DEBUG_MAIN 1

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

  if(lastTimestamp != timestamp) {
    lastTimestamp = timestamp;

    ui.pollButtons(); // DEBUG make the buttons work, floating input makes the loop time go crazy

    if(getTimeout(timestamp, 200)){
      ui.setTime(getCycleTime());
      ui.updateDisplay();
    }

    if(getTimeout(timestamp, 100)){
      horn.update();
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

void flag_control(){
  static int lastState = -1;

  int state = 0;

  long cycle_time_ms = getCycleTime();
  
  ui.setFlag(motor.getFlagPosition());
  ui.setDesiredFlagPosition(motor.getDesiredFlagPosition());

  // no heat in progress
  if (cycle_time_ms < (TRANSITION_TIME_MINUTES * MS_PER_MINUTE)) {
    motor.red();
    state = 1;
    if(lastState != state){
      horn.red();
    }
  } else {
    // HEAT_ENDING for heat ending
    if (cycle_time_ms < getHeatEndingSoonTime()) {
      motor.green();
      state = 2;
      if(lastState != state){
        horn.green();
      }
    } else {
      motor.yellow();
      state = 3;
      if(lastState != state){
        horn.yellow();
      }
    }  
  }

  lastState = state;
}


void loop()
{
  fireTimedEvents(millis());
  flag_control();
}
