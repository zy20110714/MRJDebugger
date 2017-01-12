#ifndef SHOWQUEUE_H
#define SHOWQUEUE_H

#include <QObject>
#include <vector>
#include "node.h"

class ShowQueue
{
public:
    int MaxLength;
    Node* head;//队列头
    Node* rear;//队列尾
    ShowQueue(int queueSize);
    int Count();
    void Clear();
    bool IsEmpty();
    void Append(double data);
//    int DeQ();
    int GetValue(int index);
//    float GetAverageValue(int m, int n);
//    float GetMaxValue(int m, int n);
//    float GetMinValue(int m, int n);
//    float GetSDeviation(int m, int n);
    void Print(std::vector<double> & eachValue);
    void DelHead();
    void FillZero();

private:
    int num;//队列元素个数
};

#endif // SHOWQUEUE_H
