#include <LedControl.h>
/* 
 * Create a new controller 
 * Params :
 * int dataPin    The pin on the Arduino where data gets shifted out
 * int clockPin   The pin for the clock
 * int csPin      The pin for selecting the device when data is to be sent
 * int numDevices The maximum number of devices that can be controlled
 * Syntax:
 * LedControl(int dataPin, int clkPin, int csPin, int numDevices);
*/
// Display control with MAX7219
LedControl lc = LedControl(5,7,6,1); 
const int MAX_DIGITS = 8;
boolean dot = false;

const byte numbers[10] = {
  //abcdefg
  B01111110, // ZERO
  B01100000, // ONE
  B00111101, // TWO
  B01111001, // THREE
  B01100011, // FOUR 
  B01011011, // FIVE
  B01011111, // SIX
  B01110000, // SEVEN
  B01111111, // EIGHT
  B01111011, // NINE
};

void setup(void){
  
  // Display
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  
}
 
void loop(void){
  int number, i;
  for(number = 0; number < 10; number++){
    for(i = 0; i<MAX_DIGITS; i++){
      //lc.setDigit(0, i, (byte)number, dot);
      displayNumber(0, i, numbers[number], dot);
    }
    delay(1000);
    //lc.clearDisplay(0);
  }
  dot != dot;
}

void displayNumber(int address, int digit, byte value, boolean dot){
  lc.setRow(address, digit, value);
}
