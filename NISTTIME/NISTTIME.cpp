#include "NISTTIME.h"

NISTTIME::NISTTIME(const char* host, int utc_offset) {

    _host = host;
    _utc_offset = utc_offset;
    date_string = {"", "", "", "", ""};

}

DateTime NISTTIME::datetime(void) {

  String stringtime = get_date();

  DateTime _datetime;

  date_string.hour = String(stringtime.substring(9,11).toInt() + _utc_offset); // adding the utc offset here...
  date_string.minute = stringtime.substring(12,14);
  date_string.day = stringtime.substring(6,8);
  date_string.month = stringtime.substring(3,5);
  date_string.year = stringtime.substring(0,2);

  _datetime.hour = date_string.hour.toInt(); // adding the utc offset here...
  _datetime.minute = date_string.minute.toInt();
  _datetime.day = date_string.day.toInt();
  _datetime.month = date_string.month.toInt();
  _datetime.year = date_string.year.toInt();

  return _datetime;

}

// return String of format "16-04-23 20:30:09 50 0 0 648.6 UTC(NIST) *" (at least for host "time.nist.gov") from the internetzzzz
String NISTTIME::get_date(void) {

  // Use WiFiClient class to create TCP connection
  WiFiClient client;
  if (!client.connect(_host, _http_port)) {
    Serial.println("connection failed");
    return "00-00-00  00:00"; // String signaling connection error
  }

  // send the request to the server
  client.print("HEAD / HTTP/1.1\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; ESP8266 Arduino;)\r\n\r\n");
  delay(100);

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()) {

    String line = client.readStringUntil('\r');

    if (line.indexOf("Date") != -1) {
      Serial.print("=====>");
    } else {
      // date starts at pos 7 ends at 24
      return line.substring(7,24);
    }
  }

}
