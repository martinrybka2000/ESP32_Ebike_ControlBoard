#include <Arduino.h>

#include "ProgramData.h"
#include "DataSmoother.h"
#include "Oled.h"
#include "ThrottleReader.h"
#include "TemperatureReader.h"
#include "LEDBlinker.h"

#define OLED_SHOW_INTERVAL_MS       300  // how often to show neewest data
#define OLED_SWITH_INTERVAL_MS      5000 // how often change displayed data, should be bigger that OLED_SHOW_INTERVAL_MS
#define THROTTLE_TIME_INTERVAL_MS   50   // how often o read the throotle value
#define TEM_SENSOR_TIME_INTERVAL_MS 1000 // how often to read the temperature value
#define LED_BLINK_TIME_INTERVAL_MS  2000 // how often the led should blink

#define THROTTLE_PIN    A6  // analog input from throttle
#define TEM_SENSOR_PIN  23  // pin for temperature sensor one wire data
#define LED_PIN         19  // pin for the onboard led

// Initializing the objects
ProgramData programData;
Oled oled;
ThrottleReader throttle;
TemperatureReader temperatureReader(TEM_SENSOR_PIN, programData);
LEDBlinker ledBlinker;

String names[] = {"Battery", "Speed", "Battery"};                       // display name of the data
Oled::valueUnit units[] = {Oled::VOLT, Oled::SPEED, Oled::TEMPERATURE}; // unit of the data chosen from enum inside oled class

void setup()
{
  Serial.begin(115200);           // serial communication for debuging
  Serial.println("Hello world");

  oled.setup();                   // one time oled setup
  ledBlinker.setup(LED_PIN);      // one time led setup
}

void loop()
{
  throttle.getThrootleToProgramData(programData, THROTTLE_PIN, THROTTLE_TIME_INTERVAL_MS);

  // oled tests
  float data[] = {programData.ThrottleValueInVoltage, programData.ThrottleValueInPercent, programData.BatteryTemperature}; // data to display
  oled.show(names, data, units, OLED_SWITH_INTERVAL_MS, OLED_SHOW_INTERVAL_MS); // method to display data on oled
  //Serial.println(oled.bug); // FOR DEBUGING

  temperatureReader.ReadTemperature(programData, TEM_SENSOR_TIME_INTERVAL_MS);

  ledBlinker.Blink(LED_BLINK_TIME_INTERVAL_MS);

  delay(1);   // one ms delay
}

