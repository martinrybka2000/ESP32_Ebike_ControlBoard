#include <Arduino.h>

// #include "ProgramData.h"
// #include "DataSmoother.h"
#include "Oled.h"

// main program data
// ProgramData programData;
// DataSmoother dataSmoother(10);
Oled oled;

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello world");

  // oled tests
  oled.setup();
  
  // programData.BatteryTemperature = 22.4;

  // data smoother test
  // for (size_t i = 0; i < 20; i++)
  // {
  //   dataSmoother.AddData(i);
  // }
  // dataSmoother.PrintData();
}

void loop()
{
  // Serial.printf("prgoramData.BatterTem = %f\n", programData.BatteryTemperature);
  // Serial.printf("DataSmoother result = %f\n", dataSmoother.GetData());
  // delay(1000);

  // oled tests
  oled.show("Battery", 40.0, Oled::valueUnit::TEMPERATURE, 2000);
  oled.show("Speed", 25.5, Oled::valueUnit::SPEED, 2000);
}