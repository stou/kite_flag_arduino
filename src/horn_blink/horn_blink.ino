
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

  static int horn_signal_type = HORN_RED;
  long time = millis();

  if(!(time % 1000)) {
    // Serial.print(" ");  
    Serial.println(time);
    // toggle debug pin
  }

  horn_signal(HORN_UPDATE);

  // demonstrate the horn signals by alternating between them
  int signal_cycle_time = HORN_TICK_DURATION_MS * (8 + 3);
  if(!(time % 5000)) {
    horn_signal(horn_signal_type % 3 + 1);
    ++horn_signal_type;

//    horn_signal(HORN_RED);
//    horn_signal(HORN_GREEN);
//    horn_signal(HORN_YELLOW);
  } 
}
