#ifndef COLOR_H
#define COLOR_H

#include <inttypes.h>

// Color
// -----------------------------------------
// Simple color struct used like a class to
// store and manipulate RGB color data
// Makes it easier for the programmer
struct Color
{
  public: 
    Color(uint8_t r=0, uint8_t g=0, uint8_t b=0);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setColor(uint32_t);
    uint8_t getR();
    uint8_t getG();
    uint8_t getB();
    uint32_t getColor();
    String toString();
    
  private: 
    uint8_t _r, _g, _b;

};

#endif
