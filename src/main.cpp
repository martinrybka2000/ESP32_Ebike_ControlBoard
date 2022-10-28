#include <Arduino.h>

#include "ProgramData.h"

// main program data
ProgramData programData;

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello world");
  programData.BatteryTemperature = 22.4;
}

void loop()
{
  Serial.printf("prgoramData.BatterTem = %f\n", programData.BatteryTemperature);
  delay(1000);
}