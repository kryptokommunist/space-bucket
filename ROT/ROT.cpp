#include "ROT.h"

ROT::ROT(uint8_t pin) {

  _pin = pin;

}

int ROT::value(void) {

  return analogRead(_pin);

}
