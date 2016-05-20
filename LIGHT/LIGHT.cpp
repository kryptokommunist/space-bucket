#include "LIGHT.h"

LIGHT::LIGHT(uint8_t pin) {

  rel = new REL(pin);

}

void LIGHT::set_schedule(int start_hour, int start_min, int end_hour, int end_min) {

  _start_hour = start_hour;
  _start_min = start_min;
  _end_hour = end_hour;
  _end_min = end_min;

}

void LIGHT::update(DateTime date_time) {

  bool active_time = is_active_time(date_time);

  // turn light on if it is off and it's past the saved start time
  if(active_time && !rel->is_on()) {
    Serial.println("Turned light on.");
    rel->on();
  // if it should be off and is not, turn it off!
} else if (!active_time && rel->is_on()) {
    Serial.println("Turned light off.");
    rel->off();
  }

}

void LIGHT::on(void) {

  rel->on();

}

void LIGHT::off(void) {

  rel->off();

}

bool LIGHT::is_active_time(DateTime date_time) {

  // case hour is enought to tell time frame is safe
  if(date_time.hour > _start_hour && date_time.hour < _end_hour) {

    return true;

    // case we have to look at the minutes
  } else if (_start_hour != _end_hour) {

    if (date_time.hour == _start_hour && date_time.minute >= _start_min) {

        return true;

    // case we have to look at the minutes
    } else if (date_time.hour == _end_hour && date_time.minute < _end_min) {

      return true;

    } else {

      return false;

    }


  } else {

    if(date_time.minute >= _start_min && date_time.minute < _end_min) {

      return true;

    } else {

      return false;

    }

  }

}
