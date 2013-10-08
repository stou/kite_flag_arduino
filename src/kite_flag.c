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
// #define DEBUG_STATE 1

#ifndef DEBUG_STATE
#define HAS_LCD_SUPPORT 1
#endif

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

#ifdef DEBUG_STATE
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

// boot time
long epoch; 

int heatNumber = 0;
int heatTimeMinutes = 10;


#ifdef HAS_LCD_SUPPORT
#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
#endif



const int BUTTON_RIGHT  = 0;
const int BUTTON_UP     = 1;
const int BUTTON_DOWN   = 2;
const int BUTTON_LEFT   = 3;
const int BUTTON_SELECT = 4;
const int BUTTON_NONE   = 5;

int update_display(long cycle_time_ms)
{
#ifdef HAS_LCD_SUPPORT
  // TODO update the contents on the real display
	  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print("cycle time S");
  lcd.setCursor(0, 1);
  // print the number of seconds since cycle started
  lcd.print(cycle_time_ms/1000);

#else
  // TODO show display contents on serial interface
  Serial.print("cycle time S");
  Serial.println(cycle_time_ms/1000);
  Serial.print("heatTimeMinutes: ");
  Serial.println(heatTimeMinutes);
#endif
}

int read_buttons()
{
  int adcKeyIn = analogRead(0);      // read the value from the sensor 
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  
#if 1
  if (adcKeyIn > 1000) return BUTTON_NONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 use this threshold
  if (adcKeyIn < 50)   return BUTTON_RIGHT;  
  if (adcKeyIn < 250)  return BUTTON_UP; 
  if (adcKeyIn < 450)  return BUTTON_DOWN; 
  if (adcKeyIn < 650)  return BUTTON_LEFT; 
  if (adcKeyIn < 850)  return BUTTON_SELECT;  
#else
  // For V1.0 use this threshold
  if (adcKeyIn < 50)   return BUTTON_RIGHT;  
  if (adcKeyIn < 195)  return BUTTON_UP; 
  if (adcKeyIn < 380)  return BUTTON_DOWN; 
  if (adcKeyIn < 555)  return BUTTON_LEFT; 
  if (adcKeyIn < 790)  return BUTTON_SELECT;   
#endif
  return BUTTON_NONE;  // when all others fail, return this...
}

// depending on which button was pushed, we perform an action
void handle_buttons(int lcdKey)
{
  switch (lcdKey){
  case BUTTON_RIGHT:
    heatNumber++;
    break;
  case BUTTON_LEFT:
    heatNumber--;
    break;
  case BUTTON_UP:
    heatTimeMinutes++;
    break;
  case BUTTON_DOWN:
    heatTimeMinutes--;
    break;
  // case BUTTON_SELECT:
  //   lcd.print(GREEN);
  //   break;
  // case BUTTON_NONE:
  //   lcd.print("Time left");
    break;
  default:
    break;
  }
}

// runs motor until desired desiredFlag is showing
void showFlag(int desiredFlag) {
  int state = digitalRead(desiredFlag);
  
  switch(desiredFlag) {
  case NO_HEAT_FLAG:
    digitalWrite(LED_FLAG_RED, HIGH);
    digitalWrite(LED_FLAG_GREEN, LOW);
    digitalWrite(LED_FLAG_YELLOW, LOW);
    break;
  case HEAT_IN_PROGESS_FLAG:
    digitalWrite(LED_FLAG_RED, LOW);
    digitalWrite(LED_FLAG_GREEN, HIGH);
    digitalWrite(LED_FLAG_YELLOW, LOW);
    break;
  case HEAT_ENDING_SOON_FLAG:
    digitalWrite(LED_FLAG_RED, LOW);
    digitalWrite(LED_FLAG_GREEN, LOW);
    digitalWrite(LED_FLAG_YELLOW, HIGH);
    break;
  default:
    break;
  }
  
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
  
  // setup LEDs for signalling current flag
  pinMode(LED_FLAG_RED, OUTPUT);
  pinMode(LED_FLAG_GREEN, OUTPUT);
  pinMode(LED_FLAG_YELLOW, OUTPUT);
  
#ifdef HAS_LCD_SUPPORT
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
#endif
  
#ifdef DEBUG_STATE
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
  
  int lcdKey = read_buttons();
  handle_buttons(lcdKey);

  update_display(cycle_time_ms);
  
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
