// Circuit Playground Gothic Lantern
// -----------------------------------------
// Thingiverse Models for 3D Printing:
// Gothic Lantern: https://www.thingiverse.com/thing:2395716
// Circuit Playground Electronics Enclosure for Gothic Lantern: https://www.thingiverse.com/thing:2418725
// Lid for Circuit Playground Electronics Enclosure: https://www.thingiverse.com/thing:2428996 
//
// Electronics:
// Adafruit Circuit Playground Classic: https://www.adafruit.com/product/3000 
//
// NOTES: 
// Thank you to Tony DiCola for Circuit Playground example code on the Adafruit Learning System 
// which helped me get familiar with this device. 
//
// See the header files (*.h) for information on what each animation can do
//
// When this sketch is running, you can cycle through the animations by using the buttons
// on the Circuit Playground Classic or by tilting it to make use or the X and Y axis
// accelerometer readings. 
//
// Author: Bill Rainford @TinWhiskerzBlog @TheRainford
// License: MIT License (https://opensource.org/licenses/MIT)
#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_SleepyDog.h"

// Include all the demos, note that each demo is defined in a separate class to keep the sketch
// code below clean and simple.
#include "Demo.h"
#include "Candle.h"
#include "VUMeter.h"
#include "Zapper.h"
#include "RainbowCycle.h"
#include "Lamp.h"
#include "Temperature.h"

// Define the acceleration value that we want to react to as an input
#define X_THRESHOLD 4
#define Y_THRESHOLD 4

// Create an instance of each Demo class.
Candle candle;
VUMeter vuMeter;
Zapper zapper;
RainbowCycle rainbowCycle;
Lamp lamp;
Temperature temperature;


// Make a list of all demo class instances and keep track of the currently selected one.
int currentDemo = 0;
Demo*  demos[] = {
  &candle,
  &zapper,
  &vuMeter,
  &rainbowCycle,
  &temperature,
  &lamp
};

void setup() {
  // Initialize serial port and circuit playground library
  Serial.begin(115200);
  Serial.println("Circuit Playground Gothic Lantern!");
  CircuitPlayground.begin(255);
}

void loop() {
  // Check if slide switch is on the left (false) and go to sleep.
  while (!CircuitPlayground.slideSwitch()) {
    // Turn off the pixels, then go into deep sleep for a second.
    CircuitPlayground.clearPixels();
    Watchdog.sleep(1000);
  }

  // Check for any button presses by checking their state twice with
  // a delay inbetween.  If the first press state is different from the
  // second press state then something was pressed/released!
  bool leftFirst = CircuitPlayground.leftButton();
  bool rightFirst = CircuitPlayground.rightButton();
  delay(10);

  // Run current mode's main loop.
  demos[currentDemo]->loop();

  // Now check for buttons that were released.
  bool leftSecond = CircuitPlayground.leftButton();
  bool rightSecond = CircuitPlayground.rightButton();

  float xAccel = CircuitPlayground.motionX();
  //Serial.print("X Axis Acceleration is: "); Serial.println(xAccel, DEC);
  float yAccel = CircuitPlayground.motionY();
  //Serial.print("Y Axis Acceleration is: "); Serial.println(yAccel, DEC);

  // Left button will change the current demo.
  // X Axis acceleration will also change current demo. 
  if ((leftFirst && !leftSecond) || (xAccel > X_THRESHOLD)) {
    // Turn off all the pixels when entering new mode.
    CircuitPlayground.clearPixels();

    if (xAccel > 5){
      // Delay 2 seconds as hitting the lantern sometimes results
      // in a few high x acceleration readings and jumps around
      delay(2000);
    }
    // Increment the current demo (looping back to zero if at end).
    currentDemo += 1;
    if (currentDemo >= (sizeof(demos)/sizeof(Demo*))) {
      currentDemo = 0;
    }
    Serial.print("Changed to demo: "); Serial.println(currentDemo, DEC);
  }


  // Right button will change the mode of the current demo.
  if ((rightFirst && !rightSecond) || (yAccel > Y_THRESHOLD)) {
    demos[currentDemo]->modePress();
  }
}

