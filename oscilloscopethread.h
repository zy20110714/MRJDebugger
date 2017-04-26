#ifndef OSCILLOSCOPETHREAD_H
#define OSCILLOSCOPETHREAD_H

#include <QThread>

class OscilloScopeThread : public QThread
{
public:
    OscilloScopeThread(void * t);
protected:
    void run();
private:
    void * p;
};

#endif // OSCILLOSCOPETHREAD_H
