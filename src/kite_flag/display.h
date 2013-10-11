//  Author: Rasmus Stougaard

#ifndef _DISPLAY_H
#define _DISPLAY_H 1

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

// int heatNumber = 0;

const int BUTTON_RIGHT  = 0;
const int BUTTON_UP     = 1;
const int BUTTON_DOWN   = 2;
const int BUTTON_LEFT   = 3;
const int BUTTON_SELECT = 4;
const int BUTTON_NONE   = 5;

#endif 
// _DISPLAY_H
