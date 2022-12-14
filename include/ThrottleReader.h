#ifndef THROTTLE_READER
#define THROTTLE_READER

#define SMOOTH_BUFFER_SIZE 10    // size of the smoothing algorythm buffer
#define REFERENCE_VOLTAGE  3.3   // reference voltage of esp32
#define ADC_MAX_VALUE      4096.0// maximum value that we can read from adc
#define MIN_ERROR_VALUE    0.684 // min voltage that can be read from potencjometr
//#define MAX_ERROR_VALUE    3.0   // max voltage that can be read from potencjomter
#define CORRECTION         1.9 // value muliplayer used to corret the offset of the voltage

DataSmoother dataSmoother(SMOOTH_BUFFER_SIZE); // fpr smoothing input data

class ThrottleReader {

   unsigned long lastTempRequest; // how often data should be added

    public:
        ThrottleReader(){} //empty constructor

        void getThrootleToProgramData(ProgramData &programData, uint8_t adc_pin ,unsigned long interval_ms = 100){

            if (millis() - lastTempRequest >= interval_ms){                                                       // if set time interval has passed

                dataSmoother.AddData(analogRead(adc_pin));           // reading adc value into the data smoother algoythm

                // this solution is only used in this particural situation, normaly initialization should be used to determine low and max value
                if((((dataSmoother.GetData() / ADC_MAX_VALUE) - (MIN_ERROR_VALUE / REFERENCE_VOLTAGE)) * 100 ) * CORRECTION <= 0) { // check if there is any value below 0
                    programData.ThrottleValueInPercent = 0; // getting around below 0 values
                }
                else {
                    programData.ThrottleValueInPercent = (((dataSmoother.GetData() / ADC_MAX_VALUE) - (MIN_ERROR_VALUE / REFERENCE_VOLTAGE)) * 100 ) * CORRECTION; // removing the minimum value to get 0 %
                }

                // this solution is only used in this particural situation, normaly initialization should be used to determine low and max value
                if((((dataSmoother.GetData() * REFERENCE_VOLTAGE) / ADC_MAX_VALUE) - (MIN_ERROR_VALUE)) * CORRECTION <= 0) {
                    programData.ThrottleValueInVoltage = 0; // getting around below 0 values
                }
                else {
                    programData.ThrottleValueInVoltage = (((dataSmoother.GetData() * REFERENCE_VOLTAGE) / ADC_MAX_VALUE) - (MIN_ERROR_VALUE)) * CORRECTION;       
                }
                
                lastTempRequest = millis();                                                                        // reading the last time for calculating interval
            }
        }

};

#endif
