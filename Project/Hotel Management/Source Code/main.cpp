#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QDateTime>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include "datastruct.h"

int stringToIntForType(QString typeStr);
void logPrint(QString str);

int layerShow;
int price[5];//房间价格
int itemPrice[5]; //房间内消费的价格
int interval;
bool searchmodel;

int now;
QString nowFormat;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash =new QSplashScreen;
    splash->setPixmap(QPixmap(":/image/image/splash.jpg"));
    splash->show();

    a.processEvents();

    nowFormat="yyyy/MM/dd hh:mm:ss";

    MainWindow w;
    w.show();

    splash->finish(&w);
    delete splash;

    w.roomButtonChange();
    return a.exec();
}


int stringToIntForType(QString typeStr){ //将对应类型房间字符串转换成房价
    switch(typeStr.at(0).toLatin1()){
        case 's':
            return price[0];
            break;
        case 'd':
            return price[1];
            break;
        case 'l':
            return price[2];
            break;
        case 'b':
            return price[3];
            break;
        case 'v':
            return price[4];
            break;
        default:
            return 0;
            break;
    }
}

void logPrint(QString str){ //日志函数
    QSqlQuery query;
    QDateTime temp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
    temp=temp.addSecs(qint64(now));
    query.exec("INSERT INTO log (time, string)"
        "VALUES('"+temp.toString(nowFormat)+"', '"+str+"')");
}
