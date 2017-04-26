#include "oscilloscopethread.h"
#include <QDebug>
#include "mainwindow.h"

OscilloScopeThread::OscilloScopeThread(void *t)
{
    p = t;
}

void OscilloScopeThread::run()
{
    qDebug() << "Thread run";
    MainWindow *JT = (MainWindow*)p;
//    MainWindow * JT = (MainWindow*)arg;
//    //自定义高精度定时器，CPU占用51~70%左右
    while(true) { // < 执行的次数
        JT->slotTimeOscilloScopeDone();
//        GetLocalTime( &sys );
//        cout << sys.wHour << ":"
//        << sys.wMinute << ":"
//        << sys.wSecond << ":"
//        << sys.wMilliseconds << endl;
        JT->can1->controller.delayMs(10);
    } // CPU占用43%左右
}
