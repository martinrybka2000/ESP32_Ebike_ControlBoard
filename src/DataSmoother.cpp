#include "DataSmoother.h"

DataSmoother::DataSmoother(size_t N)                            // creating a new Queue 
{
    DataQueue = new cppQueue(sizeof(double), N, FIFO, true);    
}

DataSmoother::~DataSmoother()
{
    delete DataQueue;
}

void DataSmoother::AddData(double data)                         // Adding new values to queue, if overflow the oldest value is overriten
{
    DataQueue->push(&data);                                     
}

double DataSmoother::GetData()                                  // Calculating the average value of queue values and returning it
{
    double average = 0;

    for (size_t i = 0; i < DataQueue->getCount(); i++)      
    {
        double tem = 0;
        DataQueue->peekIdx(&tem, i);
        average += tem;
    }

    return average / DataQueue->getCount();
}

// void DataSmoother::PrintData()
// {
//     double res;
//     for (size_t i = 0; i < DataQueue->getCount(); i++)
//     {
//         DataQueue->peekIdx(&res, i);
//         Serial.println("%d: ", i);
//         Serial.println(res);
//     }
// }
