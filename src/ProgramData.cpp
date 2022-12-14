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

// function for printing program data probably takes a lot of time to execute should only be used in deebuging
void printProgramData(ProgramData &programData)
{
    Serial.println("***************************************");
    Serial.println("Program Data:");
    Serial.printf("Mac address : %s\n", programData.macAddress);
    Serial.printf("Battery temp: %f\n", programData.BatteryTemperature);
    Serial.printf("Thr value   : %f\n", programData.ThrottleValueInPercent);
    Serial.printf("The value   : %f\n", programData.ThrottleValueInVoltage);
    Serial.printf("Error flags : %d\n", programData.ErrorFlags);
    Serial.printf("Connec to AP: %d\n", programData.ConnectedToHotSpot);
    Serial.println("VESC data   :");
    Serial.printf("    Amp hours    : %f\n", programData.VescData.ampHours);
    Serial.printf("    Amp hours  Ch: %f\n", programData.VescData.ampHoursCharged);
    Serial.printf("    Watt hours   : %f\n", programData.VescData.wattHours);
    Serial.printf("    Watt hours Ch: %f\n", programData.VescData.wattHoursCharged);
    Serial.printf("    Batt current : %f\n", programData.VescData.avgInputCurrent);
    Serial.printf("    BLDC current : %f\n", programData.VescData.avgMotorCurrent);
    Serial.printf("    Batt Voltage : %f\n", programData.VescData.inpVoltage);
    Serial.printf("    Temp Mosfet  : %f\n", programData.VescData.tempMosfet);
    Serial.printf("    Temp BLDC    : %f\n", programData.VescData.tempMotor);
    Serial.printf("    RPM          : %f\n", programData.VescData.rpm);
    Serial.printf("    ID           : %d\n", programData.VescData.id);
    Serial.println("***************************************");
}