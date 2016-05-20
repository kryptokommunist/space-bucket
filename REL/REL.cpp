#include "REL.h"

REL::REL(uint8_t pin) {

  _pin = pin;
  _is_on = false;
  pinMode(_pin, OUTPUT);

}

bool REL::is_on(void) {

  return _is_on;

}

void REL::on(void) {

  _is_on = true;
  digitalWrite(_pin, HIGH);

}

void REL::off(void) {

  _is_on = false;
  digitalWrite(_pin, LOW);

}
