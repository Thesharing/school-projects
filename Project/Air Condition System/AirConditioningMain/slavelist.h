#ifndef SLAVELIST_H
#define SLAVELIST_H

#include <QVector>
#include "slave.h"
#include "status.h"
#include "wind.h"

class SlaveList{
private:
    QVector<Slave> * slaves; //USELESS
    QVector<Slave> * slaveList;
    int nowNum;
    int maxNum;
public:
    SlaveList(){
        maxNum = 6;
        nowNum = 0;
        slaveList = new QVector<Slave>;
        slaves = new QVector<Slave>;
    }

    ~SlaveList(){
        delete slaves;
        delete slaveList;
    }

    bool registerSlave(int roomNum, QHostAddress address, quint16 port);
    Slave* findSlave(Slave slave);
    Slave* findSlave(int num);
    Slave* findAvailablePos();
    QVector<Slave>* getSalves(){
        return slaveList;
    }
    Slave * getLowestWindSlave();
    void updateSlave(int roomNum, float targetTemp, float nowTemp, QDateTime refreshTime);
    void updateSlave(int roomNum, Wind wind, float usage);
    void updateSlave(int roomNum, ConnectionStatus status);
    void updateSlave(int roomNum, ConnectionStatus status,  Wind wind, float usage);
    void logoffSlave(int roomNum);
    void stopAllSlaves();
    void saveToDatabase();
};


#endif // SLAVELIST_H
