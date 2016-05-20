#ifndef NISTTIME_H
#define NISTTIME_H

#if ARDUINO >= 100
 #include "Arduino.h"
#endif

#include <ESP8266WiFi.h> // wifi functionality

struct DateTime {

  int hour;
  int minute;
  int day;
  int month;
  int year;

};

struct DateTimeString {

  String hour;
  String minute;
  String day;
  String month;
  String year;

};

class NISTTIME {
  public:
    NISTTIME(const char* host, int utc_offset);
    DateTime datetime(void);
    DateTimeString date_string;
  private:
    const int _http_port = 13;
    const char* _host;
    int _utc_offset;
    String get_date(void);
};
#endif
