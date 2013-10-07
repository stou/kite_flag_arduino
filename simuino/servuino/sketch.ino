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

#define DEBUG_STATE 1

//variabel for changing heat_time in min
const int TRANSITION_TIME_MINUTES = 2; 
const int HEAT_TIME_MINUTES = 10;
const int HEAT_ENDING_SOON_TIME_MINUTES = 1;

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
	int state = digitalReadX(67,desiredFlag);

#ifdef DEBUG_STATE
    // Serial.printX(70,"state: ");
    // Serial.printlnX(71,state);
    // Serial.printX(72,"desiredFlag: ");
    // Serial.printlnX(73,desiredFlag);
	// switch off all leds
	digitalWriteX(75,DEBUG_FLAG_RED, 0);
	digitalWriteX(76,DEBUG_FLAG_GREEN, 0);
	digitalWriteX(77,DEBUG_FLAG_YELLOW, 0);

	switch(desiredFlag) {
		case NO_HEAT_FLAG:
			digitalWriteX(81,DEBUG_FLAG_RED, 1);
		break;
		case HEAT_IN_PROGESS_FLAG:
			digitalWriteX(84,DEBUG_FLAG_GREEN, 1);
		break;
		case HEAT_ENDING_SOON_FLAG:
			digitalWriteX(87,DEBUG_FLAG_YELLOW, 1);
		break;
		default:
		break;
	}
#endif

	if(!state) {
#ifdef DEBUG_STATE
		Serial.printlnX(96,"FLAG_MOTOR running");
#endif
		digitalWriteX(98,FLAG_MOTOR, HIGH);
	} else {
#ifdef DEBUG_STATE
		Serial.printlnX(101,"FLAG_MOTOR off");
#endif
		digitalWriteX(103,FLAG_MOTOR, LOW);
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
	pinModeX(119,DEBUG_FLAG_RED, OUTPUT);
	pinModeX(120,DEBUG_FLAG_GREEN, OUTPUT);
	pinModeX(121,DEBUG_FLAG_YELLOW, OUTPUT);

	Serial.beginX(123,9600);
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
	Serial.printX(158,"cycle time: ");
	long cycle_time_minutes = cycle_time_ms / MS_PER_MINUTE;
	Serial.printlnX(160,cycle_time_minutes);
#endif

	// no heat in progress
	if (cycle_time_ms < TRANSITION_TIME_MS) {
#ifdef DEBUG_STATE
		Serial.printlnX(166,"show NO_HEAT_FLAG");
#endif
		showFlag(NO_HEAT_FLAG);
	} else {
		// HEAT_ENDING for heat ending
		if (cycle_time_ms < (TRANSITION_TIME_MS + HEAT_TIME_MS - HEAT_ENDING_SOON_TIME_MS)) {
#ifdef DEBUG_STATE
			Serial.printlnX(173,"show HEAT_IN_PROGESS_FLAG");
#endif
			showFlag(HEAT_IN_PROGESS_FLAG);
		} else {
#ifdef DEBUG_STATE
			Serial.printlnX(178,"show HEAT_ENDING_SOON_FLAG");
#endif
			showFlag(HEAT_ENDING_SOON_FLAG);
		}
		
	}


}