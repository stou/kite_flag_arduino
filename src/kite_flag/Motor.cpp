//  Author: Rasmus Stougaard

#include "Motor.h"

int Motor::output = 0;

Motor::Motor(const int outputPin) {
  pinMode (outputPin, OUTPUT);
  Motor::output = outputPin;

  // setup inputs for flag switches
  pinMode(NO_HEAT_FLAG, INPUT_PULLUP);
  pinMode(HEAT_IN_PROGESS_FLAG, INPUT_PULLUP);
  pinMode(HEAT_ENDING_SOON_FLAG, INPUT_PULLUP);
  
  // setup LEDs for signalling current flag
  pinMode(LED_FLAG_RED, OUTPUT);
  pinMode(LED_FLAG_GREEN, OUTPUT);
  pinMode(LED_FLAG_YELLOW, OUTPUT);

}


const char* Motor::getFlagPosition() {
  switch(flagPosition){
  case NO_HEAT_FLAG:
    return "red";
  case HEAT_IN_PROGESS_FLAG:
    return "green";
  case HEAT_ENDING_SOON_FLAG:
    return "yellow";
  default:
    break;
  }
  return "";
}


// runs motor until desired desiredFlag is showing
void Motor::showFlag(int desiredFlag) {
  int runMotor = (flagPosition != desiredFlag);

  isInPosition = digitalRead(desiredFlag);

#ifdef DEBUG_MOTOR
  Serial.print("s: ");
  Serial.print(isInPosition);
  Serial.print(runMotor);
  Serial.print(" ");
  Serial.println(desiredFlag);
#endif

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
  
  if(runMotor && isInPosition) {
#ifdef DEBUG_MOTOR
    Serial.println("FLAG_MOTOR running");
#endif
    digitalWrite(output, HIGH);
  } else {
#ifdef DEBUG_MOTOR
    Serial.println("FLAG_MOTOR off");
#endif
    flagPosition = desiredFlag;
    digitalWrite(output, LOW);
  }
}
