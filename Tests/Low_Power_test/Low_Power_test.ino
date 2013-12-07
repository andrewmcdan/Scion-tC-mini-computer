/*

low power testing. This code gets the chip down to 
6.2 µA while it is powered down.

 */

#include "LowPower.h"
 
// Pin 13 has an LED connected on most Arduino boards.
int led = 13;
int del_ay = 400;

void setup() {                
  pinMode(led, OUTPUT);     
}

void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(del_ay);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(del_ay);               // wait for a second
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(del_ay);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(del_ay);               // wait for a second
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(del_ay);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(del_ay);               // wait for a second
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(del_ay);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(del_ay);               // wait for a second
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(del_ay);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(del_ay);               // wait for a second
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(del_ay);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(del_ay);               // wait for a second

  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}
