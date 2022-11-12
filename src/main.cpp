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
#define TIMESTAMP 1000

void setup()
{
  //Serial.begin(115200);
  Serial.begin(9600);
  Serial.println("Hello world");
  oled.setup();
  
  //programData.BatteryTemperature = 22.4;

}

void loop()
{
  //oled tests
  oled.show({"Battery", "Speed"}, {10, Throttle.getThrottleInProcent(THROTTLE_PIN)}, {Oled::TEMPERATURE, Oled::SPEED}, TIMESTAMP);
  //Serial.println(Throttle.getThrottleInProcent(THROTTLE_PIN));
  delay(50);
}
