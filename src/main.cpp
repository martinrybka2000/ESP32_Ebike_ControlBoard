#include <Arduino.h>

#include "ProgramData.h"
#include "DataSmoother.h"
#include "Oled.h"
#include "ThrottleReader.h"

// main program data
//ProgramData programData;
Oled oled;
ThrottleReader Throttle;

// const
uint8_t THROTTLE_PIN = A0; // analog input from throttle
#define TIMESTAMP 5000 // how often change displayed data

void setup()
{
  //Serial.begin(115200);
  Serial.begin(9600);
  Serial.println("Hello world");
  oled.setup(); // one time oled setup
  
  //programData.BatteryTemperature = 22.4;

}
void loop()
{
  String names[] = {"Battery", "Speed", "Motor"}; // display name of the data
  float data[] = {Throttle.getThrottleInVoltage(THROTTLE_PIN), Throttle.getThrottleInProcent(THROTTLE_PIN), 100}; // data to display
  Oled::valueUnit units[] = {Oled::VOLT, Oled::SPEED, Oled::TEMPERATURE}; // unit of the data chosen from enum inside oled class
  
  // oled tests
  oled.show(names, data, units, TIMESTAMP); // method to display data on oled
  //Serial.println(oled.bug); // FOR DEBUGING
}
