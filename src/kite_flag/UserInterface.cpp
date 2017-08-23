//  Author: Rasmus Stougaard

#include "UserInterface.h"
#include <Arduino.h>
#include <LiquidCrystal.h>


byte UserInterface::customBackslash[8] = {
  0b00000,
  0b10000,
  0b01000,
  0b00100,
  0b00010,
  0b00001,
  0b00000,
  0b00000
};
    
//const char UserInterface::motorGlyphs[] = "-\\|/";
const char UserInterface::motorGlyphs[] =   {'-',customBackslashCode, '|', '/'};

UserInterface::UserInterface(LiquidCrystal *lcdObj){
  // // select the pins used on the LCD panel
  // LiquidCrystal lcd( rs, enable, d0, d1, d2, d3);

  heatTimeMinutes = 10L;
  
  lcd = lcdObj;

  // create missing backslash char, see https://omerk.github.io/lcdchargen/
  lcd->createChar(customBackslashCode, customBackslash);

   // set up the LCD's number of columns and rows: 
  lcd->begin(16, 2);
}

char UserInterface::getUiFlagDescription(int flag) {
  switch(flag){
    case 1:
      return 'R';
    case 2:
      return 'G';
    case 3:
      return 'Y';
    default:
     return '?'; 
  }
}

char UserInterface::getMotorRunningGlyph() {
  static int updateIteration = 0;
  return motorGlyphs[updateIteration++ % 4];
}

void UserInterface::updateDisplay()
{
  
#ifdef HAS_LCD_SHIELD_CONNECTED
  // TODO update the contents on the real display
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):

  lcd->setCursor(0, 0);
  if(10 > heatTimeMinutes) {
    lcd->print(" ");
  }
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

  // DEBUG
  lcd->setCursor(4,0);
  lcd->print("F:");
  lcd->setCursor(6,0);
  lcd->print(getUiFlagDescription(flagPosition));
  lcd->setCursor(7,0);
  lcd->print(getUiFlagDescription(desiredFlagPosition));

  lcd->setCursor(9,0);
  lcd->print("M:");
  lcd->setCursor(11,0);
  lcd->print(digitalRead(11) ? '_' : getMotorRunningGlyph());
  
  lcd->setCursor(13,0);
  lcd->print(digitalRead(2) ? "R" : "r");
  lcd->setCursor(14,0);
  lcd->print(digitalRead(3) ? "G" : "g");
  lcd->setCursor(15,0);
  lcd->print(digitalRead(12) ? "Y" : "y");
  
#endif

#ifdef DEBUG_DISPLAY

  Serial.print("heat duration: ");
  Serial.print(getHeatTimeMinutes());
  Serial.print(" flag: ");
  Serial.print(getUiFlagDescription(flagPosition));
  Serial.print(" desired flag: ");
  Serial.print(getUiFlagDescription(desiredFlagPosition));
  Serial.print(" cycle time: ");
  Serial.print(minutes);
  Serial.print(":");
  Serial.print(seconds);

  Serial.print(" SW: ");
  Serial.print(digitalRead(2));
  Serial.print(digitalRead(3));
  Serial.print(digitalRead(12));

  Serial.print(" Motor: ");
  Serial.print(digitalRead(11) ? "OFF" : "ON " );
  Serial.print(" In pos: ");
  Serial.println(flagIsInPosition ? "NO " : "YES");
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
    if(MAX_HEAT_TIME > heatTimeMinutes){
      heatTimeMinutes++;
    }
    break;
  case BUTTON_DOWN:
    if(MIN_HEAT_TIME < heatTimeMinutes){
      heatTimeMinutes--;
    }
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
