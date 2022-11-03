#ifndef PROGRAM_DATA_H
#define PROGRAM_DATA_H

#include "VescUart.h"

// error flags
#define ERROR_OLED_BROKEN 0
#define ERROR_TEM_BROKEN 1
#define ERROR_FLAG_MAX 15
// etc

// Structure for storing all actual data
typedef struct ProgramDataStruct
{
    uint16_t ErrorFlags = 0;
    double ThrottleValue = 0;
    double BatteryTemperature = 0;
    uint32_t EspSerialNumber;
    VescUart::dataPackage VescData = {0};
    bool ConnectedToHotSpot = false;
} ProgramData;

void writeErrorFlag(ProgramData &programData, uint16_t errorFlag);
void resetErrorFlag(ProgramData &programData, uint16_t errorFlag);
bool readErrorFlag(ProgramData &programData, uint16_t errorFlag);

#endif