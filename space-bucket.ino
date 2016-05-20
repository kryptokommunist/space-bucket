#include "DHT.h" // temperature/humidity sensor
#include "ROT/ROT.h" // potentiometer: functions: .value(), returns int 0 to 1024
#include "FAN/FAN.h" // control computer fans: functions: .set_speed(int speed), speed has to be between 0 and 1023
#include "NISTTIME/NISTTIME.h" // get access to NIST time, Wifi needs to be connected for that! functions: .datetime() returns DateTime struct
#include "LIGHT/LIGHT.h" // functions: .set_schedule(int start_hour, int start_min, int end_hour, int end_min) expects values between 1 and 24!
#include <Wire.h>
#include "OLED/SSD1306.h" //
#include "OLED/SSD1306Ui.h"
#include <ESP8266WiFi.h> // wifi functionality
#define DHTPIN D4
#define DHTTYPE DHT22

#define ROTPIN A0 // pin connected to potentiometer
#define FANPIN D0 // pin connected to fan
#define LEDRELAISPIN D1 // pin connected to relais

// Pin definitions for I2C OLED Display
#define OLED_SDA    D2
#define OLED_SDC    D3
#define OLED_ADDR   0x3C

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
SSD1306   oled(OLED_ADDR, OLED_SDA, OLED_SDC);    // For I2C
SSD1306Ui ui( &oled );

float t; //Holds Temperature
float h; //Holds Humidity

float target_temp = 21; // target temperature
float range_temp = 1; // range which is acceptable
float max_temp = 29; // light will be switched off above that temp
#define MIN_FAN_SPEED 200 // minimum speed of fan (max = 1023)
#define MAX_FAN_SPEED 1023 // minimum speed of fan (max = 1023)

bool drawFrame1(SSD1306 *display, SSD1306UiState* state, int x, int y);
bool drawFrame2(SSD1306 *display, SSD1306UiState* state, int x, int y);
bool drawFrame3(SSD1306 *display, SSD1306UiState* state, int x, int y);
bool msOverlay(SSD1306 *display, SSD1306UiState* state);
// this array keeps function pointers to all frames
// how many frames are there?
int frameCount = 3;
// frames are the single views that slide from right to left
bool (*frames[])(SSD1306 *display, SSD1306UiState* state, int x, int y) = {drawFrame1, drawFrame2, drawFrame3};

bool (*overlays[])(SSD1306 *display, SSD1306UiState* state)             = { msOverlay };
int overlaysCount = 1;

void setup() {
   ui.setTargetFPS(30);
   ui.setIndicatorPosition(BOTTOM);
   ui.setIndicatorDirection(LEFT_RIGHT);
   ui.setFrameAnimation(SLIDE_LEFT);
   ui.setFrames(frames, frameCount);
   ui.setOverlays(overlays, overlaysCount);
   ui.init();
   oled.flipScreenVertically();


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

  light.set_schedule(8,30,22,40); //light control setup
  fan.set_speed(MIN_FAN_SPEED); //initial fan speed

}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  ui.update();

  DateTime date_time = nist.datetime();

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
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //---------------- code after this only executed if tempsensor works! ----------

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
  
  Serial.println(fan.speed);

  if(t < max_temp) {
    light.update(date_time);
  } else {
    light.off();
  }

}


bool drawFrame1(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  display->setFont(ArialMT_Plain_24);

  // The coordinates define the center of the text
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 22, "Speed: " + String(fan.speed));

  return false;
}

bool drawFrame2(SSD1306 *display, SSD1306UiState* state, int x, int y) { 
  // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  display->setTextAlignment(TEXT_ALIGN_CENTER);

  display->setFont(ArialMT_Plain_16);
  display->drawString(64 + x, 36 + y, nist.date_string.day + "." + nist.date_string.month + "." + nist.date_string.year);

  display->setFont(ArialMT_Plain_24);
  display->drawString(64 + x, 12 + y, nist.date_string.hour + ":" + nist.date_string.minute);

  return false;
}

bool drawFrame3(SSD1306 *display, SSD1306UiState* state, int x, int y) { 
  // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  display->setTextAlignment(TEXT_ALIGN_CENTER);

  display->setFont(ArialMT_Plain_24);
  display->drawString(64 + x, 16 + y, "krypt0gr0w");

  return false;
}

bool msOverlay(SSD1306 *display, SSD1306UiState* state) {
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(128, 0, "H: " + String(h) + "%  T: " + String(t) + "*C" );
  return true;
}
