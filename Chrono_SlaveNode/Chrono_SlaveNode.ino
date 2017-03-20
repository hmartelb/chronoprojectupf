#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

const int pinIR = 2;
int msg[1] ;                             // Array to be transmitted
RF24 radio(9,10);                        // Create an instance of an RF24 object (radio module) 
const uint64_t pipe = 0xE8E8F0F0E1LL;    // Communication channel
 
void setup(void){
  
  pinMode(pinIR, INPUT);                 // I/O configuration
  Serial.begin(9600);                    // Serial communication
  radio.begin();                         // Initialize the radio
  radio.openWritingPipe(pipe);           // Open the communication channel
  
}          
 
void loop(void){
  
    if(digitalRead(pinIR)) msg[0] = 1;   // The message depends on the pin status
    else msg[0] = 0;
    radio.write(msg, 1);                 // Send the message with the current status of the pin
 
}
