#ifndef SLAVE_H
#define SLAVE_H

#include "wind.h"
#include "status.h"
#include "master.h"
#include <QHostAddress>
#include <QDateTime>

extern Master master;

class Slave{
public:
    int no;
    ConnectionStatus connectionStatus;
    float currentTemperature;
    float targetTemperature;
    Wind wind;
    float usage;
    float cost;
    QHostAddress address;
    quint16 port;
    QDateTime refreshTime;

    Slave(){
        this->no = 0;
        this->connectionStatus = ConnectionStatus::disconnected;
        this->currentTemperature = 25;
        this->targetTemperature = 25;
        this->wind = Wind::stop;
        this->usage = 0;
        this->cost = 0;
        this->address = QHostAddress::LocalHost;
        this->port = 0;
        this->refreshTime = QDateTime::currentDateTime();
    }

    Slave(int no){
        this->no = no;
        this->connectionStatus = ConnectionStatus::disconnected;
        this->currentTemperature = 25;
        this->targetTemperature = 25;
        this->wind = Wind::stop;
        this->usage = 0;
        this->cost = 0;
        this->address = QHostAddress::LocalHost;
        this->port = 0;
        this->refreshTime = QDateTime::currentDateTime();
    }

    Slave(int no, QHostAddress address, quint16 port){
        this->no = no;
        this->connectionStatus = ConnectionStatus::disconnected;
        this->currentTemperature = 25;
        this->targetTemperature = 25;
        this->wind = Wind::stop;
        this->usage = 0;
        this->cost = 0;
        this->address = address;
        this->port = port;
        this->refreshTime = QDateTime::currentDateTime();
    }

    QString generateStr();

    /*Slave(const Slave &slave){
        this->no = slave.no;
        this->connectionStatus = slave.connectionStatus;
        this->currentTemperature = slave.currentTemperature;
        this->targetTemperature = slave.targetTemperature;
        this->wind= slave.wind;
        this->usage = slave.usage;
        this->address = slave.address;
        this->port = slave.port;
    }*/
};

#endif // SLAVE_H
