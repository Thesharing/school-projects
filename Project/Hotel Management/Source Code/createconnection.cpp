#include <QtSql>
#include <QWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::createConnection(bool isExist){
    if(!db.open()){
        //如果数据库未打开，则报错
        QMessageBox::critical(0,QObject::tr("数据库错误"),db.lastError().text());
        return;
    }
    QSqlQuery query;
    if(!isExist){//如果数据库是第一次创建，则写入表项
        //guestInfo是宾客信息
        //roomInfo是房间信息
        //bookInfo是预定信息
        //log是日志信息
        query.exec("CREATE TABLE guestInfo ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name NVARCHAR,"
                    "gender NVARCHAR,"
                    "idNum NVARCHAR,"
                    "workplace NVARCHAR,"
                    "phoneNum NVARCHAR,"
                    "consume INT,"
                    "checkInTime INT,"
                    "checkOutTime INT,"
                    "checkInTimeText NVARCHAR,"
                    "checkOutTimeText NVARCHAR,"
                    "room INT)"
                    );
        query.exec("CREATE TABLE roomInfo ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "num INT,"
                    "type VARCHAR,"
                    "status VARCHAR,"
                    "checkInTime INT,"
                    "checkOutTime INT,"
                    "checkInTimeText NVARCHAR,"
                    "checkOutTimeText NVARCHAR,"
                    "cost INT)"
                    );
        query.exec("CREATE TABLE bookInfo ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name VARCHAR,"
                    "phoneNum VARCHAR,"
                    "checkInTime INT,"
                    "checkOutTime INT,"
                    "checkInTimeText NVARCHAR,"
                    "checkOutTimeText NVARCHAR,"
                    "room INT)"
                    );
        query.exec("CREATE TABLE log ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "time NVARCHAR," //******
                    "string NVARCHAR)"
                    );
        //向房间表里写入初始的房间信息
        sqlInitialize();
    }
}

void MainWindow::sqlInitialize(){//向房间表里写入初始的房间信息
    QSqlQuery query;
    for(int j=1;j<=18;j++){
        query.exec("INSERT INTO roomInfo (num, type, status, checkInTime, checkOutTime, cost)"
            "VALUES("+QString::number(200+j)+", 'standard', 'empty', 0, 0, 0)");
    }
    for(int j=1;j<=18;j++){
        query.exec("INSERT INTO roomInfo (num, type, status, checkInTime, checkOutTime, cost)"
            "VALUES("+QString::number(300+j)+", 'doublebed', 'empty', 0, 0, 0)");
    }
    for(int j=1;j<=18;j++){
        query.exec("INSERT INTO roomInfo (num, type, status, checkInTime, checkOutTime, cost)"
            "VALUES("+QString::number(400+j)+", 'largebed', 'empty', 0, 0, 0)");
    }
    for(int j=1;j<=18;j++){
        query.exec("INSERT INTO roomInfo (num, type, status, checkInTime, checkOutTime, cost)"
            "VALUES("+QString::number(500+j)+", 'business', 'empty', 0, 0, 0)");
    }
    for(int j=1;j<=18;j++){
        query.exec("INSERT INTO roomInfo (num, type, status, checkInTime, checkOutTime, cost)"
            "VALUES("+QString::number(600+j)+", 'vip', 'empty', 0, 0, 0)");
    }
}

