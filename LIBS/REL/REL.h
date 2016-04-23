#ifndef REL_H
#define REL_H

#if ARDUINO >= 100
 #include "Arduino.h"
#endif

class REL {
  public:
    REL(uint8_t pin);
    void on(void);
    void off(void);
    bool is_on(void);
  private:
    uint8_t _pin;
    bool _is_on;
};
#endif
