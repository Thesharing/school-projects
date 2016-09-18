#include "requestlist.h"
#include "slavelist.h"
#include "master.h"
#include "writedown.h"
#include <QDebug>

#define LOSTCONNECTTIME 15

extern Master master;
extern SlaveList slaveList;
extern float windUsage[4];
extern float windCost;

void RequestList::addSlave(int roomNum){
    if(findRequest(roomNum)==nullptr){
        Request newRequest(roomNum);
        requestList->append(newRequest);
    }
}

Request* RequestList::findRequest(int roomNum){
    Request* it = requestList->begin();
    while(it != requestList->end()){
        if(it->roomNum == roomNum){
            return it;
        }
        ++it;
    }
    return nullptr;
}

/*void RequestList::deleteRequest(int roomNum){
    Request * it = requestList->begin();
    while(it != requestList->end()){
        if(it->roomNum == roomNum){
            Request tmp(*it);
            requestList->removeOne(tmp);
            break;
        }
        ++it;
    }
}*/

void RequestList::updateRequest(Request newRequest){
    Request * request = findRequest(newRequest.roomNum);
    if(request!= nullptr){
        request->setRequest(newRequest);
    }
}

void RequestList::setRequestDealt(int roomNum){
    Request * request = findRequest(roomNum);
    if(request != nullptr){
        request->dealt=true;
    }
}

Request* RequestList::findNextAvailableRequest(){
    if(master.strategyMode == Strategy::FIFS){
        Request * result = nullptr;
        Request * it = requestList->begin();
        while(it != requestList->end()){
            if(it->dealt == false){
                if(result == nullptr || result->time.msecsTo(it->time)<0){
                    result = it;
                }
            }
            ++it;
        }
        return result;
    }
    else if(master.strategyMode == Strategy::HP){
        Request * result = nullptr;
        Request * it = requestList->begin();
        while(it != requestList->end()){
            if(it->dealt == false){
                if(it->wind == Wind::stop){
                    it->dealt = true;
                }
                else if(result == nullptr || result->wind < it->wind){
                    result = it;
                }
            }
            ++it;
        }
        return result;
    }
    return nullptr;
}

void RequestList::dealRequests(){
    QVector<Slave> * list = slaveList.getSalves();
    Slave* it = list->begin();
    while(it != list->end()){
        if(it->connectionStatus!=ConnectionStatus::disconnected && it->refreshTime.secsTo(QDateTime::currentDateTime()) > LOSTCONNECTTIME){
            it->connectionStatus = ConnectionStatus::lostconnect;
            Request * temp = findRequest(it->no);
            temp->dealt = true;
            if(it->wind != Wind::stop){
                it->wind = Wind::stop;
                master.nowNum--;
                qDebug()<<"1--";
                writeDownEnd(it->no,it->currentTemperature);
            }
        }
        else if(it->wind != Wind::stop){
            it->usage += windUsage[(int)it->wind];
            it->cost += windUsage[(int)it->wind]*windCost;
            Request * temp = findRequest(it->no);
            if(temp->dealt == false){
                if(temp->workMode == master.workMode && temp->wind != it->wind){
                    writeDownEnd(it->no,it->currentTemperature);
                    it->wind = temp->wind;
                    writeDownBegin(it->no,it->currentTemperature,it->wind);
                    if(it->wind == Wind::stop){
                        master.nowNum--;
                        qDebug()<<"2--";
                    }
                }
                temp->dealt = true;
            }
        }
        it++;
    }
    if(master.strategyMode == Strategy::FIFS){
        while(master.nowNum < master.maxNum){
            Request * temp = findNextAvailableRequest();
            if(temp==nullptr){
                break;
            }
            if(temp->wind == Wind::stop){
                Slave * tempSlave = slaveList.findSlave(temp->roomNum);
                if(tempSlave!=nullptr){
                    if(tempSlave->wind != Wind::stop){
                        writeDownBegin(tempSlave->no,tempSlave->currentTemperature,tempSlave->wind);
                        tempSlave->wind = Wind::stop;
                        master.nowNum--;
                    }
                }
                temp->dealt = true;
            }
            else{
                Slave * tempSlave = slaveList.findSlave(temp->roomNum);
                tempSlave->wind = temp->wind;
                temp->dealt = true;
                master.nowNum++;
                qDebug()<<"1++";
                writeDownBegin(tempSlave->no,tempSlave->currentTemperature,tempSlave->wind);
            }
        }
    }
    else if(master.strategyMode == Strategy::HP){
        while(master.nowNum < master.maxNum){
            Request * temp = findNextAvailableRequest();
            if(temp == nullptr){
                break;
            }
            if(temp->wind == Wind::stop){
                Slave * tempSlave = slaveList.findSlave(temp->roomNum);
                if(tempSlave!=nullptr){
                    if(tempSlave->wind != Wind::stop){
                        writeDownBegin(tempSlave->no,tempSlave->currentTemperature,tempSlave->wind);
                        tempSlave->wind = Wind::stop;
                        master.nowNum--;
                    }
                }
                temp->dealt = true;
            }
            else{
                Slave * tempSlave = slaveList.findSlave(temp->roomNum);
                tempSlave->wind = temp->wind;
                temp->dealt = true;
                master.nowNum++;
                qDebug()<<"2++";
                writeDownBegin(tempSlave->no,tempSlave->currentTemperature,tempSlave->wind);
            }
        }
        while(true){
            Request * requestTemp = findNextAvailableRequest();
            if(requestTemp == nullptr){
                break;
            }
            Slave * slaveTemp = slaveList.getLowestWindSlave();
            if(slaveTemp == nullptr){
                break;
            }
            if(requestTemp->wind > slaveTemp->wind){
                Request * requestTemp2 = findRequest(slaveTemp->no);
                requestTemp2->wind = slaveTemp->wind;
                requestTemp2->targetTemperature =slaveTemp->targetTemperature;
                requestTemp2->workMode = master.workMode;
                requestTemp2->time = QDateTime::currentDateTime();
                requestTemp2->dealt = false;
                slaveTemp->wind = Wind::stop;
                writeDownEnd(slaveTemp->no,slaveTemp->currentTemperature);
                Slave * slaveTemp2 = slaveList.findSlave(requestTemp->roomNum);
                slaveTemp2->wind = requestTemp->wind;
                requestTemp->dealt = true;
                writeDownBegin(slaveTemp2->no,slaveTemp2->currentTemperature,slaveTemp2->wind);
            }
            else{
                break;
            }
        }
    }
}

void RequestList::setAllRequestDealt(){
    Request * it = requestList->begin();
    while(it != requestList->end()){
        it->dealt = true;
        ++it;
    }
}
