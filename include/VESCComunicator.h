#ifndef VESCCOMUNICATOR_H_
#define VESCCOMUNICATOR_H_

#include <Arduino.h>
#include "VescUart.h"
#include "ProgramData.h"

class VESCComunicator
{
private:
    VescUart *vescUart;
    unsigned long lastDataRequest;
    unsigned long lastThrootleWrite;

public:
    VESCComunicator(/* args */);
    ~VESCComunicator();

    void setup(Stream *serialport)
    {
        vescUart = new VescUart(30);        // setting the timeout
        vescUart->setSerialPort(serialport); // setting uart vesc comunicator to serial 1
        vescUart->setDebugPort(&Serial);

        lastDataRequest = millis();         // initial time read

    }

    void getDataToProgramData(ProgramData &programData, unsigned long interval_ms = 1000)
    {
        if (millis() - lastDataRequest >= interval_ms)          // if set time interval has passed
        {
            if(vescUart->getVescValues()){                        // trying to read vesc data
                programData.VescData = vescUart->data;           // if succesful save it to program data
                resetErrorFlag(programData, ERROR_VESC_BROKEN); // reset the error flag
            }
            else
                writeErrorFlag(programData, ERROR_VESC_BROKEN); // else set error flag

            lastDataRequest = millis();                         // reading time 
            
            // debug
            printProgramData(programData);
        }

    }

    void writeThrootleValue(ProgramData &programData, unsigned long interval_ms = 1000)
    {
        if (millis() - lastThrootleWrite >= interval_ms)          // if set time interval has passed
        {
            if (programData.ThrottleValueInPercent >= 3.0)          // lowwer boundry 
                vescUart->setCurrent(vescUart->PercentToCurrent(programData.ThrottleValueInPercent)); // sending current value to vesc

            lastThrootleWrite = millis();                         // reading time 
        }
    }
};

VESCComunicator::VESCComunicator(/* args */)
{
}

VESCComunicator::~VESCComunicator()
{
}


#endif