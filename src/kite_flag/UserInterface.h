//  Author: Rasmus Stougaard

#ifndef _USERINTERFACE_H
#define _USERINTERFACE_H 1

#define DEBUG_DISPLAY 1

#define HAS_LCD_SHIELD_CONNECTED 1

#include <LiquidCrystal.h>

class UserInterface {
  public:
    UserInterface(LiquidCrystal *lcdObj);
    ~UserInterface(){};

    // void setup();
    void updateDisplay();
    void pollButtons();
    long getHeatTimeMinutes() {
      return heatTimeMinutes;
    };

    void setTime(long cycleTimeMS) {
        seconds = cycleTimeMS/1000 % 60;
        minutes = cycleTimeMS/1000/60;
    }

    void setFlag(int flag){
      flagPosition = flag;
    }

  private:
    void handleButton(int lcdKey);
    int readButtons();
    
    int seconds;
    int minutes;
    
    long heatTimeMinutes;

    LiquidCrystal *lcd;
    static const int BUTTON_RIGHT  = 0;
    static const int BUTTON_UP     = 1;
    static const int BUTTON_DOWN   = 2;
    static const int BUTTON_LEFT   = 3;
    static const int BUTTON_SELECT = 4;
    static const int BUTTON_NONE   = 5;
    
    static const long MAX_HEAT_TIME = 30;
    static const long MIN_HEAT_TIME = 1;

    int flagPosition;
};

#endif 
// _USERINTERFACE_H
