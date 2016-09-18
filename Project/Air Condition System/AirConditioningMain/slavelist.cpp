#include "slavelist.h"
#include "requestlist.h"
#include "master.h"
#include "writedown.h"
#include <QDebug>
#include <QSqlQuery>
#include <QVariant>

extern RequestList requestList;
extern Master master;

bool SlaveList::registerSlave(int roomNum, QHostAddress address, quint16 port){
    Slave * pos = findSlave(roomNum);
    Slave * temp = new Slave(roomNum, address, port);
    temp->connectionStatus = ConnectionStatus::connected;
    if(pos!=nullptr){
        pos->connectionStatus = ConnectionStatus::connected;
        pos->refreshTime = QDateTime::currentDateTime();
    }
    else if(slaveList->size()<maxNum){
        QSqlQuery query;
        query.exec("SELECT usage, cost FROM slave WHERE num="+QString::number(roomNum));
        if(query.next()){
            float usage = query.value(0).toFloat();
            temp->usage = usage;
            float cost = query.value(1).toFloat();
            temp->cost = cost;
        }
        slaveList->append(*temp);
        requestList.addSlave(temp->no);     
    }
    else{
        pos = findAvailablePos();
        if(pos != nullptr){
            slaveList->erase(pos);
            slaveList->append(*temp);
        }
        else return false;
    }
    return true;
}

Slave* SlaveList::findSlave(Slave slave){
    Slave* it = slaveList->begin();
    while(it != slaveList->end()){
        if(it->no == slave.no){
            return it;
        }
        it++;
    }
    return nullptr;
}

Slave *SlaveList::findSlave(int num){
    Slave * it = slaveList->begin();
    while(it != slaveList->end()){
        if(it->no == num){
            return it;
        }
        it++;
    }
    return nullptr;
}

Slave* SlaveList::findAvailablePos(){
    Slave* it = slaveList->begin();
    while(it != slaveList->end()){
        if(it->connectionStatus == ConnectionStatus::disconnected){
            return it;
        }
        it++;
    }
    it = slaveList->begin();
    while(it != slaveList->end()){
        if(it->connectionStatus == ConnectionStatus::lostconnect){
            return it;
        }
        it++;
    }
    return nullptr;
}

void SlaveList::updateSlave(int roomNum, float targetTemp, float nowTemp, QDateTime refreshTime){
    Slave * temp = findSlave(roomNum);
    if(temp != nullptr){
        temp->currentTemperature = nowTemp;
        temp->targetTemperature = targetTemp;
        temp->refreshTime = refreshTime;
    }
}

void SlaveList::updateSlave(int roomNum, Wind wind, float usage){
    Slave * temp = findSlave(roomNum);
    if(temp != nullptr){
        temp->wind = wind;
        temp->usage += usage;
    }
}

void SlaveList::updateSlave(int roomNum, ConnectionStatus status){
    Slave * temp = findSlave(roomNum);
    if(temp != nullptr)
        temp->connectionStatus = status;
}

void SlaveList::updateSlave(int roomNum, ConnectionStatus status,  Wind wind, float usage){
    Slave * temp = findSlave(roomNum);
    if(temp != nullptr){
        temp->connectionStatus = status;
        temp->wind = wind;
        temp->usage += usage;
    }
}

void SlaveList::logoffSlave(int roomNum){
    Slave * temp = findSlave(roomNum);
    if(temp != nullptr){
        temp->connectionStatus = ConnectionStatus::disconnected;
        requestList.setRequestDealt(roomNum);
        if(temp->wind != Wind::stop){
            temp->wind = Wind::stop;
            master.nowNum--;
            writeDownEnd(temp->no,temp->currentTemperature);
        }
    }
}

Slave * SlaveList::getLowestWindSlave(){
    if(slaveList->size() == 0){
        return nullptr;
    }
    Slave * result = nullptr;
    Slave * it = slaveList->begin();
    while(it != slaveList->end()){
        if(it->wind!=Wind::stop){
            if(result == nullptr || result->wind > it->wind){
                result = it;
            }
        }
        it++;
    }
    return result;
}

void SlaveList::stopAllSlaves(){
    Slave* it = slaveList->begin();
    while(it != slaveList->end()){
        if(!master.isWorking)
            it->connectionStatus = ConnectionStatus::disconnected;
        if(it->wind!=Wind::stop){
            it->wind = Wind::stop;
            master.nowNum--;
            writeDownEnd(it->no,it->currentTemperature);
        }
        it++;
    }
}

void SlaveList::saveToDatabase(){
    QSqlQuery query;
    Slave* it = slaveList->begin();
    while(it != slaveList->end()){
        query.exec("SELECT * FROM slave WHERE num ="+QString::number(it->no)+"'");
        if(query.next()){
            query.exec("UPDATE slave SET usage ="+QString::number(it->usage)+", cost ="+QString::number(it->cost)+" WHERE num ="+QString::number(it->no));
        }
        else{
            query.exec("INSERT INTO slave (num, usage, cost) VALUES ("+QString::number(it->no)+","+
                       QString::number(it->usage)+","+QString::number(it->cost)+")");
        }
        it++;
    }
}
