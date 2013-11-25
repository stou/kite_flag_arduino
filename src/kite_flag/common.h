
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
#ifdef DEBUG_MAIN
const long MS_PER_MINUTE = 4000;
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

#endif 
// _COMMON_H
