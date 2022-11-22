#include <Arduino.h>

#include "ProgramData.h"
#include "DataSmoother.h"
#include "Oled.h"
#include "ThrottleReader.h"
#include "TemperatureReader.h"

#define TIMESTAMP 5000 // how often change displayed data
#define THROTTLE_PIN A0 // analog input from throttle

ProgramData programData;
Oled oled;
ThrottleReader Throttle;
TemperatureReader temperatureReader(23, programData);

String names[] = {"Battery", "Speed", "Motor"}; // display name of the data
Oled::valueUnit units[] = {Oled::VOLT, Oled::SPEED, Oled::TEMPERATURE}; // unit of the data chosen from enum inside oled class

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello world");
  oled.setup(); // one time oled setup
  
}
void loop()
{
  float data[] = {Throttle.getThrottleInVoltage(THROTTLE_PIN), Throttle.getThrottleInProcent(THROTTLE_PIN), 100}; // data to display
  
  // oled tests
  oled.show(names, data, units, TIMESTAMP); // method to display data on oled
  //Serial.println(oled.bug); // FOR DEBUGING

  temperatureReader.ReadTemperature(programData, 1000);
  delay(1);
}

