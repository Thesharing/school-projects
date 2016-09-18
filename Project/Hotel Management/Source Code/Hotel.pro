#-------------------------------------------------
#
# Project created by QtCreator 2015-05-23T20:51:19
#
#-------------------------------------------------

QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hotel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    createconnection.cpp \
    guestdialog.cpp \
    roominfodialog.cpp \
    checkin.cpp \
    checkout.cpp \
    dialogconsume.cpp \
    reservedialog.cpp \
    backupdialog.cpp \
    restoredialog.cpp \
    checkin1dialog.cpp \
    cancelreservedialog.cpp \
    setting.cpp \
    helpdialog.cpp \
    checkindoubledialog.cpp

HEADERS  += mainwindow.h \
    datastruct.h \
    guestdialog.h \
    roominfodialog.h \
    checkin.h \
    checkout.h \
    dialogconsume.h \
    reservedialog.h \
    backupdialog.h \
    restoredialog.h \
    checkin1dialog.h \
    cancelreservedialog.h \
    setting.h \
    helpdialog.h \
    checkindoubledialog.h

FORMS    += mainwindow.ui \
    guestdialog.ui \
    roominfodialog.ui \
    dialogconsume.ui \
    checkindialog.ui \
    checkoutDialog.ui \
    reservedialog.ui \
    backupdialog.ui \
    restoredialog.ui \
    checkin1dialog.ui \
    cancelreservedialog.ui \
    setting.ui \
    helpdialog.ui \
    checkindoubledialog.ui

DISTFILES += \
    ../../../主题/Icons/Metro/Metro UI Icon Set/Folders & OS/Blank Folder.png \
    ../../../主题/Icons/Metro/Metro UI Icon Set/Applications/Adobe Acrobat Reader.png \
    ../../../主题/Icons/Metro/Metro UI Icon Set/Applications/Adobe Acrobat Reader.png \
    ../../../主题/Icons/Metro/Metro Uinvert Dock Icon Set/Google Services/Google Reader alt.png

RESOURCES += \
    icons.qrc
