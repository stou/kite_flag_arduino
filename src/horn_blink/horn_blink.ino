
#define DEBUG_HORN

#include "horn.h"

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
const int HORN = 13;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(HORN, OUTPUT);  
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  horn_demo();
}
