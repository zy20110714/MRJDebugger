#-------------------------------------------------
#
# Project created by QtCreator 2016-12-26T17:20:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RSJDebugger
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    paintarea.cpp \
    showitem.cpp \
    showqueue.cpp \
    node.cpp \
    mainwindow_OscilloScope.cpp \
    mainwindow_PID.cpp \
    mainwindow_Set.cpp

HEADERS  += mainwindow.h \
    paintarea.h \
    showitem.h \
    showqueue.h \
    node.h

FORMS    += mainwindow.ui

INCLUDEPATH += /home/zy/QtProjects/APIpthreadfordualarm/jointcontrolthread

LIBS += -L/home/zy/QtProjects/APIpthreadfordualarm/lib -lOUR_jointcontrolthread
