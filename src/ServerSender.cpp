#include "ServerSender.h"

ServerSender::ServerSender(/* args */)
{
}

ServerSender::~ServerSender()
{
}

void ServerSender::sendAsyncRequest(ProgramData &programData){

        if (request.readyState() == readyStateUnsent || request.readyState() == readyStateDone) // checking if the messege is back or was not send
        {
            requestOpenResult = request.open("POST", backend_url.c_str());  // trying to open a connection

            if (requestOpenResult)
            {
                // Only send() if open() returns true, or crash   
                // creating the json file structure        
                doc["board_mac"] = programData.macAddress;
                doc["time_stamp"] = "1939-09-01T10:00:00";
                doc["battery_temp"] = programData.BatteryTemperature;
                doc["throttle_value"] = programData.ThrottleValueInPercent;
                doc["motor_current"] = programData.VescData.avgMotorCurrent;
                doc["input_current"] = programData.VescData.avgInputCurrent;
                doc["duty_cycle_now"] = programData.VescData.dutyCycleNow;
                doc["rpm"] = programData.VescData.rpm;
                doc["input_voltage"] = programData.VescData.inpVoltage;
                doc["amp_hours"] = programData.VescData.ampHours;
                doc["amp_hours_charged"] = programData.VescData.ampHoursCharged;
                doc["watt_hours"] = programData.VescData.wattHours;
                doc["watt_hours_charged"] = programData.VescData.wattHoursCharged;
                doc["mosfet_temp"] = programData.VescData.tempMosfet;
                doc["motor_temp"] = programData.VescData.tempMotor;
                doc["error_code"] = programData.VescData.error;

                serializeJson(doc, body);   // writing the json text to a string

                request.send(body);         // sending the json string
            }
            else
            {
            Serial.println(F("Can't send bad request"));
            }
        }
        else
        {
            Serial.println(F("Can't send request"));
        }
}