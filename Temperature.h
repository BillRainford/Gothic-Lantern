#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "Demo.h"
#include "Color.h"


#define WHITE 0
#define BLUE 1
#define CYAN 2
#define GREEN 3
#define YELLOW 4
#define ORANGE 5
#define RED 6
#define BLACK 7

static Color temperature_colors[] = {Color(255, 255, 255), // White
                                     Color(0, 0, 255), // Blue
                                     Color(0, 255, 255), // Cyan
                                     Color(0, 255, 0), // Green
                                     Color(255, 255, 0), // Yellow
                                     Color(255, 55, 0), // Orange
                                     Color(255, 0, 0), // Red
                                     Color(0, 0, 0) // Off/Black
                                    };
                                    
static int NUM_TEMP_COLORS = sizeof(temperature_colors)/sizeof(struct Color);

static int animation_color_indices[] = { 1, 1, 1, 2, 2, 2, 3, 3, 3, // Cycle through colors from cold to hot
                                         4, 4, 4, 5, 5, 5, 6, 6, 6,
                                         7, 7, 7,
                                         6, 6, 6, 5, 5, 5, 4, 4, 4, // Cycle through colors from hot down to cold
                                         3, 3, 3, 2, 2, 2, 1, 1, 1, 
                                         -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, // Flash white to signal temp reading coming
                                         -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2 // display color for a while
                                       };
static int NUM_FRAMES = sizeof(animation_color_indices)/sizeof(int);
int current_frame = 0; // Current frame in the above 'animation' that cycles through the above array of colors 

const long interval = 100;           // interval at which to increment animations (milliseconds)
const int temp_interval = 50;        // how many intervals (above) before we update the temperature reading (50 100ms intervals == 5 seconds) 
unsigned long previousMillis = 0;

// Temperature
// -----------------------------------------
// Simple class that will set the color based on  
// as a lamp. Holding a single color. 
//
// NOTE: Given this sensor is in a closed lamp
// which can capture heat, it is likely to give 
// readings that seem warmer than 
// the ambient air temp you'll feel in the room
class Temperature: public Demo {
public:
  Temperature() { initialized = false; current_color = 0; color_animation_index = 0; current_temp = 0; update_temp = temp_interval; }
  ~Temperature() {}

  virtual void loop() {
    if(!initialized){
      // Initialize these values here as CircuitPlayground obj
      // is only initialized after this class is instantiated 
      numPixels = CircuitPlayground.strip.numPixels();

      Serial.println("Temperature Demo:");
      
      initialized = true;
    }
    

    unsigned long currentMillis = millis();

    // check to see if it's time to update the animation
    if(currentMillis - previousMillis > interval) {
        // save the last time you updated the animations
        previousMillis = currentMillis;
        update_temp--; // decrease counter keep track of when we should update temperature reading 

        // Update temp after 'temp_interval' number of (Default 100ms) intervals 
        if (update_temp <= 0) {
          update_temp = temp_interval; // reset counter

          // Take new temperature reading 
          current_temp = CircuitPlayground.temperatureF();
          Serial.print("Current Temperature: ");Serial.println(current_temp);
        }
        
        Increment();
    }
    Update();  
  }

  virtual void modePress() {
    // No additional mode options defined for this demo
  }

  void Update() {
    // Update the animation 
    if(animation_color_indices[current_frame] == -1) {
      // Shut off the pixel ring 
      current_color = BLACK;
    }
    else if(animation_color_indices[current_frame] == -2) {
      // Set the color based on temperature reading 
      if (current_temp <= 32) {
        current_color = BLUE;  // Cold
      }
      else if (current_temp > 32 && current_temp <= 64) {
        current_color = CYAN;  // Cool
      }
      else if (current_temp > 64 && current_temp <= 69 ) {
        current_color = GREEN; // Room Temperature (Not too hot, not too cold)
      }
      else if (current_temp > 69 && current_temp <= 80) {
        current_color = YELLOW;  // WARM
      }
      else if (current_temp > 90 && current_temp <= 100) {
        current_color = ORANGE;  // WARMER
      }
      else if (current_temp > 10 ) {
        current_color = RED; // HOT
      }
    }
    else { 
      // Must be in color animation loop 
      current_color = animation_color_indices[current_frame];
    }

    for(int i=0; i<numPixels; ++i) {
      CircuitPlayground.strip.setPixelColor(i, temperature_colors[current_color].getR(), temperature_colors[current_color].getG(), temperature_colors[current_color].getB());
    }
    // Show all the pixels.
    CircuitPlayground.strip.show();
    
  }

  void Increment() {
    // Update Counters
    current_frame += 1;
    if (current_frame >= NUM_FRAMES) {
      current_frame = 0; 
    }
    
  }

private:
  boolean initialized; 
  int current_color;
  int numPixels;
  int color_animation_index;
  int current_temp;
  int update_temp;
  
};

#endif 
