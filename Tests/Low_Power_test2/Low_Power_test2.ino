/*

low power testing. This code gets the chip down to 
less than .1 µA while it is powered down. 

*/

#include "LowPower.h"

const int wakeUpPin = 2;
const int led = 13;
const int del_ay = 200;

void setup()
{
  // Set all of the pins to outputs and set them to the low state.
  // This saves a few µA
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  for ( int i = 3; i < 14; i++){
    pinMode(i,OUTPUT);
  }
  digitalWrite(0,LOW);
  digitalWrite(1,LOW);
  for ( int i = 3; i < 14; i++){
    digitalWrite(i,LOW);
  }
  pinMode(wakeUpPin, INPUT); 
  digitalWrite(wakeUpPin,HIGH);  // turn the internal pull-up
}

void loop() 
{
  for ( int i = 0; i < 6; i++){ // blink the LED
    digitalWrite(led, HIGH);  
    delay(del_ay);            
    digitalWrite(led, LOW);   
    delay(del_ay);            
  }
  
  attachInterrupt(0, wakeUp, LOW);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
  detachInterrupt(0); 
}

void wakeUp()
{
    // Just a handler for the pin interrupt.
}