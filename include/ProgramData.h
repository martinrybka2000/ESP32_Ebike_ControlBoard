#ifndef PROGRAM_DATA_H
#define PROGRAM_DATA_H

#include "VescUart.h"

#define OLED_COM_BROKEN 0
#define TEM_SENSOR_COM_BROKEN 1
// etc

// Structure for storing all actual data
typedef struct ProgramDataStruct
{
    uint8_t ErrorFlags = 0;
    double ThrottleValue = 0;
    double BatteryTemperature = 0;
    uint32_t EspSerialNumber;
    VescUart::dataPackage VescData = {0};
    bool ConnectedToHotSpot = false;
} ProgramData;

#endif