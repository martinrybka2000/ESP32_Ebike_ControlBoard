#ifndef THROTTLE_READER
#define THROTTLE_READER

#define SMOOTH_BUFFER_SIZE 10    // size of the smoothing algorythm buffer
#define REFERENCE_VOLTAGE  3.3   // reference voltage of esp32
#define ADC_MAX_VALUE      4096.0// maximum value that we can read from adc

DataSmoother dataSmoother(SMOOTH_BUFFER_SIZE);

class ThrottleReader {

   unsigned long lastTempRequest;

    public:
        ThrottleReader(){}

        void getThrootleToProgramData(ProgramData &programData, uint8_t adc_pin ,unsigned long interval_ms = 100){

            if (millis() - lastTempRequest >= interval_ms){                                                       // if set time interval has passed

               dataSmoother.AddData(analogRead(adc_pin));                                                         // reading adc value into the data smoother algoythm
               programData.ThrottleValueInPercent = dataSmoother.GetData() * (100.0 / ADC_MAX_VALUE);             // saving the values to program data
               programData.ThrottleValueInVoltage = dataSmoother.GetData()  * (REFERENCE_VOLTAGE / ADC_MAX_VALUE);       

               lastTempRequest = millis();                                                                        // reading the last time for calculating interval
            }
        }

};

#endif
