//  Author: Rasmus Stougaard


#include "Horn.h"
#include <Arduino.h>

int Horn::output = 0;

Horn::Horn(const int outputPin) {
    Horn::output = outputPin;
    digitalWrite(output, HIGH);
    pinMode (outputPin, OUTPUT);
}

// Unit of sound lenth is controlled by HORN_TICK_DURATION_MS
// sound '-'
// pause ' '
//
// GREEN:  '___'
// YELLOW: '_ ___'
// RED:    '_ _ ___'

void Horn::signal(int type) {

  static long lastTimestamp = 0;
  static int remainingTicks = 0;

  if(! remainingTicks){
    switch(type) {
      case HORN_RED:
        remainingTicks = 8;
        break;
      case HORN_GREEN:
        remainingTicks = 4;
        break;
      case HORN_YELLOW:
        remainingTicks = 6;
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
      digitalWrite(output, LOW);
      break;
    default:
      digitalWrite(output, HIGH);
      break;    
  }

  long timestamp = millis();
  // run the countdown
  if(remainingTicks && lastTimestamp != timestamp && ! (timestamp % HORN_TICK_DURATION_MS)){
    --remainingTicks;
    lastTimestamp = timestamp;
  }

}

