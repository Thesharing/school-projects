#include "request.h"
#include "convert.h"
#include <QDebug>

void Request::setRequest(Request request){
    this->roomNum = request.roomNum;
    this->workMode = request.workMode;
    this->wind = request.wind;
    this->targetTemperature = request.targetTemperature;
    this->time = request.time;
    this->dealt = request.dealt;
}

QString Request::outputForDialog(){
   QString str = "RoomNum:"+ QString::number(this->roomNum)+" WorkMode:"+convertWorkModeToQStr(this->workMode)+" Wind:"+convertWindToQStr(this->wind)+" Target:"+QString::number(this->targetTemperature)+" Time:"+this->time.time().toString()+" Dealt: ";
   if (this->dealt)
       str += "true";
   else str += "false";
   return str;
}
