#-------------------------------------------------
#
# Project created by QtCreator 2015-12-05T16:18:18
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bank
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    regdialog.cpp \
    depositdialog.cpp \
    withdrawdialog.cpp \
    editdialog.cpp \
    database.cpp \
    account.cpp \
    bankserver.cpp \
    configdialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    regdialog.h \
    depositdialog.h \
    withdrawdialog.h \
    editdialog.h \
    account.h \
    bankserver.h \
    configdialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    regdialog.ui \
    depositdialog.ui \
    withdrawdialog.ui \
    editdialog.ui \
    configdialog.ui

DISTFILES +=

RESOURCES += \
    icon.qrc
