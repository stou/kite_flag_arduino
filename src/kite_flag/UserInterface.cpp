//  Author: Rasmus Stougaard

#include "UserInterface.h"
#include <Arduino.h>
#include <LiquidCrystal.h>

UserInterface::UserInterface(LiquidCrystal *lcdObj){
  // // select the pins used on the LCD panel
  // LiquidCrystal lcd( rs, enable, d0, d1, d2, d3);

  heatTimeMinutes = 10;
   // set up the LCD's number of columns and rows: 
  lcd->begin(16, 2);

  lcd = lcdObj;
}

void UserInterface::updateDisplay()
{
  // TODO update the contents on the real display
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):

  lcd->setCursor(0, 0);
  lcd->print(heatTimeMinutes);

  lcd->setCursor(15, 1);
  lcd->print(seconds % 10);
  lcd->setCursor(14, 1);
  lcd->print(seconds / 10);
  lcd->setCursor(13, 1);
  lcd->print(":");
  lcd->setCursor(12, 1);
  lcd->print(minutes % 10);
  lcd->setCursor(11, 1);
  lcd->print(minutes / 10);

#ifdef DEBUG_DISPLAY
  Serial.print("cycle time: ");
  Serial.print(minutes);
  Serial.print(":");
  Serial.println(seconds);
  Serial.print("heatTimeMinutes: ");
  Serial.println(heatTimeMinutes);
#endif
}

void UserInterface::pollButtons()
{
  int lcdKey = readButtons();
  handleButton(lcdKey);

}

int UserInterface::readButtons()
{
  int adcKeyIn = analogRead(0);      // read the value from the sensor 
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  
#if 1
  if (adcKeyIn > 1000) return BUTTON_NONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 use this threshold
  if (adcKeyIn < 50)   return BUTTON_RIGHT;  
  if (adcKeyIn < 250)  return BUTTON_UP; 
  if (adcKeyIn < 450)  return BUTTON_DOWN; 
  if (adcKeyIn < 650)  return BUTTON_LEFT; 
  if (adcKeyIn < 850)  return BUTTON_SELECT;  
#else
  // For V1.0 use this threshold
  if (adcKeyIn < 50)   return BUTTON_RIGHT;  
  if (adcKeyIn < 195)  return BUTTON_UP; 
  if (adcKeyIn < 380)  return BUTTON_DOWN; 
  if (adcKeyIn < 555)  return BUTTON_LEFT; 
  if (adcKeyIn < 790)  return BUTTON_SELECT;   
#endif
  return BUTTON_NONE;  // when all others fail, return this...
}

// depending on which button was pushed, we perform an action
void UserInterface::handleButton(int lcdKey)
{
  static int lastButton;

  if(lastButton == lcdKey) {
  	return;
  }
  lastButton = lcdKey;

  switch (lcdKey){
  // case BUTTON_RIGHT:
  //   heatNumber++;
  //   break;
  // case BUTTON_LEFT:
  //   heatNumber--;
  //   break;
  case BUTTON_UP:
    heatTimeMinutes++;
    break;
  case BUTTON_DOWN:
    heatTimeMinutes--;
    break;
  // case BUTTON_SELECT:
  //   lcd.print(GREEN);
  //   break;
  // case BUTTON_NONE:
  //   lcd.print("Time left");
    break;
  default:
    break;
  }
}
