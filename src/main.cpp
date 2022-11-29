#include <Arduino.h>

#include "ProgramData.h"
#include "DataSmoother.h"
#include "Oled.h"
#include "ThrottleReader.h"
#include "TemperatureReader.h"
#include "LEDBlinker.h"
#include "VESCComunicator.h"

#define OLED_SHOW_INTERVAL_MS       300  // how often to show neewest data
#define OLED_SWITH_INTERVAL_MS      5000 // how often change displayed data, should be bigger that OLED_SHOW_INTERVAL_MS
#define THROTTLE_TIME_INTERVAL_MS   50   // how often o read the throotle value
#define TEM_SENSOR_TIME_INTERVAL_MS 1000 // how often to read the temperature value
#define LED_BLINK_TIME_INTERVAL_MS  2000 // how often the led should blink
#define VESC_DATA_READ_INTERVAL_MS  500  // how often should i read data from vesc

#define THROTTLE_PIN    A6  // analog input from throttle
#define TEM_SENSOR_PIN  23  // pin for temperature sensor one wire data
#define LED_PIN         19  // pin for the onboard led
#define VESC_SERIAL     Serial1
#define LED_ONBOARD     2 


// Initializing the objects
ProgramData programData;

Oled oled;
ThrottleReader throttle;
TemperatureReader temperatureReader(TEM_SENSOR_PIN, programData);
VESCComunicator vescComunicator;
LEDBlinker ledBlinker;
LEDBlinker ledBlinkerTest;

String names[] = {"Battery", "Speed", "Battery", "Test"};                       // display name of the data
Oled::valueUnit units[] = {Oled::VOLT, Oled::SPEED, Oled::TEMPERATURE, Oled::VOLT}; // unit of the data chosen from enum inside oled class

void setup()
{
  Serial.begin(115200);           // serial communication for debuging
  Serial.println("Hello world");

  oled.setup();                   // one time oled setup
  ledBlinker.setup(LED_PIN);      // one time led setup
  ledBlinkerTest.setup(LED_ONBOARD);

  vescComunicator.setup(&VESC_SERIAL);  // setting vesc comunicator serial
}

void loop()
{
  // reading the throotle value
  throttle.getThrootleToProgramData(programData, THROTTLE_PIN, THROTTLE_TIME_INTERVAL_MS);

  // reading vesc values
  vescComunicator.getDataToProgramData(programData, VESC_DATA_READ_INTERVAL_MS);

  // reading the temp value
  temperatureReader.ReadTemperatureToProgramData(programData, TEM_SENSOR_TIME_INTERVAL_MS);

  // oled tests
  float data[] = {programData.ThrottleValueInVoltage, programData.ThrottleValueInPercent, programData.BatteryTemperature, programData.VescData.rpm, 100}; // data to display
  oled.show(names, data, units, OLED_SWITH_INTERVAL_MS, OLED_SHOW_INTERVAL_MS); // method to display data on oled
  //Serial.println(oled.bug); // FOR DEBUGING

  // blinking the led
  ledBlinker.Blink(LED_BLINK_TIME_INTERVAL_MS);
  ledBlinkerTest.Blink(100);

  delay(1);   // one ms delay
}

