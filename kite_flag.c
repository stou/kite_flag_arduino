

//Output for motor setup,Horn, stop switchs
const int FLAG_MOTOR = 13;

const int NO_HEAT_FLAG = 2; //red flag
const int HEAT_IN_PROGESS_FLAG = 3; // green flag
const int HEAT_ENDING_SOON_FLAG = 12; // yellow flag

//Variables defined

//starts time cycle
long epoch; 

// time since start of cycle
long elapsed_time;

//variabel for changing heat_time in min
const int HEAT_TIME_MINUTES = 10;
const int HEAT_ENDING_SOON_TIME_MINUTES = 2;
const int TRANSITION_TIME_MINUTES = 4; 

// number of milliseconds per minute
// NB! use 1000 to speed up during debugging (1 second)
const int MS_PER_MINUTE = 1000 * 60;

//the duration of timed event nr 1, set to 1s while debugging
const long TRANSITION_TIME_MS = MS_PER_MINUTE * TRANSITION_TIME_MINUTES;
const long HEAT_TIME_MS = MS_PER_MINUTE * HEAT_TIME_MINUTES;
const long HEAT_ENDING_SOON_TIME_MS = MS_PER_MINUTE * HEAT_ENDING_SOON_TIME_MINUTES;

const long CYCLE_DURATION = TRANSITION_TIME_MS + HEAT_TIME_MINUTES;

// runs motor until desired desiredFlag is showing
void showFlag(int desiredFlag) {
	int state = digitalRead(desiredFlag);

	if(!state) {
		digitalWrite(FLAG_MOTOR, HIGH);
	}
}


void setup()
{
	epoch = millis(); //start time

	pinMode (NO_HEAT_FLAG, INPUT);  //set pin 2 as input
	pinMode (HEAT_IN_PROGESS_FLAG, INPUT);  //set pin 3 as input
	pinMode (HEAT_ENDING_SOON_FLAG, INPUT);  //set pin 12 as input
	pinMode (FLAG_MOTOR, OUTPUT); //set pin 13 as output
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
	//sets up MY timer
	elapsed_time = millis() - epoch;


	long cycle_time_ms = elapsed_time % CYCLE_DURATION;

	// no heat in progress
	if (cycle_time_ms < TRANSITION_TIME_MS) {
		showFlag(NO_HEAT_FLAG);
	} 

	// HEAT_ENDING for heat ending
	if (cycle_time_ms < (TRANSITION_TIME_MS + HEAT_TIME_MS - HEAT_ENDING_SOON_TIME_MS)) {
		showFlag(HEAT_ENDING_SOON_FLAG);
	} else {
		showFlag(HEAT_IN_PROGESS_FLAG);
	}

}