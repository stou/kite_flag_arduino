
#ifdef DEBUG_SERIAL
#define DEBUG_MOTOR 1
#endif

// runs motor until desired desiredFlag is showing
void showFlag(int desiredFlag) {

  static int flagPosition;
  int runMotor = (flagPosition != desiredFlag);

  int state = digitalRead(desiredFlag);

#ifdef DEBUG_MOTOR
  Serial.print("s: ");
  Serial.print(state);
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
  
  if(runMotor && state) {
#ifdef DEBUG_MOTOR
    Serial.println("FLAG_MOTOR running");
#endif
    digitalWrite(FLAG_MOTOR, HIGH);
  } else {
#ifdef DEBUG_MOTOR
    Serial.println("FLAG_MOTOR off");
#endif
    flagPosition = desiredFlag;
    digitalWrite(FLAG_MOTOR, LOW);
  }
}

