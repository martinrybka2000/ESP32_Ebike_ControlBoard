#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "ProgramData.h"
#include "DataSmoother.h"
#include "Oled.h"
#include "ThrottleReader.h"
#include "TemperatureReader.h"
#include "LEDBlinker.h"
#include "VESCComunicator.h"

#if !( defined(ESP8266) ||  defined(ESP32) )
  #error This code is intended to run on the ESP8266 or ESP32 platform! Please check your Tools->Board setting.
#endif

// Level from 0-4
#define ASYNC_HTTP_DEBUG_PORT     Serial
#define _ASYNC_HTTP_LOGLEVEL_     4

#define ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN_TARGET      "AsyncHTTPRequest_Generic v1.10.2"
#define ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN             1010002

// Seconds for timeout, default is 3s
#define DEFAULT_RX_TIMEOUT           10

// Uncomment for certain HTTP site to optimize
//#define NOT_SEND_HEADER_AFTER_CONNECTED        true

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <AsyncHTTPRequest_Generic.h>             // https://github.com/khoih-prog/AsyncHTTPRequest_Generic


#define OLED_SHOW_INTERVAL_MS       300  // how often to show neewest data
#define OLED_SWITH_INTERVAL_MS      5000 // how often change displayed data, should be bigger that OLED_SHOW_INTERVAL_MS
#define THROTTLE_TIME_INTERVAL_MS   50   // how often o read the throotle value
#define TEM_SENSOR_TIME_INTERVAL_MS 1000 // how often to read the temperature value
#define LED_BLINK_TIME_INTERVAL_MS  2000 // how often the led should blink
#define VESC_DATA_READ_INTERVAL_MS  500  // how often should i read data from vesc

#define THROTTLE_PIN    A6  // analog input from throttle
#define TEM_SENSOR_PIN  23  // pin for temperature sensor one wire data
#define LED_PIN         19  // pin for the onboard led
#define VESC_SERIAL     Serial1
#define LED_ONBOARD     2

#define CNT_WIFI_CONNECTION 20 // hom many times the esp trys to coonect to wifi f.e. 20*100ms = 2s

// wifi hotspot password for testing
// const char* ssid = "Redmi";
// const char* password = "87654123";
const char* ssid = "Dlink_2_4";
const char* password = "Ceqlk123";

String backend_url("http://35.158.118.191/");

// String backend_url("http://fad18710-c488-4194-9f3e-8c935b8d4c04.mock.pstmn.io/");

// Initializing the objects
ProgramData programData;

Oled oled;
ThrottleReader throttle;
TemperatureReader temperatureReader(TEM_SENSOR_PIN, programData);
VESCComunicator vescComunicator;
LEDBlinker ledBlinker;
LEDBlinker ledBlinkerTest;

String names[] = {"Battery", "Speed", "Battery", "Test1", "Test2", "Test3", "Test4", "Test5"};                       // display name of the data
Oled::valueUnit units[] = {Oled::VOLT, Oled::SPEED, Oled::TEMPERATURE, Oled::PROCENT, Oled::AMPER, Oled::PROCENT, Oled::AMPER, Oled::AMPER}; // unit of the data chosen from enum inside oled class

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

AsyncHTTPRequest request;
StaticJsonDocument<256> doc;

void sendRequest()
{
  static bool requestOpenResult;

  if (request.readyState() == readyStateUnsent || request.readyState() == readyStateDone)
  {
    requestOpenResult = request.open("POST", backend_url.c_str());

    if (requestOpenResult)
    {
      // Only send() if open() returns true, or crash
      // request.send();
      String body;
      doc.clear();
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

      serializeJson(doc, body);

      request.send(body);
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

void requestCB(void *optParm, AsyncHTTPRequest *request, int readyState)
{
  (void) optParm;

  if (readyState == readyStateDone)
  {
    AHTTP_LOGDEBUG(F("\n**************************************"));
    AHTTP_LOGDEBUG1(F("Response Code = "), request->responseHTTPString());

    if (request->responseHTTPcode() == 200)
    {
      Serial.println(F("\n**************************************"));
      Serial.println(request->responseText());
      Serial.println(F("**************************************"));
    }
  }
}


void setup()
{
  Serial.begin(115200);                            // serial communication for debuging

  oled.setup(sizeof(names)/ sizeof(names[0]));     // one time oled setup (needs arrey size)
  ledBlinker.setup(LED_PIN);                       // one time led setup
  ledBlinkerTest.setup(LED_ONBOARD);

  vescComunicator.setup(&VESC_SERIAL);             // setting vesc comunicator serial

  // ************** WIFI staff *******************************
  programData.macAddress = WiFi.macAddress();                   // reading my mac address XX:XX:XX:XX:XX:XX
  Serial.println("MY mac Address: " + programData.macAddress);  // displaying the mac addres in console
  programData.macAddress.replace(":", "");                      // removing ':' from the mac address string 

  WiFi.begin(ssid, password);                       // starting connections

  Serial.println("Connecting");                     // trying to coonec to wifi for (CNT * 100ms)ms
  for (size_t i = 0; i < CNT_WIFI_CONNECTION; i++)
  {
    if(WiFi.status() == WL_CONNECTED){              // if connected set status and break
      programData.ConnectedToHotSpot = true;
      break;
    }
    delay(100);                                     // 100ms delay betwwent connection attemps
    Serial.print(".");
  }
  if(programData.ConnectedToHotSpot){
    Serial.print("Connected to WiFi with IP Address: ");
    Serial.println(WiFi.localIP());

    backend_url += "data/board/" + programData.macAddress + "/livedata";  // adding acaddres to backend data
    
    Serial.println(backend_url);

    request.setDebug(false);
    request.onReadyStateChange(requestCB);

    sendRequest();
  }
  else{                                             // if not connected to AP turn off the wifi
    Serial.println("NO connection turning off wifi");
    WiFi.disconnect(true);
  }
  // ***********************************************************
}

void loop()
{
  // reading the throotle value
  throttle.getThrootleToProgramData(programData, THROTTLE_PIN, THROTTLE_TIME_INTERVAL_MS);

  // reading vesc values
  vescComunicator.getDataToProgramData(programData, VESC_DATA_READ_INTERVAL_MS);

  // reading the temp value
  temperatureReader.ReadTemperatureToProgramData(programData, TEM_SENSOR_TIME_INTERVAL_MS);

  // oled tests
  float data[] = {programData.ThrottleValueInVoltage, programData.ThrottleValueInPercent, programData.BatteryTemperature, 69, 100, 15, 20, 35}; // data to display

  oled.show(names, data, units, OLED_SWITH_INTERVAL_MS, OLED_SHOW_INTERVAL_MS); // method to display data on oled
  //Serial.println(oled.bug); // FOR DEBUGING

  // blinking the led
  ledBlinker.Blink(LED_BLINK_TIME_INTERVAL_MS);
  ledBlinkerTest.Blink(100);

  // sending post request
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    unsigned long timenow = micros();
    sendRequest();
    lastTime = millis();

    unsigned long elapsedTime = micros() - timenow;
    Serial.printf("Elapsed time in microsecods: %lu\n", elapsedTime);
  }

  delay(1);   // one ms delay
}
