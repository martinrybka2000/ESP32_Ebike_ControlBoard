#include <Arduino.h>
#include "ProgramData.h"

// functions that write, reset and read error flags

void writeErrorFlag(ProgramData &programData, uint16_t errorFlag)
{
    programData.ErrorFlags |= (1 << errorFlag);
}

void resetErrorFlag(ProgramData &programData, uint16_t errorFlag)
{
    programData.ErrorFlags &= ~(1 << errorFlag);
}

bool readErrorFlag(ProgramData &programData, uint16_t errorFlag)
{
    return programData.ErrorFlags & (1 << errorFlag);
}