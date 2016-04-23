#if ARDUINO >= 100
 #include "Arduino.h"
#endif

class FAN {
  public:
    FAN(uint8_t pin);
    void set_speed(int speed);
    int speed;
  private:
    int _pin;
};
