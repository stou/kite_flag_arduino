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
//#define DEBUG_STATE 1

//variabel for changing heat_time in min
const int TRANSITION_TIME_MINUTES = 2; 
const int HEAT_TIME_MINUTES = 10;
const int HEAT_ENDING_SOON_TIME_MINUTES = 1;

// END OF CONFIGURATION ==========================

// number of milliseconds per minute
#ifdef DEBUG_STATE
// NB! speed up during debugging (2 ms second)
const int MS_PER_MINUTE = 2;
#else
const int MS_PER_MINUTE = 1000 * 60;
#endif
//the duration of timed event nr 1, set to 1s while debugging
const long TRANSITION_TIME_MS = MS_PER_MINUTE * TRANSITION_TIME_MINUTES;
const long HEAT_TIME_MS = MS_PER_MINUTE * HEAT_TIME_MINUTES;
const long HEAT_ENDING_SOON_TIME_MS = MS_PER_MINUTE * HEAT_ENDING_SOON_TIME_MINUTES;

const long CYCLE_DURATION = TRANSITION_TIME_MS + HEAT_TIME_MINUTES;

//Output for motor setup,Horn, stop switchs
const int FLAG_MOTOR = 13;
const int DEBUG_FLAG_RED = 8;
const int DEBUG_FLAG_GREEN = 7;
const int DEBUG_FLAG_YELLOW = 6;

const int NO_HEAT_FLAG = 2; //red flag
const int HEAT_IN_PROGESS_FLAG = 3; // green flag
const int HEAT_ENDING_SOON_FLAG = 12; // yellow flag

// boot time
long epoch; 

// runs motor until desired desiredFlag is showing
void showFlag(int desiredFlag) {
	int state = digitalRead(desiredFlag);

#ifdef DEBUG_STATE
    // Serial.print("state: ");
    // Serial.println(state);
    // Serial.print("desiredFlag: ");
    // Serial.println(desiredFlag);
	// switch off all leds
	digitalWrite(DEBUG_FLAG_RED, 0);
	digitalWrite(DEBUG_FLAG_GREEN, 0);
	digitalWrite(DEBUG_FLAG_YELLOW, 0);

	switch(desiredFlag) {
		case NO_HEAT_FLAG:
			digitalWrite(DEBUG_FLAG_RED, 1);
		break;
		case HEAT_IN_PROGESS_FLAG:
			digitalWrite(DEBUG_FLAG_GREEN, 1);
		break;
		case HEAT_ENDING_SOON_FLAG:
			digitalWrite(DEBUG_FLAG_YELLOW, 1);
		break;
		default:
		break;
	}
#endif

	if(!state) {
#ifdef DEBUG_STATE
		Serial.println("FLAG_MOTOR running");
#endif
		digitalWrite(FLAG_MOTOR, HIGH);
	} else {
#ifdef DEBUG_STATE
		Serial.println("FLAG_MOTOR off");
#endif
		digitalWrite(FLAG_MOTOR, LOW);
	}
}


void setup()
{
	epoch = millis(); //start time

	pinMode (NO_HEAT_FLAG, INPUT);  //set pin 2 as input
	pinMode (HEAT_IN_PROGESS_FLAG, INPUT);  //set pin 3 as input
	pinMode (HEAT_ENDING_SOON_FLAG, INPUT);  //set pin 12 as input
	pinMode (FLAG_MOTOR, OUTPUT); //set pin 13 as output

#ifdef DEBUG_STATE
	// setup some debug pins - could be useful if LEDs where connected here.
	pinMode(DEBUG_FLAG_RED, OUTPUT);
	pinMode(DEBUG_FLAG_GREEN, OUTPUT);
	pinMode(DEBUG_FLAG_YELLOW, OUTPUT);

	Serial.begin(9600);
#endif
}


long getCycleTime() {
#ifdef DEBUG_STATE
	static int step = 0;
	long cycle_time_ms = 0;
	cycle_time_ms = step;
	cycle_time_ms = cycle_time_ms % (TRANSITION_TIME_MS + HEAT_TIME_MS);

	++step;
#else
	long elapsed_time = millis() - epoch;

	long cycle_time_ms = elapsed_time % CYCLE_DURATION;
#endif
	return cycle_time_ms;
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

#ifdef DEBUG_STATE
	Serial.print("cycle time: ");
	long cycle_time_minutes = cycle_time_ms / MS_PER_MINUTE;
	Serial.println(cycle_time_minutes);
#endif

	// no heat in progress
	if (cycle_time_ms < TRANSITION_TIME_MS) {
#ifdef DEBUG_STATE
		Serial.println("show NO_HEAT_FLAG");
#endif
		showFlag(NO_HEAT_FLAG);
	} else {
		// HEAT_ENDING for heat ending
		if (cycle_time_ms < (TRANSITION_TIME_MS + HEAT_TIME_MS - HEAT_ENDING_SOON_TIME_MS)) {
#ifdef DEBUG_STATE
			Serial.println("show HEAT_IN_PROGESS_FLAG");
#endif
			showFlag(HEAT_IN_PROGESS_FLAG);
		} else {
#ifdef DEBUG_STATE
			Serial.println("show HEAT_ENDING_SOON_FLAG");
#endif
			showFlag(HEAT_ENDING_SOON_FLAG);
		}
		
	}


}