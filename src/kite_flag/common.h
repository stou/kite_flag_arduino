//  Author: Rasmus Stougaard

#ifndef _COMMON_H
#define _COMMON_H 1

#include <Arduino.h>

//variabel for changing heat_time in min
const long TRANSITION_TIME_MINUTES = 2L; 
const long HEAT_ENDING_SOON_TIME_MINUTES = 1L;

// END OF CONFIGURATION ==========================

// number of milliseconds per minute
#ifdef DEBUG_MAIN
const long MS_PER_MINUTE = 4000L;
#else
const long MS_PER_MINUTE = 60000L;
#endif

//Output for motor setup,Horn, stop switchs
const int FLAG_MOTOR = 11;
const int HORN = 13;

#endif 
// _COMMON_H

