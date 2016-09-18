#-------------------------------------------------
#
# Project created by QtCreator 2015-12-08T20:41:40
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Store
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    regdialog.cpp \
    product.cpp \
    food.cpp \
    cloth.cpp \
    book.cpp \
    database.cpp \
    cartdialog.cpp \
    addcartdialog.cpp \
    electro.cpp \
    carteditdialog.cpp \
    paydialog.cpp \
    settingdialog.cpp \
    configdialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    regdialog.h \
    product.h \
    datastruct.h \
    food.h \
    cloth.h \
    book.h \
    type.h \
    cartdialog.h \
    addcartdialog.h \
    electro.h \
    carteditdialog.h \
    paydialog.h \
    settingdialog.h \
    configdialog.h \
    defineres.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    regdialog.ui \
    cartdialog.ui \
    addcartdialog.ui \
    carteditdialog.ui \
    paydialog.ui \
    settingdialog.ui \
    configdialog.ui

RESOURCES += \
    icon.qrc
