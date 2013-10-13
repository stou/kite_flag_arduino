// Author: Rasmus Stougaard

//#define DEBUG_MAIN 1
#define DEBUG_HORN 1
#define DEBUG_DISPLAY 1

#include <LiquidCrystal.h>
#include "common.h"

#include "display.h"

// boot time
long epoch; 
long heatTimeMinutes = 10;

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
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  Serial.begin(9600);
  Serial.println("setup done");
}

long getCycleTime() {
  long elapsed_time = millis() - epoch;
  return elapsed_time % ((TRANSITION_TIME_MINUTES + heatTimeMinutes) * MS_PER_MINUTE);
}

long getHeatEndingSoonTime() {
  return (TRANSITION_TIME_MINUTES + heatTimeMinutes - HEAT_ENDING_SOON_TIME_MINUTES) *  MS_PER_MINUTE;
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
  
  int lcdKey = read_buttons();
  handle_buttons(lcdKey);

  update_display(cycle_time_ms);
  
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
