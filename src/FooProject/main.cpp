#include <Arduino.h>
#include <Wire.h>
#include "FooLib.h"

FooClass FooObject;

// void setup() {

// 	Serial.begin(115200);
// 	delay(1000);

// }

// void loop() {

// 	Serial.println("Hello world");
// 	FooObject.firstFooMethod();
// 	delay(1000);
// 	FooObject.secondFooMethod();
// 	delay(1000);

// }

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);         
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);                      // wait for a second
}

