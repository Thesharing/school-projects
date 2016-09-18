#include "mainwindow.h"
#include "logindialog.h"
#include "configdialog.h"
#include "master.h"
#include "slavelist.h"
#include "requestlist.h"
#include "writedown.h"

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDateTime>
#include <QDialog>

Master master;
SlaveList slaveList;
RequestList requestList;
float windUsage[4];
float windCost;
bool isInitialed = false;

bool initConfig();
bool initDatabase();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(initDatabase()==false){
        return -1;
    }
    loginDialog * logindialog = new loginDialog();
    int result = logindialog->exec();
    windUsage[(int)Wind::high] = 1.5;
    windUsage[(int)Wind::medium] = 1;
    windUsage[(int)Wind::low] = 0.5;
    windUsage[(int)Wind::stop] = 0;
    windCost = 1;
    if(result==QDialog::Accepted){
        if(QMessageBox::question(NULL, "恢复备份", "是否使用上次的设置？",
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes){
            if(initConfig()==false){
                configDialog * configdialog = new configDialog();
                while(configdialog->exec() == QDialog::Rejected){
                    QMessageBox::warning(NULL, "设置", "您必须要进行设置才能进入主控机。", QMessageBox::Yes | QMessageBox::Yes);
                }
            }
        }
        else{
            configDialog * configdialog = new configDialog();
            while(configdialog->exec() == QDialog::Rejected){
                QMessageBox::warning(NULL, "设置", "您必须要进行设置才能进入主控机。", QMessageBox::Yes | QMessageBox::Yes);
            }
        }
        MainWindow w;
        w.show();
        return a.exec();
    }
    else return 0;
}


bool initConfig(){
    QFileInfo dbFileInfo("config.ini");
    bool isExist = dbFileInfo.exists();
    QFile text("config.ini");
    text.close();
    if(!text.open(QIODevice::Text|QIODevice::ReadWrite)){
        QMessageBox::warning(NULL, "提示","无法打开配置文件，请联系管理员。", QMessageBox::Yes | QMessageBox::Yes);
        return false;
    }
    if(isExist){
        QTextStream in(&text);
        try{
            int workMode;
            in>>workMode;
            master.workMode=WorkMode(workMode);
            int strategyMode;
            in>>strategyMode;
            master.strategyMode = Strategy(strategyMode);
            float refreshFreq;
            in>>refreshFreq;
            master.refreshFreq = refreshFreq;
            float cost;
            in>>cost;
            windCost = cost;
            float usage;
            in>>usage;
            windUsage[(int)Wind::low] = usage;
            in>>usage;
            windUsage[(int)Wind::medium] = usage;
            in>>usage;
            windUsage[(int)Wind::high] = usage;
            master.bootTime = QDateTime::currentDateTime();
            float maxNum;
            in>>maxNum;
            master.maxNum = maxNum;
        }
        catch(...){
            return false;
        }
    }
    return true;
}

bool initDatabase(){
    QFileInfo dbFileInfo("data.db");
    bool isExist = dbFileInfo.exists();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");
    if(!db.open()){
        QMessageBox::critical(NULL,QObject::tr("数据库错误"),db.lastError().text());
        return false;
    }
    QSqlQuery query;
    if(!isExist){
        query.exec("CREATE TABLE accountInfo ("
                   "account NVARCHAR PRIMARY KEY,"
                   "password NVARCHAR)");
        query.exec("CREATE TABLE slave ("
                   "num INT,"
                   "usage REAL,"
                   "cost REAL)");
        query.exec("CREATE TABLE request("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "roomNum INT,"
                   "begintime VARCHAR,"
                   "stoptime VARCHAR,"
                   "begintemp REAL,"
                   "stoptemp REAL,"
                   "wind REAL,"
                   "cost REAL,"
                   "pending NVARCHAR)");
        query.exec("CREATE TABLE event("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "num INT,"
                   "event NVARCHAR,"
                   "time VARCHAR)");
    }
    return true;
}

