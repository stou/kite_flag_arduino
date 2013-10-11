//  Author: Rasmus Stougaard


#include "horn.h"

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
#ifdef DEBUG_HORN
    Serial.println("horn type: HORN_RED");
#endif
        break;
      case HORN_GREEN:
        remainingTicks = 4;
#ifdef DEBUG_HORN
    Serial.println("horn type: HORN_GREEN");
#endif
        break;
      case HORN_YELLOW:
        remainingTicks = 6;
#ifdef DEBUG_HORN
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
      digitalWrite(HORN, HIGH);
      break;
    default:
      digitalWrite(HORN, LOW);
      break;    
  }

  long timestamp = millis();
  // run the countdown
  if(remainingTicks && lastTimestamp != timestamp && ! (timestamp % HORN_TICK_DURATION_MS)){
#ifdef DEBUG_HORN
    Serial.print("remainingTicks: ");
    Serial.print(remainingTicks);
    Serial.print(" : ");
    Serial.println(digitalRead(HORN));
#endif
    --remainingTicks;
    lastTimestamp = timestamp;
  }

}
