// Author: Rasmus Stougaard

//================================================
//  Scenario for simuino
//================================================
//
// SCENDIGPIN 2    1     0
// SCENDIGPIN 3    1     0
// SCENDIGPIN 12   1     0

// SCENDIGPIN 2    25    1
// SCENDIGPIN 3    25    0
// SCENDIGPIN 12   25    0

// SCENDIGPIN 2    69    0
// SCENDIGPIN 3    69    1
// SCENDIGPIN 12   69    0

// SCENDIGPIN 2    267   0
// SCENDIGPIN 3    267   0
// SCENDIGPIN 12   267   1

// SCENDIGPIN 2    289   1
// SCENDIGPIN 3    289   0
// SCENDIGPIN 12   289   0

//================================================
//  Program source
//================================================

// CONFIGURATION Section =========================

// uncomment this line to single step in simuino simulator
#define DEBUG_MAIN 1
// #define DEBUG_SIMUINO 1


// allow running in simuino
#ifdef DEBUG_SIMUINO
#define INPUT_PULLUP INPUT
#endif

#ifndef DEBUG_MAIN
#define HAS_LCD_SUPPORT 1
#endif

#include "common.h"

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
  
#ifdef HAS_LCD_SUPPORT
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
#endif
  
  Serial.begin(9600);
  Serial.println("setup done");
}


long getCycleTime() {
#ifdef DEBUG_MAIN
  static int step = 0;
  long cycle_time_ms = 0;
  cycle_time_ms = step;
  cycle_time_ms = cycle_time_ms % ((TRANSITION_TIME_MINUTES + heatTimeMinutes) * MS_PER_MINUTE);
  
  ++step;
#else
  long elapsed_time = millis() - epoch;
  long cycle_time_ms = elapsed_time % ((TRANSITION_TIME_MINUTES + heatTimeMinutes) * MS_PER_MINUTE);
#endif
  return cycle_time_ms;
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
  long cycle_time_ms = getCycleTime();
  
  int lcdKey = read_buttons();
  handle_buttons(lcdKey);

  update_display(cycle_time_ms);
  
#ifdef DEBUG_MAIN
  Serial.print("cycle time: ");
  long cycle_time_minutes = cycle_time_ms / MS_PER_MINUTE;
  Serial.println(cycle_time_minutes);
#endif
  
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
