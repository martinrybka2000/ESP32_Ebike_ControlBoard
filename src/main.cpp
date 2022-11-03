#include <Arduino.h>

#include "ProgramData.h"
#include "DataSmoother.h"
#include "TemperatureReader.h"

// main program data
ProgramData programData;
DataSmoother dataSmoother(10);
TemperatureReader temperatureReader(23);

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello world");
  programData.BatteryTemperature = 22.4;
}

void loop()
{
  temperatureReader.ReadTemperature(programData.BatteryTemperature, 1000);
  delay(1);
}