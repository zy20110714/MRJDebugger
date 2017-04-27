#-------------------------------------------------
#
# Project created by QtCreator 2016-12-26T17:20:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MRJDebugger
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    mainwindow_OscilloScope.cpp \
    mainwindow_PID.cpp \
    mainwindow_Set.cpp \
    connectform.cpp \
    Component/paintarea.cpp \
    Component/showitem.cpp \
    Component/showqueue.cpp \
    mainwindow_move.cpp \
    mainwindow_health.cpp \
    mainwindow_bottom.cpp \
    advancedcontrol.cpp \
    oscilloscopethread.cpp

HEADERS  += mainwindow.h \
    connectform.h \
    Component/paintarea.h \
    Component/showitem.h \
    Component/showqueue.h \
    advancedcontrol.h \
    oscilloscopethread.h

FORMS    += mainwindow.ui \
    connectform.ui \
    advancedcontrol.ui

#INCLUDEPATH += /home/zy/QtProjects/APIpthreadfordualarm/jointcontrolthread
#LIBS += -L/home/zy/QtProjects/APIpthreadfordualarm/lib -lOUR_jointcontrolthread

include ($$PWD/qwt/qwt.prf)
#INCLUDEPATH += C:/Qwt-6.1.3/include
#LIBS += C:/Qwt-6.1.3/lib/qwt.dll

INCLUDEPATH += $$PWD/Include \
    $$PWD/Component

LIBS += $$PWD/build/release/CANAPI.dll

RESOURCES += \
    image.qrc
RC_FILE = MRJDebugger.rc
