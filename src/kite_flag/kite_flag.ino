// Author: Rasmus Stougaard

// #define DEBUG_MAIN 1

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

    // Need the LiquidCrystal display shield, floating input makes the loop time go crazy
    ui.pollButtons(); 

    if(getTimeout(timestamp, 200)){
      ui.setTime(getCycleTime());
      ui.updateDisplay();
    }

    if(getTimeout(timestamp, 100)){
      horn.update();
    }

#if 0 // DEBUG_BUTTONS
    if(getTimeout(timestamp, 200)){
        Serial.print("button: ");
        Serial.print(analogRead(0));
        Serial.print(" heat_duration: ");
        Serial.println(ui.getHeatTimeMinutes()*100);
    }
#endif
    
#if 0 // DEBUG_OUPUTS
    if(getTimeout(timestamp, 100)){
      
        Serial.print(" red: ");
        Serial.print(digitalRead(A1)+2);
        Serial.print(" green: ");
        Serial.print(digitalRead(A2)+4);
        Serial.print(" yellow: ");
        Serial.print(digitalRead(A3)+6);      
        Serial.print(" SW: ");
        Serial.print(" red: ");
        Serial.print(digitalRead(2)+10);
        Serial.print(" green: ");
        Serial.print(digitalRead(3)+12);
        Serial.print(" yellow: ");
        Serial.print(digitalRead(12)+14);

        Serial.print(" horn: ");
        Serial.print(digitalRead(13) ? "-3" : "-2" );
      
        Serial.print(" Motor: ");
        Serial.print(digitalRead(11) ? "0" : "1" );
        Serial.print(" In-pos: ");
        Serial.println(motor.flagInPosition() ? "1" : "0");
    }
#endif
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
