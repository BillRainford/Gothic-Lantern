#ifndef CANDLE_H
#define CANDLE_H

#include "Demo.h"
#include "Color.h"

enum FLAME_HUE {FLAME_RED = 10, 
                FLAME_ORANGE = 35, 
                FLAME_YELLOW = 45,
                FLAME_GREEN = 120,
                FLAME_BLUE = 240,
                FLAME_PURPLE = 270,
                FLAME_PINK = 310};
 // Primary hue of the flame.  This is a value in
 // degrees from 0.0 to 360.0, see HSV color space
 // for different hue values:
 //   https://en.wikipedia.org/wiki/HSL_and_HSV#/media/File:Hsl-hsv_models.svg
 // The value 35 degrees is a nice orange in the
 // middle of red and yellow hues that will look like
 // a flame flickering as the hues animate.
 // The value 45 is a nice yellow in the middle of the 
 // yellow hues that I also liked
 // For the flicker effect the pixels will cycle 
 // around hues that are +/-10 degrees of this value.


// Color gamma correction table, this makes the hues of the NeoPixels
// a little more accurate by accounting for our eye's non-linear color
// sensitivity.  See this great guide for more details:
//   https://learn.adafruit.com/led-tricks-gamma-correction/the-issue
const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

// Candle
// -----------------------------------------
// Simple animation to simulate a candle burning (flickering) 
// Default colors are a yellow and red flicker
// Tweaked from Adafruit samples -- I drop some LEDs etc to give 
// more variation and realistic flickering 
class Candle: public Demo {
public:
  Candle() { 
    initialized = false;
    flame_hue = FLAME_HUE::FLAME_ORANGE;
  }
  ~Candle() {
    delete pixels;
  }

  // Helper to change the color of a NeoPixel on the Circuit Playground board.
  // Will automatically convert from HSV color space to RGB and apply gamma
  // correction.
  void setPixelHSV(Color &c, float h, float s, float v) {
    // Convert HSV to RGB
    float r, g, b = 0.0;
    HSVtoRGB(&r, &g, &b, h, s, v);
    // Lookup gamma correct RGB colors (also convert from 0...1.0 RGB range to 0...255 byte range).
    uint8_t r1 = pgm_read_byte(&gamma8[int(r*255.0)]);
    uint8_t g1 = pgm_read_byte(&gamma8[int(g*255.0)]);
    uint8_t b1 = pgm_read_byte(&gamma8[int(b*255.0)]);
    // Set the color of the pixel.
    c.setColor(r1, g1, b1);
    //Serial.print("New Color Value: "); Serial.println(c.toString());
  }

