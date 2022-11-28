#include <Arduino.h>

#include "TemperatureReader.h"

TemperatureReader::TemperatureReader(int OneWirePin, ProgramData &programData)
{
    // initializing one wire comunication and the ds18b20 class
    if (OneWirePin == -1)                                               // if pin not selected choose default pin
        this->oneWire = new OneWire(ONE_WIRE_PIN_DEFAULT);
    else
        this->oneWire = new OneWire(OneWirePin);

    sensor = new DallasTemperature(this->oneWire);                      // creating a new sensor class and starting the sensor
    sensor->begin();
    if (!sensor->getAddress(tempDeviceAddress, 0))                      // if sensor not connected write an error flag
    {
        writeErrorFlag(programData, ERROR_TEM_BROKEN);
    }

    sensor->setResolution(tempDeviceAddress, SENSOR_RESOLUTION);        // setting sensor resolution

    sensor->setWaitForConversion(false);                                // set the option that the esp does not stop when requesting temperature
    sensor->requestTemperatures();                                      // request the first temperature reading

    lastTempRequest = millis();                                         // reading actual prccesor time
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
    if (millis() - lastTempRequest >= interval_ms)                      // if set time interval has passed
    {
        programData.BatteryTemperature = sensor->getTempCByIndex(0);    // reading the temperature
        if (programData.BatteryTemperature == DEVICE_DISCONNECTED_C)    // if device disconected write an error flag
        {
            writeErrorFlag(programData, ERROR_TEM_BROKEN);
        }
        sensor->requestTemperatures();                                  // requesting tem read for the next time

        lastTempRequest = millis();                                     // reading the last time for calculating interval

        // test
        // Serial.print("Tem: ");
        // Serial.println(String(programData.BatteryTemperature));
    }
}