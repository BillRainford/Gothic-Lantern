#ifndef LAMP_H
#define LAMP_H

#include "Demo.h"
#include "Color.h"

#define DEFAULT_WAIT 200  // default # ms to wait 

static Color stock_colors[] = {Color(255, 255, 255), // White
                               Color(0, 0, 255), // Blue
                               Color(255, 0, 0), // Red
                               Color(255, 255, 0), // Yellow
                               Color(0, 255, 0), // Green
                               Color(0, 255, 255), // Cyan
                               Color(255, 0, 255), // Magenta
                               Color(128, 0, 128), // Purple
                               Color(255, 55, 0) // Orange
                              };
static int NUM_COLORS = sizeof(stock_colors)/sizeof(struct Color);
               
// Lamp
// -----------------------------------------
// Simple class that allows the Gothic Lantern to function 
// as a lamp. Holding a single color. 
class Lamp: public Demo {
public:
  Lamp() { initialized = false; current_color = 0; cycle_colors = true; }
  ~Lamp() {}

  virtual void loop() {
    if(!initialized){
      // Initialize these values here as CircuitPlayground obj
      // is only initialized after this class is instantiated 
      numPixels = CircuitPlayground.strip.numPixels();

      // Read the sound sensor and use it to initialize the random number generator.
      randomSeed(CircuitPlayground.soundSensor());

      initialized = true;
    }
    Serial.println("Lamp Demo:");

    if (cycle_colors) {
      current_color = random(NUM_COLORS);
    }
    Serial.print("Current Color Index: ");Serial.println(current_color);

    for(int i=0; i<numPixels; ++i) {
      CircuitPlayground.strip.setPixelColor(i, stock_colors[current_color].getR(), stock_colors[current_color].getG(), stock_colors[current_color].getB());
    }
    // Show all the pixels.
    CircuitPlayground.strip.show();

    if (cycle_colors) {
      delay(DEFAULT_WAIT);
    }
    
  }

  virtual void modePress() {
    if (cycle_colors) { // Shut off demo mode cycling 
      cycle_colors = false;
      current_color = 0;
    }
    else { // Cycle to next color on each subsequent mode push 
      current_color += 1;
    }
    if (current_color >= NUM_COLORS) { // cycled through all stock colors, resume demo cycling
      current_color = 0;
      cycle_colors = true;
    }
    Serial.print("Cycle Colors: "); Serial.println(cycle_colors);
    Serial.print("Current Color: "); Serial.println(current_color);

  }

private:
  boolean initialized; 
  boolean cycle_colors;
  int current_color;
  int numPixels;
  
};

#endif
