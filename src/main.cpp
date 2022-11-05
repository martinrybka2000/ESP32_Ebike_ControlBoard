#include <Arduino.h>

#include "ProgramData.h"
#include "DataSmoother.h"
#include "Oled.h"
#include "ThrottleReader.h"

// main program data
// ProgramData programData;
// DataSmoother dataSmoother(10);
// Oled oled;
ThrottleReader Throttle;

uint8_t THROTTLE_PIN = A0;

void setup()
{
  //Serial.begin(115200);
  Serial.begin(9600);
  Serial.println("Hello world");


  // oled tests
  // oled.setup();
  
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
  // oled.show("Battery", 40.0, Oled::valueUnit::TEMPERATURE, 2000);
  // oled.show("Speed", 25.5, Oled::valueUnit::SPEED, 2000);

  Serial.println(Throttle.getThrottleInVoltage(THROTTLE_PIN));
  Serial.println(Throttle.getThrottleInProcent(THROTTLE_PIN));
  delay(3000);
}