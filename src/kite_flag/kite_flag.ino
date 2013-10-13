// Author: Rasmus Stougaard

//#define DEBUG_MAIN 1
#define DEBUG_HORN 1
#define DEBUG_DISPLAY 1

#include "common.h"
#include <LiquidCrystal.h>
#include "UserInterface.h"

// select the pins used on the LCD panel
UserInterface ui(8, 9, 4, 5, 6, 7);

// boot time
long epoch; 

void setup()
{
  epoch = millis();
  
  // setup inputs for flag switches
  pinMode (NO_HEAT_FLAG, INPUT_PULLUP);
  pinMode (HEAT_IN_PROGESS_FLAG, INPUT_PULLUP);
  pinMode (HEAT_ENDING_SOON_FLAG, INPUT_PULLUP);

  // setup outputs for Motor and Horn
  pinMode (FLAG_MOTOR, OUTPUT);
  pinMode (HORN, OUTPUT);
  
  // setup LEDs for signalling current flag
  pinMode(LED_FLAG_RED, OUTPUT);
  pinMode(LED_FLAG_GREEN, OUTPUT);
  pinMode(LED_FLAG_YELLOW, OUTPUT);
    
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
  horn_demo();
  long cycle_time_ms = getCycleTime();
  
  ui.pollButtons();

  ui.updateDisplay(cycle_time_ms);
  
  // no heat in progress
  if (cycle_time_ms < (TRANSITION_TIME_MINUTES * MS_PER_MINUTE)) {
#ifdef DEBUG_MAIN
    Serial.println("show NO_HEAT_FLAG");
#endif
    showFlag(NO_HEAT_FLAG);
  } else {
    // HEAT_ENDING for heat ending
    if (cycle_time_ms < getHeatEndingSoonTime()) {
#ifdef DEBUG_MAIN
      Serial.println("show HEAT_IN_PROGESS_FLAG");
#endif
      showFlag(HEAT_IN_PROGESS_FLAG);
    } else {
#ifdef DEBUG_MAIN
      Serial.println("show HEAT_ENDING_SOON_FLAG");
#endif
      showFlag(HEAT_ENDING_SOON_FLAG);
    }
    
  }
}
