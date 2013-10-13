//  Author: Rasmus Stougaard

#ifndef _MOTOR_H
#define _MOTOR_H 1

#define DEBUG_HORN 1
#include <Arduino.h>

class Motor {
public:

  Motor(int outputPin);
  ~Motor(){};

  void red()
  {
    showFlag(NO_HEAT_FLAG);
  };
  void green()
  {
    showFlag(HEAT_IN_PROGESS_FLAG);
  };
  void yellow()
  {
    showFlag(HEAT_ENDING_SOON_FLAG);
  };

  void demo();

private:
  int flagPosition;
  int isInPosition;
  void showFlag(int desiredFlag);
  static int output;

  // static const int FLAG_UPDATE = 0;
  static const int FLAG_RED = 1;
  static const int FLAG_GREEN = 2;
  static const int FLAG_YELLOW = 3;
  static const int FLAG_TICK_DURATION_MS = 300;


  static const int LED_FLAG_RED = A1;
  static const int LED_FLAG_GREEN = A2;
  static const int LED_FLAG_YELLOW = A3;

  // red flag
  static const int NO_HEAT_FLAG = 2; 
  // green flag
  static const int HEAT_IN_PROGESS_FLAG = 3;
  // yellow flag
  static const int HEAT_ENDING_SOON_FLAG = 12;

};

#endif 
// _MOTOR_H
