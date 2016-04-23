#ifndef ROT_H
#define ROT_H
#if ARDUINO >= 100
 #include "Arduino.h"
#endif

class ROT {
  public:
    ROT(uint8_t pin);
    int value(void);
  private:
    uint8_t _pin;
};
#endif
