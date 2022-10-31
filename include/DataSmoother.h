#ifndef DATA_SMOOTHER_H
#define DATA_SMOOTHER_H

#include <Arduino.h>

#include "cppQueue.h"

class DataSmoother
{
private:
    cppQueue *DataQueue;

public:
    DataSmoother(size_t N);
    ~DataSmoother();

    void AddData(double data);
    double GetData();
    void PrintData();
};

#endif