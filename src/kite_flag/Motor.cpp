//  Author: Rasmus Stougaard

#include "Motor.h"

int Motor::output = 0;

Motor::Motor(const int outputPin) {
  Motor::output = outputPin;

  // turn off motor
  digitalWrite(output, HIGH);
  pinMode (outputPin, OUTPUT);

  // setup inputs for flag switches
  pinMode(NO_HEAT_FLAG, INPUT_PULLUP);
  pinMode(HEAT_IN_PROGESS_FLAG, INPUT_PULLUP);
  pinMode(HEAT_ENDING_SOON_FLAG, INPUT_PULLUP);
  
  // setup LEDs for signalling current flag
  pinMode(LED_FLAG_RED, OUTPUT);
  pinMode(LED_FLAG_GREEN, OUTPUT);
  pinMode(LED_FLAG_YELLOW, OUTPUT);
}


int Motor::getFlagPosition() {
  switch(flagPosition){
  case NO_HEAT_FLAG:
    return 1;
  case HEAT_IN_PROGESS_FLAG:
    return 2;
  case HEAT_ENDING_SOON_FLAG:
    return 3;
  default:
    break;
  }
  return 0;
}

int Motor::getDesiredFlagPosition() {
  switch(desiredFlagPosition){
  case NO_HEAT_FLAG:
    return 1;
  case HEAT_IN_PROGESS_FLAG:
    return 2;
  case HEAT_ENDING_SOON_FLAG:
    return 3;
  default:
    break;
  }
  return 0;
}


// runs motor until desired desiredFlag is showing
void Motor::showFlag(int desiredFlag) {
  desiredFlagPosition = desiredFlag;
  int runMotor = (flagPosition != desiredFlag);

  isInPosition = digitalRead(desiredFlag);

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
    digitalWrite(output, LOW);
  } else {
    flagPosition = desiredFlag;
    digitalWrite(output, HIGH);
  }
}
