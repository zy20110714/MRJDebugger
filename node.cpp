#include "node.h"

Node::Node(double v)
{
    this->value = v;
    this->next = NULL;
}

void Node::Value(double)
{
    this->value = value;
}

double Node::Value()
{
    return this->value;
}

Node* Node::Next()
{
    return this->next;
}

void Node::Next(Node* value)
{
    this->next = value;
}
