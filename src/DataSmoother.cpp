// #include "DataSmoother.h"

// DataSmoother::DataSmoother(size_t N)
// {
//     DataQueue = new cppQueue(sizeof(double), N, FIFO, true);
// }

// DataSmoother::~DataSmoother()
// {
//     delete DataQueue;
// }

// void DataSmoother::AddData(double data)
// {
//     DataQueue->push(&data);
// }

// double DataSmoother::GetData()
// {
//     double average = 0;

//     for (size_t i = 0; i < DataQueue->getCount(); i++)
//     {
//         double tem = 0;
//         DataQueue->peekIdx(&tem, i);
//         average += tem;
//     }

//     return average / DataQueue->getCount();
// }

// void DataSmoother::PrintData()
// {
//     double res;
//     for (size_t i = 0; i < DataQueue->getCount(); i++)
//     {
//         DataQueue->peekIdx(&res, i);
//         Serial.printf("%d: ", i);
//         Serial.println(res);
//     }
// }
