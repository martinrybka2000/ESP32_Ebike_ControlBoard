#ifndef LED_BLINKER_H_
#define LED_BLINKER_H_

#include <Arduino.h>

class LEDBlinker
{
private:
    uint8_t myLedPin;           // for rememebring with pin to blink
    unsigned long lastInterval; // for storing las time read
    boolean toggle;             // value for togling the pin state

public:
    LEDBlinker(/* args */);
    ~LEDBlinker();

    void setup(uint8_t led_pin)
    {
        toggle = HIGH;
        myLedPin = led_pin;
        pinMode(led_pin, OUTPUT);   // setting the pin as an output

        lastInterval = millis();    // init time read
    }

    void Blink(unsigned long interval_ms)
    {
        if (millis() - lastInterval >= interval_ms) // current time - last time code was done >= how often refresh display
        {
            toggle ^= HIGH;               // switching the state
            digitalWrite(myLedPin, toggle); // writing the state to pin
            lastInterval = millis();        // update wait time            
        }
    }
};

LEDBlinker::LEDBlinker(/* args */)
{
}

LEDBlinker::~LEDBlinker()
{
}

#endif