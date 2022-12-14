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
#include "ServerSender.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <AsyncHTTPRequest_Generic.h>             // https://github.com/khoih-prog/AsyncHTTPRequest_Generic


#define OLED_SHOW_INTERVAL_MS       300  // how often to show neewest data
#define OLED_SWITH_INTERVAL_MS      2000 // how often change displayed data, should be bigger that OLED_SHOW_INTERVAL_MS
#define THROTTLE_TIME_INTERVAL_MS   50   // how often o read the throotle value
#define TEM_SENSOR_TIME_INTERVAL_MS 1000 // how often to read the temperature value
#define LED_BLINK_TIME_INTERVAL_MS  2000 // how often the led should blink
#define VESC_DATA_READ_INTERVAL_MS  500  // how often should i read data from vesc
#define BESC_WRITE_CURRNET_INTERVL_MS 50 // how often should the current value be written to vesc
#define SERVER_POST_DATA_INTERVAL_MS 2000 // how often should the data be send to the server

#define THROTTLE_PIN    A6  // analog input from throttle
#define TEM_SENSOR_PIN  23  // pin for temperature sensor one wire data
#define LED_PIN         19  // pin for the onboard led
#define VESC_SERIAL     Serial1

#define CNT_WIFI_CONNECTION 20 // hom many times the esp trys to coonect to wifi f.e. 20*100ms = 2s

// wifi hotspot password for testing
const char* ssid = "Redmi";
const char* password = "87654123";
// const char* ssid = "Dlink_2_4";
// const char* password = "Ceqlk123";

String backend_url("http://35.158.118.191/"); //backend ip

// Initializing the objects
ProgramData programData;

Oled oled;
ThrottleReader throttle;
TemperatureReader temperatureReader(TEM_SENSOR_PIN, programData);
VESCComunicator vescComunicator;
LEDBlinker ledBlinker;
ServerSender serverSender;

String names[] = {"Battery", "Speed", "Battery", "Test1", "Test2", "Test3", "Test4", "Test5"};                       // display name of the data
Oled::valueUnit units[] = {Oled::VOLT, Oled::SPEED, Oled::TEMPERATURE, Oled::PROCENT, Oled::AMPER, Oled::PROCENT, Oled::AMPER, Oled::AMPER}; // unit of the data chosen from enum inside oled class

void setup()
{
  Serial.begin(115200);                            // serial communication for debuging

  oled.setup(sizeof(names)/ sizeof(names[0]));     // one time oled setup (needs arrey size)
  ledBlinker.setup(LED_PIN);                       // one time led setup

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
  if(programData.ConnectedToHotSpot){               // if connected to hotspot add rest of the backed url and setup the request sender
    Serial.print("Connected to WiFi with IP Address: ");
    Serial.println(WiFi.localIP());

    backend_url += "data/board/" + programData.macAddress + "/livedata";  // adding acaddres to backend data
    
    serverSender.setup(backend_url);
    Serial.println(backend_url);
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

  // writing current to vesc
  vescComunicator.writeThrootleValue(programData, VESC_DATA_READ_INTERVAL_MS);

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

  // sending post request
  serverSender.SendRequestEvent(programData, SERVER_POST_DATA_INTERVAL_MS);

  delay(1);   // one ms delay
}
