#ifndef THROTTLE_READER
#define THROTTLE_READER

DataSmoother dataSmoother(10);

class ThrottleReader {

    public:
        ThrottleReader(){}

        float getThrottleInVoltage(uint8_t pin) { 
           dataSmoother.AddData(analogRead(pin) * (5.0 / 1023.0));
           return dataSmoother.GetData();
        }
        
        float getThrottleInProcent(uint8_t pin) {  
           dataSmoother.AddData(analogRead(pin) * (100.0 / 1023.0));
           return dataSmoother.GetData();
        }
};

#endif
