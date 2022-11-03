#include <Arduino.h>

#include "TemperatureReader.h"

TemperatureReader::TemperatureReader(int OneWirePin)
{
    // initializing one wire comunication and the ds18b20 class
    if (OneWirePin == -1)
        this->oneWire = new OneWire(ONE_WIRE_PIN_DEFAULT);
    else
        this->oneWire = new OneWire(OneWirePin);

    sensor = new DallasTemperature(this->oneWire);
    sensor->begin();
    sensor->getAddress(tempDeviceAddress, 0);
    // TODO if addres not found set an error flag
    sensor->setResolution(tempDeviceAddress, SENSOR_RESOLUTION);

    sensor->setWaitForConversion(false);
    sensor->requestTemperatures();

    // reading actual prccesor time
    lastTempRequest = millis();
}

TemperatureReader::~TemperatureReader()
{
    if (oneWire != nullptr)
        delete oneWire;
    if (sensor != nullptr)
        delete sensor;
}

void TemperatureReader::ReadTemperature(double &batteryTem, unsigned long interval_ms)
{
    if (millis() - lastTempRequest >= interval_ms)
    {
        batteryTem = sensor->getTempCByIndex(0);
        if (batteryTem == DEVICE_DISCONNECTED_C)
        {
            // write error flag
        }
        sensor->requestTemperatures();

        lastTempRequest = millis();
    }
}