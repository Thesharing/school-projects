#ifndef REQUEST_H
#define REQUEST_H

#include "wind.h"
#include "workmode.h"
#include <QDateTime>

class Request
{
public:
    int roomNum;
    WorkMode workMode;
    Wind wind;
    float targetTemperature;
    bool dealt;
    QDateTime time;

    Request(){
        roomNum =0;
        workMode = WorkMode::Cold;
        wind = Wind::stop;
        targetTemperature =25;
        dealt = true;
        time = QDateTime::currentDateTime();
    }

    Request(int num){
        roomNum = num;
        workMode = WorkMode::Cold;
        wind = Wind::stop;
        targetTemperature = 25;
        dealt = true;
        time = QDateTime::currentDateTime();
    }

    Request(int num, WorkMode workMode, Wind wind, float targetTemp, bool dealt, QDateTime time):
    roomNum(num),workMode(workMode),wind(wind),targetTemperature(targetTemp),dealt(dealt),time(time)
    { }

    Request(const Request& request){
        roomNum = request.roomNum;
        workMode = request.workMode;
        wind = request.wind;
        targetTemperature = request.targetTemperature;
        dealt = request.dealt;
        time = request.time;
    }

    void setRequest(Request request);
    QString outputForDialog();
};

#endif // REQUEST_H
