#include "FAN.h"

FAN::FAN(uint8_t pin) {

  _pin = pin;
  speed = 0;

}

void FAN::set_speed(int setspeed) {

  speed = setspeed;
  analogWrite(_pin, speed);

}
