#ifndef NODE_H
#define NODE_H

#include <QObject>

class Node
{
public:
    Node(double v);
    double Value();
    void Value(double);
    Node* Next();
    void Next(Node* value);

private:
    double value;
    Node* next;
};

#endif // NODE_H
