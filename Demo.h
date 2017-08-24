#ifndef DEMO_H
#define DEMO_H

#define MIC_PIN         A4  // Microphone is attached to this analog pin (A4 for circuit playground)

static unsigned int sample;


// Define each mode with the following interface for a loop and modePress
// function that will be called during the main loop and if the mode button
// was pressed respectively.  It's up to each mode implementation to fill
// these in and add their logic.
class Demo {
public:
  virtual ~Demo() {}

  virtual void loop() = 0;
  virtual void modePress() = 0;
};

// Linear interpolation function is handy for all the modes to use.
float lerp(float x, float xmin, float xmax, float ymin, float ymax) {
  if (x >= xmax) {
    return ymax;
  }
  if (x <= xmin) {
    return ymin;
  }
  return ymin + (ymax-ymin)*((x-xmin)/(xmax-xmin));
}

#endif
