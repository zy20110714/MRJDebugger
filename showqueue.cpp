#include "showqueue.h"
#include <iostream>

ShowQueue::ShowQueue(int queueSize)
{
    head = rear = NULL;
    num = 0;
    MaxLength = queueSize;
}

int ShowQueue::Count()
{
    return num;
}

void ShowQueue::DelHead()
{
    if (IsEmpty()) {
        return;
    }
    Node* t = head;
    if (head->Next() != head) {
        head = head->Next();
        rear->Next(head);
    } else {
        head = rear = NULL;
    }
    free(t);
    num--;
}

void ShowQueue::Clear()
{
    while (!IsEmpty()) {
        DelHead();
    }
}

bool ShowQueue::IsEmpty()
{
    return (num == 0);
}

//追加
void ShowQueue::Append(double data)
{
    if (MaxLength < 1) {
        return;
    }
    Node* q = new Node(data);
    //第一个元素入列时
    if (IsEmpty()) {
        head = rear = q;
        rear->Next(head);
        num++;
    } else {
        //若队列已经满了
        if (Count() == MaxLength) {
            // delete head then append
            DelHead();
        }
        //把新元素挂到链尾
        rear->Next(q);
        //修正rear指向为最后一个元素
        rear = q;
        rear->Next(head);
        //元素总数+1
        num++;
    }
}

void ShowQueue::FillZero()
{
    Clear();
    for (int i = 0; i < MaxLength; i++) {
        Append(0);
    }
}

int ShowQueue::GetValue(int index)
{
    if (index >= Count()) {
        std::cout << "ShowQueue::GetValue : index is out of range.\n";
        std::cout.flush();
        return 0;
    }
    Node* current = head;
    int j = 0;
    while (j != index) {
        current = current->Next();
        j++;
    }
    return current->Value();
}

////得到区间内所有数据的平均值
//float ShowQueue::GetAverageValue(int m, int n)
//{
//    //允许选定的区间超过数据范围，但选定区间起始位置在数据范围内才返回有效值。m和n都从0开始，而Count()在只有1个元素的时候返回的是1
//    if (m > Count() - 1)
//    {
//        return 0.0f;
//    }
//    //进入临界区对链表进行读取
//    //访问链表元素的指针
//    Node current = head;
//    //用来配合计数
//    int j = 0;
//    //移动到链表指定区间起始处
//    while (j != m)
//    {
//        current = current.Next;
//        j++;
//    }
//    //取所要的值中的第1个
//    float sum = current.Value;
//    current = current.Next;
//    j++;
//    //第n个值还没加上，则继续循环
//    while (j != n + 1)
//    {
//        //全部累加后float有可能溢出
//        sum += current.Value;
//        //移动指针
//        current = current.Next;
//        j++;
//    }
//    //总和sum除以已累加的元素个数，得到均值
//    return sum / (j - m);
//}

////得到区间内数据的最大值
//float ShowQueue::GetMaxValue(int m, int n)
//{
//    //允许选定的区间超过数据范围，但选定区间起始位置在数据范围内才返回有效值
//    if (m > Count() - 1)
//    {
//        return 0.0f;
//    }
//    Node current = head;
//    int j = 0;
//    while (j != m)
//    {
//        current = current.Next;
//        j++;
//    }
//    float max = current.Value;
//    current = current.Next;
//    j++;
//    while (j != n + 1)
//    {
//        if (current.Value > max)
//        {
//            max = current.Value;
//        }
//        current = current.Next;
//        j++;
//    }
//    return max;
//}

////得到区间内数据的最小值
//float ShowQueue::GetMinValue(int m, int n)
//{
//    if (m > Count() - 1)
//    {
//        return 0.0f;
//    }
//    Node current = head;
//    int j = 0;
//    while (j != m)
//    {
//        current = current.Next;
//        j++;
//    }
//    float min = current.Value;
//    current = current.Next;
//    j++;
//    while (j != n + 1)
//    {
//        if (current.Value < min)
//        {
//            min = current.Value;
//        }
//        current = current.Next;
//        j++;
//    }
//    return min;
//}

////得到区间内数据的标准差（总体）
//float ShowQueue::GetSDeviation(int m, int n)
//{
//    if (m > Count() - 1)
//    {
//        return 0.0f;
//    }
//    //求出这组数据的均值
//    float pAverage = Convert.ToSingle(GetAverageValue(m, n));

//    //循环中求“离均差平方和”，访问链表元素的指针
//    Node current = head;
//    //用来配合计数
//    int j = 0;
//    //移动到链表指定区间起始处
//    while (j != m)
//    {
//        current = current.Next;
//        j++;
//    }
//    //取所要的值中的第1个
//    float sum = Convert.ToSingle(Math.Pow(current.Value - pAverage, 2));
//    current = current.Next;
//    j++;
//    //第n个值还没加上，则继续循环
//    while (j != n + 1)
//    {
//        //全部累加后float有可能溢出
//        sum += Convert.ToSingle(Math.Pow(current.Value - pAverage, 2));
//        //移动指针
//        current = current.Next;
//        j++;
//    }

//    //返回上一步结果平均后的方根
//    return Convert.ToSingle(Math.Sqrt(sum / (j - m)));
//}

using std::vector;

void ShowQueue::Print(std::vector<double> & eachValue)
{
    Node* current = head;
    for (vector<double>::iterator iter = eachValue.begin(); iter != eachValue.end(); ++iter) {
        *iter = current->Value();
        current = current->Next();
    }
}
