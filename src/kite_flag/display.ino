
#ifdef HAS_LCD_SUPPORT
#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
#endif


// int heatNumber = 0;


const int BUTTON_RIGHT  = 0;
const int BUTTON_UP     = 1;
const int BUTTON_DOWN   = 2;
const int BUTTON_LEFT   = 3;
const int BUTTON_SELECT = 4;
const int BUTTON_NONE   = 5;

int update_display(long cycle_time_ms)
{
#ifdef HAS_LCD_SUPPORT
  // TODO update the contents on the real display
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):

  long seconds = cycle_time_ms/1000 % 60;
  long minutes = cycle_time_ms/1000/60;

  lcd.setCursor(0, 0);
  lcd.print(heatTimeMinutes);

  lcd.setCursor(15, 1);
  lcd.print(seconds % 10);
  lcd.setCursor(14, 1);
  lcd.print(seconds / 10);
  lcd.setCursor(13, 1);
  lcd.print(":");
  lcd.setCursor(12, 1);
  lcd.print(minutes % 10);
  lcd.setCursor(11, 1);
  lcd.print(minutes / 10);

#else
  // TODO show display contents on serial interface
  Serial.print("cycle time S");
  Serial.println(cycle_time_ms/1000);
  Serial.print("heatTimeMinutes: ");
  Serial.println(heatTimeMinutes);
#endif
}

int read_buttons()
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
void handle_buttons(int lcdKey)
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
