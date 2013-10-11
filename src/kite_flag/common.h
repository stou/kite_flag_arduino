
//  Author: Rasmus Stougaard

#ifndef _COMMON_H
#define _COMMON_H 1

#include <Arduino.h>

// uncomment this line to single step in simuino simulator


//variabel for changing heat_time in min
const long TRANSITION_TIME_MINUTES = 2; 
const long HEAT_ENDING_SOON_TIME_MINUTES = 1;

// END OF CONFIGURATION ==========================

// number of milliseconds per minute
const long MS_PER_MINUTE = 60000;

//Output for motor setup,Horn, stop switchs
#ifdef DEBUG_HORN
const int FLAG_MOTOR = 11;
const int HORN = 13;
#else
const int FLAG_MOTOR = 13;
const int HORN = 11;
#endif

const int LED_FLAG_RED = A1;
const int LED_FLAG_GREEN = A2;
const int LED_FLAG_YELLOW = A3;

//red flag
const int NO_HEAT_FLAG = 2; 
// green flag
const int HEAT_IN_PROGESS_FLAG = 3;
// yellow flag
const int HEAT_ENDING_SOON_FLAG = 12;

#endif 
// _COMMON_H
