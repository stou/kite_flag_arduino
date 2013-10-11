/*
  Author: Rasmus Stougaard
 */

#define DEBUG_STATE 1
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
const int led = 13;
const int debugPin = A1;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);  
  pinMode(debugPin, OUTPUT);  
  Serial.begin(9600);
}

const int HORN_UPDATE = 0;
const int HORN_RED = 1;
const int HORN_GREEN = 2;
const int HORN_YELLOW = 3;
const int HORN_TICK_DURATION_MS = 500;


// Unit of sound lenth is controlled by HORN_TICK_DURATION_MS
// sound '-'
// pause ' '
//
// GREEN:  '___'
// YELLOW: '_ ___'
// RED:    '_ _ ___'

void horn_signal(int type) {

  static long lastTimestamp = 0;
  static int remainingTicks = 0;


  if(! remainingTicks){
    switch(type) {
      case HORN_RED:
        remainingTicks = 8;
#ifdef DEBUG_STATE
    Serial.println("horn type: HORN_RED");
#endif
        break;
      case HORN_GREEN:
        remainingTicks = 4;
#ifdef DEBUG_STATE
    Serial.println("horn type: HORN_GREEN");
#endif
        break;
      case HORN_YELLOW:
        remainingTicks = 6;
#ifdef DEBUG_STATE
    Serial.println("horn type: HORN_YELLOW");
#endif
        break;
      default:
      break;    
    }

  }

  //  7654321
  // '_ _ ___'
  //
  switch(remainingTicks) {
    case 1:
    case 2:
    case 3:
    // case 4:
    case 5:
    // case 6:
    case 7:
      digitalWrite(led, HIGH);
      break;
    default:
      digitalWrite(led, LOW);
      break;    
  }

  long timestamp = millis();
  // run the countdown
  if(remainingTicks && lastTimestamp != timestamp && ! (timestamp % HORN_TICK_DURATION_MS)){
#ifdef DEBUG_STATE
    Serial.print("remainingTicks: ");
    Serial.print(remainingTicks);
    Serial.print(" : ");
    Serial.println(digitalRead(led));
#endif
    --remainingTicks;
    lastTimestamp = timestamp;
  }

}

// the loop routine runs over and over again forever:
void loop() {

  long time = millis();

  // if(!(time % 1000)) {
  //   // Serial.print(" ");  
  //   Serial.println(time);
  //   // toggle debug pin
  //   digitalWrite(debugPin, ! digitalRead(debugPin));
  // }

  horn_signal(HORN_UPDATE);

  if(!(time % 5000)) {
    // horn_signal(HORN_RED);
    horn_signal(HORN_GREEN);
    horn_signal(HORN_YELLOW);
  } 

  // if(!(time % 3000)) {
  //   horn_signal(HORN_YELLOW);
  // } 
}
