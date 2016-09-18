#ifndef MASTER_H
#define MASTER_H

#include "workmode.h"
#include "strategy.h"
#include <QString>
#include <QDateTime>


class Master{
public:
    bool isWorking;
    WorkMode workMode;
    Strategy strategyMode;
    QString account;
    float refreshFreq;
    QDateTime bootTime;
    int nowNum;
    int maxNum;

    Master(){
        isWorking = false;
        workMode = WorkMode::Cold;
        strategyMode = Strategy::FIFS;
        account = QString();
        refreshFreq = 1;
        bootTime = QDateTime::currentDateTime();
        nowNum = 0;
        maxNum = 3;
    }

    Master(WorkMode workMode, QString account, float refreshFreq,
           QDateTime bootTime, int nowNum, int maxNum):workMode(workMode),account(account),
           refreshFreq(refreshFreq),bootTime(bootTime), nowNum(nowNum), maxNum(maxNum)
    { isWorking = false;}
};

#endif // MASTER_H
