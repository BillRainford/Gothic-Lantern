#ifndef ZAPPER_H
#define ZAPPER_H

#include "Demo.h"
#include "Color.h"

#define ZAP_FREQUENCY 5 // Lower number means less frequent zap

// Zapper
// -----------------------------------------
// Simple animation to simulate an old school bug zapper
// Default color is a purple-ish blue
// Makes use of Piezo Speaker to simulate zapping sound
class Zapper: public Demo {
public:
  Zapper() { 
    initialized = false;
  }
  ~Zapper() {
    delete pixels;
  }

  
  virtual void loop() {
    if(!initialized){
      // Initialize these values here as CircuitPlayground obj
      // is only initialized after this class is instantiated 
      numPixels = CircuitPlayground.strip.numPixels();
      pixels = new Color[numPixels];

      // Read the sound sensor and use it to initialize the random number generator.
      randomSeed(CircuitPlayground.soundSensor());

      Serial.println("Zapper Demo:");

      initialized = true;
    }
    // Loops happen pretty fast and we want the zaps to occur 
    // at random times but not too often
    int zap = random(1000);
    Serial.print("Random Zap value:"); Serial.println(zap);

    if (zap < ZAP_FREQUENCY) {
      // Found a bug to zap
      // Play bug zapping sound and related animation 
      int zappedBug = random(numPixels); // Single LED to light when zap happens
      Serial.print("Zapped Bug:"); Serial.println(zappedBug);
      for (int i=0;i<numPixels;i++){
        Serial.print("Setting led number:"); Serial.println(i);
        if(i == zappedBug) {
          pixels[i].setColor(25, 0, 255); // HSV 246, 100, 100 
          CircuitPlayground.strip.setPixelColor(i, pixels[i].getR(), pixels[i].getG(), pixels[i].getB());
        }
        else {
          pixels[i].setColor(0, 0, 0);  // Turn Off Other Pixels
          CircuitPlayground.strip.setPixelColor(i, pixels[i].getR(), pixels[i].getG(), pixels[i].getB());
        }
      }
      CircuitPlayground.strip.show();
      delay(20);
      for (int i=0;i<numPixels;i++){
        pixels[i].setColor(255, 255, 255);  // White Strobe
        CircuitPlayground.strip.setPixelColor(i, pixels[i].getR(), pixels[i].getG(), pixels[i].getB());
      }
      CircuitPlayground.strip.show();
      delay(20);
      for (int i=0;i<numPixels;i++){
        pixels[i].setColor(0, 0, 0);  // Off
        CircuitPlayground.strip.setPixelColor(i, pixels[i].getR(), pixels[i].getG(), pixels[i].getB());
      }
      pixels[zappedBug].setColor(25, 0, 255); // purple where bug is
      CircuitPlayground.strip.show();
      delay(20);
      for (int i=0;i<numPixels;i++){
        pixels[i].setColor(255, 255, 255);  // White Strobe
        CircuitPlayground.strip.setPixelColor(i, pixels[i].getR(), pixels[i].getG(), pixels[i].getB());
      }
      CircuitPlayground.strip.show();
      delay(50);
      for (int i=0;i<numPixels;i++){
        pixels[i].setColor(0, 0, 0);  // Off
        CircuitPlayground.strip.setPixelColor(i, pixels[i].getR(), pixels[i].getG(), pixels[i].getB());
      }
      pixels[zappedBug].setColor(25, 0, 255); // purple where bug is
      CircuitPlayground.strip.show();
      delay(20);
      // Back to steady state color
      for (int i=0;i<numPixels;i++){
        pixels[i].setColor(25, 0, 255);  // HSV 246, 100, 100 
        CircuitPlayground.strip.setPixelColor(i, pixels[i].getR(), pixels[i].getG(), pixels[i].getB());
      }
      CircuitPlayground.strip.show();
     
      CircuitPlayground.playTone(70, 500);  // Bug Zapping sound      
    }
    else {
      // Display the big light in steady state (purple)
      for (int i=0;i<numPixels;i++){
        pixels[i].setColor(25, 0, 255);  // HSV 246, 100, 100 
        CircuitPlayground.strip.setPixelColor(i, pixels[i].getR(), pixels[i].getG(), pixels[i].getB());
      }
      CircuitPlayground.strip.show();
    }
  }

  virtual void modePress() {
    // No modes defined for this demo
  }

private:
  boolean initialized; 
  int numPixels;
  Color* pixels;

};

#endif
