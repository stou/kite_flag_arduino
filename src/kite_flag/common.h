
//  Author: Rasmus Stougaard

#ifndef _COMMON_H
#define _COMMON_H 1

#include <Arduino.h>

#define DEBUG_HORN 1


//variabel for changing heat_time in min
const long TRANSITION_TIME_MINUTES = 2; 
const long HEAT_ENDING_SOON_TIME_MINUTES = 1;

// END OF CONFIGURATION ==========================

// number of milliseconds per minute
#ifdef DEBUG_SIMUINO
// NB! speed up during debugging (2 ms second)
const long MS_PER_MINUTE = 2;
#else
const long MS_PER_MINUTE = 60000;
#endif

//Output for motor setup,Horn, stop switchs
#ifdef DEBUG_HORN
const int FLAG_MOTOR = 11;
const int HORN = 13;
#else
const int FLAG_MOTOR = 13;
const int HORN = 11;
#endif

#ifdef DEBUG_SIMUINO
// the Analog pins does not seem to work as outputs in simuino
const int LED_FLAG_RED = 8;
const int LED_FLAG_GREEN = 7;
const int LED_FLAG_YELLOW = 6;
#else
const int LED_FLAG_RED = A1;
const int LED_FLAG_GREEN = A2;
const int LED_FLAG_YELLOW = A3;
#endif

const int NO_HEAT_FLAG = 2; //red flag
const int HEAT_IN_PROGESS_FLAG = 3; // green flag
const int HEAT_ENDING_SOON_FLAG = 12; // yellow flag


#endif 
// _COMMON_H
