#include "DHT.h" // temperature/humidity sensor
#include "ROT.h" // potentiometer: functions: .value(), returns int 0 to 1024
#include "FAN.h" // control computer fans: functions: .set_speed(int speed), speed has to be between 0 and 1023
#include "NISTTIME.h" // get access to NIST time, Wifi needs to be connected for that! functions: .datetime() returns DateTime struct
#include "LIGHT.h" // functions: .set_schedule(int start_hour, int start_min, int end_hour, int end_min) expects values between 1 and 24!
#include <ESP8266WiFi.h> // wifi functionality
#define DHTPIN D4
#define DHTTYPE DHT22

#define ROTPIN A0 // pin connected to potentiometer
#define FANPIN D0 // pin connected to fan
#define LEDRELAISPIN D1 // pin connected to relais

#define UTCOFFSET 2 // we are UTC+2
const char* NISTHOST = "time.nist.gov";
const char* SSID = "Mettigel24.de | Get off my LAN";
const char* PASSWORD = "N00bznet?NoWay!";

//initialize sensors and peripherals
ROT rot(ROTPIN);
DHT dht(DHTPIN, DHTTYPE);
FAN fan(FANPIN);
LIGHT light(LEDRELAISPIN);
NISTTIME nist(NISTHOST, UTCOFFSET);

float target_temp = 23; // target temperature
float range_temp = 1; // range which is acceptable
#define MIN_FAN_SPEED 200 // minimum speed of fan (max = 1023)
#define MAX_FAN_SPEED 1023 // minimum speed of fan (max = 1023)

void setup() {

  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(SSID, PASSWORD);
  delay(100);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  dht.begin(); // kick off the good old DHT-22

  light.set_schedule(8,30,19,45); //light control setup

}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  DateTime date_time = nist.datetime();

  light.update(date_time);

  Serial.println("");
  Serial.print("Hour: ");
  Serial.print(date_time.hour);
  Serial.print(":");
  Serial.print(date_time.minute);
  Serial.println("Year: ");
  Serial.print(date_time.day);
  Serial.print(".");
  Serial.print(date_time.month);
  Serial.print(".");
  Serial.print(date_time.year);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.println("");

  int delta =  t - target_temp;
  if(delta < 0 && delta < -range_temp  && fan.speed >= MIN_FAN_SPEED) {
      fan.set_speed(fan.speed + 10*(delta-range_temp));
  } else if (delta > 0 && delta > range_temp && fan.speed <= MAX_FAN_SPEED) {
      fan.set_speed(fan.speed - 10*(delta-range_temp));
  }

}
