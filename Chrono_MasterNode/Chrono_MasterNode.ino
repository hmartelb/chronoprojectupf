#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

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

// Radio Communication
int msg[1], lastmsg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

// Control variables
unsigned long start, split, time;
boolean isActive;
const int pinIR = 2; 
const int pinBuzzer = 3;
int state, laststate;

void setup(void){

  // I/O Pins Configuration
  pinMode(pinIR, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  
  // Display
  lc.shutdown(0,false);
  lc.setIntensity(0,15);
  lc.clearDisplay(0);
  displayZeros(0);
  
  // Communications
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();

  // Control variables
  isActive = false;
  state = 0;
  laststate = 0;
  lastmsg[0] = 1;
  msg[0] = 0;
}
 
void loop(void){
  state = digitalRead(pinIR);              // Get the state of the pinIR

  if (radio.available()){                
      int done = radio.read(msg, 1);
      if(lastmsg[0] == 0 && msg[0] == 1){  // Rising change (cut in the remote node)
        click();                           // Start/Stop the clock
      }
      lastmsg[0] = msg[0];                 // Assign the lastmsg for next iteration
  }

  if(laststate == 0 && state == 1){    // Rising change (cut in this node)
    click();                           // Start/Stop the clock
  }
  
  if(isActive){
    displayTime();                         // Send the time to the serial monitor and the MAX7219
  }
  laststate = state;                       // Assing the laststate for next iteration
}

void displayTime(){
  unsigned long elapsed = millis() - start;  // Get the elapsed time 
  int h,m,s,ms;                              
  unsigned long over;

  h = int(elapsed/3600000);                  // Calculate for h:min:s:ms
  over = elapsed%3600000;
  m = int(over/60000);
  over = over%60000;
  s = int(over/1000);
  ms = over%1000;
  
  char buffer[12]=" ";                       // Print the time in the serial monitor
  char* format="%0.2i:%0.2i.%0.3i";
  sprintf(buffer, format, m, s, ms);
  Serial.println(buffer); 

  // Now print the number digit by digit
  displayNumber(0,7,(byte)(h%10),false);
  displayNumber(0,6,(byte)(m/10),false);
  displayNumber(0,5,(byte)(m%10),true);
  displayNumber(0,4,(byte)(s/10),false);
  displayNumber(0,3,(byte)(s%10),true);
  displayNumber(0,2,(byte)((ms/100)%10),false);
  displayNumber(0,1,(byte)((ms/10)%10),false);
  displayNumber(0,0,(byte)ms%10,false);
}

void displayNumber(int address, int digit, int n, boolean dot){
  byte value = numbers[n];
  bitWrite(value, 7, dot);
  lc.setRow(address, digit, value);
}

void displayZeros(int address){
  int i;
  for(i=0; i<MAX_DIGITS; i++){
      boolean dotState = false;
      if(i == 3 || i == 5) dotState = true;
      displayNumber(address, i, 0, dotState);
  }  
}

void click(){
  if(isActive){
    split = millis();
    time = split - start;
  } else {
    displayZeros(0);
    start = millis();
  }
  isActive = !isActive;
  digitalWrite(pinBuzzer, HIGH);
  delay(150);                    // Delay of 150ms for IR sensor debouncing
  digitalWrite(pinBuzzer, LOW);
}

