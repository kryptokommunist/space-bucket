#include "FAN.h"

FAN::FAN(uint8_t pin) {

  _pin = pin;
  speed = 0;

}

void FAN::set_speed(int fan_speed) {

  speed = fan_speed;
  analogWrite(_pin, speed);

}
