#ifndef DATA_SMOOTHER_H
#define DATA_SMOOTHER_H

#include <Arduino.h>

#include "cppQueue.h"

// Class form calculating average value of a data streem 
class DataSmoother
{
private:
    cppQueue *DataQueue;

public:
    DataSmoother(size_t N);
    ~DataSmoother();

    void AddData(double data);
    double GetData();
    // void PrintData();
};

#endif