#ifndef PROGRAM_DATA_H
#define PROGRAM_DATA_H

#include "VescUart.h"

// error flags
#define ERROR_OLED_BROKEN 0     // error if the comunication with oled is broken
#define ERROR_TEM_BROKEN 1      // error if the temperature sensor does not respond
#define ERROR_VESC_BROKEN 2     // error if the vesc does not respond 
#define ERROR_FLAG_MAX 15       // maximum value of a flag
// etc

// Structure for storing all actual data
typedef struct ProgramDataStruct
{
    uint16_t ErrorFlags = 0;
    float ThrottleValueInPercent = 0;
    float ThrottleValueInVoltage = 0;
    float BatteryTemperature = 0;
    uint32_t EspSerialNumber;
    VescUart::dataPackage VescData = {0};
    bool ConnectedToHotSpot = false;
} ProgramData;

void writeErrorFlag(ProgramData &programData, uint16_t errorFlag);
void resetErrorFlag(ProgramData &programData, uint16_t errorFlag);
bool readErrorFlag(ProgramData &programData, uint16_t errorFlag);

#endif