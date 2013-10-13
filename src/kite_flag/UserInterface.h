//  Author: Rasmus Stougaard

#ifndef _DISPLAY_H
#define _DISPLAY_H 1

// int heatNumber = 0;


class UserInterface {
  public:
    UserInterface();
    ~UserInterface(){};

    // void setup();
    void updateDisplay(long cycle_time_ms);
    void pollButtons();
    long getHeatTimeMinutes() {
      return heatTimeMinutes;
    };

  private:
    void handleButton(int lcdKey);
    int readButtons();
    
    long heatTimeMinutes;

    // LiquidCrystal lcd;
    static const int BUTTON_RIGHT  = 0;
    static const int BUTTON_UP     = 1;
    static const int BUTTON_DOWN   = 2;
    static const int BUTTON_LEFT   = 3;
    static const int BUTTON_SELECT = 4;
    static const int BUTTON_NONE   = 5;
    
};

#endif 
// _DISPLAY_H
