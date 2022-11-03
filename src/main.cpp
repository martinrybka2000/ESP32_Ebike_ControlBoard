#include <Arduino.h>

#include "ProgramData.h"
#include "DataSmoother.h"
#include "TemperatureReader.h"

// main program data
ProgramData programData;
DataSmoother dataSmoother(10);
TemperatureReader temperatureReader();

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello world");
  programData.BatteryTemperature = 22.4;

  // data smoother test
  for (size_t i = 0; i < 20; i++)
  {
    dataSmoother.AddData(i);
  }
  dataSmoother.PrintData();
}

void loop()
{
  Serial.printf("prgoramData.BatterTem = %f\n", programData.BatteryTemperature);
  Serial.printf("DataSmoother result = %f\n", dataSmoother.GetData());
  delay(1000);
}