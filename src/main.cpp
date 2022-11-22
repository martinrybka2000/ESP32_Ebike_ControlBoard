#include <Arduino.h>

#include "ProgramData.h"
#include "DataSmoother.h"
#include "TemperatureReader.h"

// main program data
ProgramData programData;
DataSmoother dataSmoother(10);
TemperatureReader temperatureReader(23, programData);

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello world");
  programData.BatteryTemperature = 22.4;

  Serial.println("Checking flags");

  writeErrorFlag(programData, ERROR_TEM_BROKEN);
  writeErrorFlag(programData, ERROR_OLED_BROKEN);
  writeErrorFlag(programData, ERROR_FLAG_MAX);
  resetErrorFlag(programData, ERROR_TEM_BROKEN);
  Serial.println(readErrorFlag(programData, ERROR_OLED_BROKEN));
  Serial.println(readErrorFlag(programData, ERROR_TEM_BROKEN));
  Serial.println(readErrorFlag(programData, ERROR_FLAG_MAX));

  Serial.print(programData.ErrorFlags, BIN);
  Serial.println();
}

void loop()
{
  temperatureReader.ReadTemperature(programData, 1000);
  delay(1);
}