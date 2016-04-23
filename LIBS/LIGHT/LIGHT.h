#ifndef LIGHT_H
#define LIGHT_H

#if ARDUINO >= 100
 #include "Arduino.h"
#endif

#include "REL.h"
#include "NISTTIME.h"

class LIGHT {
  public:
    LIGHT(uint8_t pin);
    void set_schedule(int start_hour, int start_min, int end_hour, int end_min);
    void update(DateTime nist);
    bool is_on(void);
    void on(void);
    void off(void);
  private:
    int _start_hour;
    int _start_min;
    int _end_hour;
    int _end_min;
    REL* rel;
    bool is_active_time(DateTime date_time);
};
#endif
