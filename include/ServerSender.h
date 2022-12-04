#ifndef SERVER_SENDER_H_
#define SERVER_SENDER_H_

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <AsyncHTTPRequest_Generic.hpp>

#include "ProgramData.h"

// Level of debug informaion from 0-4
#define ASYNC_HTTP_DEBUG_PORT     Serial
#define _ASYNC_HTTP_LOGLEVEL_     4

#define ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN_TARGET      "AsyncHTTPRequest_Generic v1.10.2"
#define ASYNC_HTTP_REQUEST_GENERIC_VERSION_MIN             1010002

// Seconds for timeout, default is 3s
#define DEFAULT_RX_TIMEOUT           10

// Uncomment for certain HTTP site to optimize
//#define NOT_SEND_HEADER_AFTER_CONNECTED        true

// function that is run when the sender gets back a messege
static void requestCB(void *optParm, AsyncHTTPRequest *request, int readyState)
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

class ServerSender
{
private:
    AsyncHTTPRequest request;           // async http client 
    StaticJsonDocument<256> doc;        // json objec for easy json file making

    bool requestOpenResult;             // olds information if the request has a result
    String body;                        // body of json file to send

    String backend_url;                 // url to send the request

    unsigned long lastSendRequest;      // for measuring time

    void sendAsyncRequest(ProgramData &programData);    // sending request logic

public:
    ServerSender();
    ~ServerSender();

    void setup(String backend_url){
        request.setDebug(false);                // turing off debug
        request.onReadyStateChange(requestCB);  // what function should be run if the request has a back messege

        this->backend_url = backend_url;        // setting the backend url

        lastSendRequest = millis();             // initial time read
    }

    void SendRequestEvent(ProgramData &programData, unsigned long interval_ms = 2000)
    {
        if (programData.ConnectedToHotSpot && (millis() - lastSendRequest >= interval_ms))   // if the esp is connected to wifi and set time interval has passed
        {
            this->sendAsyncRequest(programData);                                             // send the request
            lastSendRequest = millis();                                                      // read time for interval calculation
        }
    }
};




#endif