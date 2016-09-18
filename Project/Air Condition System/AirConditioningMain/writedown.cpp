#include "writedown.h"

#include <QDebug>

const QString DATETIMEFORMAT="yyyy-MM-dd HH:mm:ss";
extern float windUsage[4];
extern float windCost;

void writeDownBegin(int num, float beginTemp, Wind wind){
   QSqlQuery query;
   query.exec("INSERT INTO request (roomNum, begintime, begintemp, wind, pending) VALUES ("+
              QString::number(num)+",'"+
              QDateTime::currentDateTime().toString(DATETIMEFORMAT)+"',"+
              QString::number(beginTemp)+",'"+
              convertWindToQStr(wind)+"','"+
              "正在工作')");
}

void writeDownEnd(int num, float stopTemp){
    QSqlQuery query;
    query.exec("SELECT begintime, wind FROM request WHERE roomNum = "+QString::number(num)+" and pending = '正在工作'");
    while(query.next()){
        QDateTime beginTime = QDateTime::fromString(query.value(0).toString(),DATETIMEFORMAT);
        float seconds = beginTime.secsTo(QDateTime::currentDateTime());
        float cost = seconds * windUsage[(int)convertQStrToWind(query.value(1).toString())];
        query.exec("UPDATE request SET stoptime='"+QDateTime::currentDateTime().toString(DATETIMEFORMAT)+
                   "', stoptemp="+QString::number(stopTemp)+
                   ", cost="+QString::number(cost)+
                   ", pending='已结算'"
                   " WHERE roomNum="+QString::number(num)+
                   " and pending='正在工作'");
    }
}

void writeDownPowerOn(int num){
    QSqlQuery query;
    query.exec("INSERT INTO event (num, event, time) VALUES ("+
               QString::number(num)+
               ",'开机','"+
               QDateTime::currentDateTime().toString(DATETIMEFORMAT)+
               "')");

}

void writeDownPowerOff(int num){
    QSqlQuery query;
    query.exec("INSERT INTO event (num, event, time) VALUES ("+
               QString::number(num)+
               ",'关机','"+
               QDateTime::currentDateTime().toString(DATETIMEFORMAT)+
               "')");
}

void selectBigger(){
    QSqlQuery query;
    QDateTime now = QDateTime::currentDateTime();
    QDateTime month(QDate(now.date().year(),now.date().month(),1),QTime(0,0));
    query.exec("SELECT begintime FROM request WHERE begintime > '"+month.toString(DATETIMEFORMAT)+"'");
    qDebug()<<month;
    if(query.next()){
        QDateTime beginTime = QDateTime::fromString(query.value(0).toString(),DATETIMEFORMAT);
        qDebug()<<beginTime;
    }
}