  // HSV to RGB color space conversion function taken directly from:
  //  https://www.cs.rit.edu/~ncs/color/t_convert.html
  void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v ) {
    int i;
    float f, p, q, t;
    if( s == 0 ) {
      // achromatic (grey)
      *r = *g = *b = v;
      return;
    }
    h /= 60;      // sector 0 to 5
    i = floor( h );
    f = h - i;      // factorial part of h
    p = v * ( 1 - s );
    q = v * ( 1 - s * f );
    t = v * ( 1 - s * ( 1 - f ) );
    switch( i ) {
      case 0:
        *r = v;
        *g = t;
        *b = p;
        break;
      case 1:
        *r = q;
        *g = v;
        *b = p;
        break;
      case 2:
        *r = p;
        *g = v;
        *b = t;
        break;
      case 3:
        *r = p;
        *g = q;
        *b = v;
        break;
      case 4:
        *r = t;
        *g = p;
        *b = v;
        break;
      default:    // case 5:
        *r = v;
        *g = p;
        *b = q;
        break;
    }
  }

  virtual void loop() {
    if(!initialized){
      // Initialize these values here as CircuitPlayground obj
      // is only initialized after this class is instantiated 
      numPixels = CircuitPlayground.strip.numPixels();
      pixels = new Color[numPixels];

      frequencies = new float[numPixels];
      phases = new float[numPixels];

      // Read the sound sensor and use it to initialize the random number generator.
      randomSeed(CircuitPlayground.soundSensor());

      // Precompute random frequency and phase values for each pixel.
      // This gives the flicker an organic but random looking appearance.
      for (int i=0; i<10; ++i) {
        // Generate random floating point frequency values between 1.0 and 4.0.
        frequencies[i] = random(1000, 4000)/1000.0;
        // Generate random floating point phase values between 0 and 2*PI.
        phases[i] = random(1000)/1000.0 * 2.0 * PI;
      }
      initialized = true;
    }

    Serial.println("Candle Demo:");

    // First determine the low and high bounds of the flicker hues.
    // These are +/- 10 degrees of the specified target hue and will
    // wrap around to the start/end as appropriate.
    float lowHue = fmod(flame_hue - 10.0, 360.0);
    float highHue = fmod(flame_hue + 10.0, 360.0);
    // Convert time from milliseconds to seconds.
    float current = millis();
    float t = current/1000.0;

    for (int i=0;i<numPixels;i++){
      // This pixel should be lit, so compute its hue by composing
      // a low frequency / slowly changing sine wave with a high
      // frequency / fast changing cosine wave.  This means the candle will
      // pulse and jump around in an organice but random looking way.
      // The frequencies and phases of the waves are randomly generated at
      // startup in the setup function.
      // Low frequency wave is a sine wave with random freuqency between 1 and 4,
      // and offset by a random phase to keep pixels from all starting at the same
      // color:
      float lowFreq  = sin(2.0*PI*frequencies[i]*t + phases[i]);
      // High frequency is a faster changing cosine wave that uses a different
      // pixel's random frequency.
      float highFreq = cos(3.0*PI*frequencies[(i+5)%10]*t);
      // Add the low and high frequency waves together, then interpolate their value
      // to a hue that's +/-10% of the configured target hue.
      float h = lerp(lowFreq+highFreq, -2.0, 2.0, lowHue, highHue);
      // Alter the saturation level to further make the candle flicker look realistic
      float s = lerp(random(60, 100), 1, 100, 0.0, 1.0);
      // Shut off a given pixel to further refine the candle flicker effect 
      float v = lerp(random(1, 10), 1, 10, 0.0, 1.0);
      if(v < 0.05) {
        v = 0.0;
      }
      else {
        v = 1.0;
      }
      setPixelHSV(pixels[i], h, s, v);
      CircuitPlayground.strip.setPixelColor(i, pixels[i].getR(), pixels[i].getG(), pixels[i].getB());

      // Useful Debug Info (uncomment to use it) 
      //Serial.println(pixels[i].toString());
      //Serial.println(pixels[i].getColor());
      //Serial.print("Red Binary: "); Serial.println(pixels[i].getR(), BIN);
      //Serial.print("Green Binary: "); Serial.println(pixels[i].getG(), BIN);
      //Serial.print("Blue Binary: "); Serial.println(pixels[i].getB(), BIN);
      //Serial.print("Combined Binary: ");Serial.println(pixels[i].getColor(), BIN);
    }
      
    // Show all the pixels.
    CircuitPlayground.strip.show();
    delay(random(100));
  }

  virtual void modePress() {
    if(flame_hue == FLAME_HUE::FLAME_ORANGE) {
      flame_hue = FLAME_HUE::FLAME_YELLOW;
    }
    else if (flame_hue == FLAME_HUE::FLAME_YELLOW) {
      flame_hue = FLAME_HUE::FLAME_RED;
    }
    else if (flame_hue == FLAME_HUE::FLAME_RED) {
      flame_hue = FLAME_HUE::FLAME_GREEN;
    }
    else if (flame_hue == FLAME_HUE::FLAME_GREEN) {
      flame_hue = FLAME_HUE::FLAME_BLUE;
    }
    else if (flame_hue == FLAME_HUE::FLAME_BLUE) {
      flame_hue = FLAME_HUE::FLAME_PURPLE;
    }
    else if (flame_hue == FLAME_HUE::FLAME_PURPLE) {
      flame_hue = FLAME_HUE::FLAME_PINK;
    }
    else if (flame_hue == FLAME_HUE::FLAME_PINK) {
      flame_hue = FLAME_HUE::FLAME_ORANGE;
    }
    else { // Default is my favorite color 
      flame_hue = FLAME_HUE::FLAME_ORANGE;
    }
  }

private:
  boolean initialized; 
  int numPixels;
  Color* pixels;
  float* frequencies;
  float* phases;
  FLAME_HUE flame_hue;
  
};


#endif
