#include <Arduino.h>

#include "TemperatureReader.h"

TemperatureReader::TemperatureReader(int OneWirePin, ProgramData &programData)
{
    // initializing one wire comunication and the ds18b20 class
    if (OneWirePin == -1)
        this->oneWire = new OneWire(ONE_WIRE_PIN_DEFAULT);
    else
        this->oneWire = new OneWire(OneWirePin);

    sensor = new DallasTemperature(this->oneWire);
    sensor->begin();
    if (!sensor->getAddress(tempDeviceAddress, 0))
    {
        writeErrorFlag(programData, ERROR_TEM_BROKEN);
    }

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

void TemperatureReader::ReadTemperature(ProgramData &programData, unsigned long interval_ms)
{
    if (millis() - lastTempRequest >= interval_ms)
    {
        programData.BatteryTemperature = sensor->getTempCByIndex(0);
        if (programData.BatteryTemperature == DEVICE_DISCONNECTED_C)
        {
            writeErrorFlag(programData, ERROR_TEM_BROKEN);
        }
        sensor->requestTemperatures();

        lastTempRequest = millis();

        // test
        Serial.print("Tem: ");
        Serial.println(String(programData.BatteryTemperature));
    }
}