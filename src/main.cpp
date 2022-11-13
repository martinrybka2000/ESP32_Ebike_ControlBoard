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
uint8_t THROTTLE_PIN = A0;
#define TIMESTAMP 2000

void setup()
{
  Serial.begin(115200);
  //Serial.begin(9600);
  Serial.println("Hello world");
  oled.setup();
  
  //programData.BatteryTemperature = 22.4;

}
void loop()
{
  String names[] = {"Battery", "Speed"};
  float data[] = {Throttle.getThrottleInVoltage(THROTTLE_PIN), Throttle.getThrottleInProcent(THROTTLE_PIN)};
  Oled::valueUnit units[] = {Oled::VOLT, Oled::SPEED};
  
  //oled tests
  oled.show(names, data, units, TIMESTAMP);
}
