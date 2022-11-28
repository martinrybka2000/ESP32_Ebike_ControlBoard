#ifndef TEM_READER_H
#define TEM_READER_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "ProgramData.h"

#define ONE_WIRE_PIN_DEFAULT 2      // deafault pin to data wire
#define SENSOR_RESOLUTION 10        // bit resolution of the sensor

class TemperatureReader
{
private:
    OneWire *oneWire;
    DallasTemperature *sensor;

    unsigned long lastTempRequest;
    DeviceAddress tempDeviceAddress;

public:
    TemperatureReader(int OneWirePin, ProgramData &programData);
    ~TemperatureReader();

    void ReadTemperature(ProgramData &programData, unsigned long interval_ms = 2000);
};

#endif