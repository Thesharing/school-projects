#-------------------------------------------------
#
# Project created by QtCreator 2016-06-01T20:31:43
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AirConditioningMain
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    regaccountdialog.cpp \
    configdialog.cpp \
    slavelist.cpp \
    slavedisplay.cpp \
    convert.cpp \
    requestlist.cpp \
    request.cpp \
    slave.cpp \
    debugdialog.cpp \
    writedown.cpp \
    reportdialog.cpp \
    checkoutdialog.cpp

HEADERS  += mainwindow.h \
    workmode.h \
    master.h \
    slave.h \
    wind.h \
    logindialog.h \
    regaccountdialog.h \
    configdialog.h \
    slavelist.h \
    status.h \
    slavedisplay.h \
    convert.h \
    requestlist.h \
    request.h \
    strategy.h \
    debugdialog.h \
    writedown.h \
    reportdialog.h \
    checkoutdialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    regaccountdialog.ui \
    configdialog.ui \
    debugdialog.ui \
    reportdialog.ui \
    checkoutdialog.ui

RESOURCES += \
    icons.qrc
