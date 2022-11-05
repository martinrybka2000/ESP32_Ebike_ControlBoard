#ifndef THROTTLE_READER
#define THROTTLE_READER

class ThrottleReader {
    public:
        ThrottleReader(){}
        float getThrottleInVoltage(uint8_t pin){ return analogRead(pin) * (5.0 / 1023.0); }
        float getThrottleInProcent(uint8_t pin){ return analogRead(pin) * (5.0 / 1023.0) * 0.2 * 100; }
};

#endif
