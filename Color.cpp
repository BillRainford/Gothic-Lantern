#include "Arduino.h"
#include "Color.h"

Color::Color(uint8_t r=0, uint8_t g=0, uint8_t b=0){
  _r = r;
  _g = g;
  _b = b;
}

void Color::setColor(uint8_t r, uint8_t g, uint8_t b) {
  _r = r;
  _g = g;
  _b = b;
}

// Set color using 32bit color value
// (eg. 0xFF0000 for red)
void Color::setColor(uint32_t color) {
  uint8_t red = (uint8_t) ((color & 0xFF0000) >> 16);
  uint8_t green = (uint8_t) (color & 0x00FF00) >> 8;
  uint8_t blue = (uint8_t) (color & 0x0000FF);
  setColor(red, green, blue);
}

uint8_t Color::getR(){
  return _r;
}

uint8_t Color::getG() {
  return _g;
}

uint8_t Color::getB() {
  return _b;
}

uint32_t Color::getColor() {
  uint32_t c = 0;
  c += _r << 16;
  c += _g << 8;
  c += _b;
  return c;
  
}

String Color::toString(){
  String rgbString = String("R: ") + _r + " G: " + _g + " B: " + _b;
  return rgbString;
}


