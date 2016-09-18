#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"
#include "debugdialog.h"
#include "reportdialog.h"
#include "configdialog.h"
#include "checkoutdialog.h"

#include "master.h"
#include "slavelist.h"
#include "workmode.h"
#include "slavedisplay.h"
#include "request.h"
#include "requestlist.h"
#include "convert.h"
#include "writedown.h"

#include <QString>
#include <QMessageBox>
#include <QWidget>
#include <QDateTime>
#include <QtNetwork>
#include <QUdpSocket>
#include <QVector>
#include <QDebug>
#include <QCloseEvent>

extern Master master;
extern SlaveList slaveList;
extern RequestList requestList;
extern float windUsage[4];
extern float windCost;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(100,70,800,680);
    initLabels();
    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setSingleShot(false);
    socket = new QUdpSocket;
    connect(timer,SIGNAL(timeout()),this,SLOT(updateFunction()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
    ui->labelAccount->setText(master.account);
    ui->labelMaxNum->setText(QString::number(master.maxNum));
    ui->labelNowNum->setText(QString::number(master.nowNum));
    ui->labelRefresh->setText(QString::number(master.refreshFreq));
    ui->labelWorkMode->setText(convertWorkModeToQStr(master.workMode));
    ui->labelStrategy->setText(convertStrategyToQStr(master.strategyMode));
    //debugDialog * debugdialog = new debugDialog();
    //debugdialog->show();
}


void MainWindow::updateFunction(){
    requestList.dealRequests();
    updateLabel();
    sendSlaves();
}

void MainWindow::sendSlaves(){
    QVector<Slave> * list = slaveList.getSalves();
    Slave * it = list->begin();
    while(it!=list->end()){
        if(it->connectionStatus == ConnectionStatus::connected){
            QString str = it->generateStr();
            QByteArray datagram = str.toLatin1();
            socket->writeDatagram(datagram,it->address,it->port);
        }
        it++;
    }
}

void MainWindow::updateLabel(){
    ui->labelTime->setText(QTime::currentTime().toString());
    QVector<Slave>* slaves = slaveList.getSalves();
    Slave * slave = slaves->begin();
    int i = 0;
    while(slave!=slaves->end()){
        slaveDisplayVector[i].updateLabel(* slave);
        ++slave;
        i++;
    }
    ui->labelNowNum->setText(QString::number(master.nowNum));
    if(master.isWorking){
        if(master.nowNum == 0){
            ui->labelWorkStatus->setText("待机");
        }
        else{
            ui->labelWorkStatus->setText("工作中");
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonPower_clicked()
{
    if(master.isWorking){
        master.isWorking = false;
        timer->stop();
        connect(socket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
        QVector<Slave> * list = slaveList.getSalves();
        QString response = "4";
        QByteArray datagram;
        datagram = response.toLatin1();
        Slave * it = list->begin();
        while(it!=list->end()){
            if(it->connectionStatus == ConnectionStatus::connected){
                socket->writeDatagram(datagram,it->address,it->port);
            }
            it++;
        }
        slaveList.stopAllSlaves();
        requestList.setAllRequestDealt();
        ui->labelWorkStatus->setText("关机");
        ui->buttonPower->setText("开机");
        updateLabel();
        changeColor(0);
    }
    else{
        master.isWorking = true;
        if(!isBinded){
            socket->bind(8888);
            isBinded = true;
        }
        timer->start();
        ui->buttonPower->setText("关机");
        if(master.workMode == WorkMode::Hot){
            changeColor(2);
        }
        else{
            changeColor(1);
        }
    }
}

void MainWindow::processPendingDatagrams(){
    while(socket->hasPendingDatagrams()){
        if(master.isWorking){
            try{
                QByteArray datagram;
                datagram.resize(socket->pendingDatagramSize());
                QHostAddress address;
                quint16 port = 0;
                socket->readDatagram(datagram.data(),datagram.size(), &address, &port);
                QString str = QString(datagram);
                qDebug()<<QTime::currentTime().toString()+" Incomming: "+str;
                QStringList data = str.split("+",QString::SkipEmptyParts);
                if(data.size()>0){
                    QString dataType = data[0];
                    QString response = "";
                    if(dataType.compare(dataType,"1")==0){
                        if(data.size()>=2){
                            QString numStr = data[1];
                            int num = numStr.toInt();
                            response = "1+";
                            if(num == 666){
                                port = 3344;
                            }
                            if(slaveList.registerSlave(num, address, port)==true){
                                response = response + "1+";
                            }
                            else{
                                response = response + "0+";
                            }
                            if(master.workMode == WorkMode::Cold){
                                response = response + "0+";
                            }
                            else{
                                response = response + "1+";
                            }
                            response = response + numStr;
                            QByteArray datagram;
                            datagram = response.toLatin1();
                            socket->writeDatagram(datagram,address,port);
                            writeDownPowerOn(num);
                        }
                    }
                    else if(dataType.compare(dataType,"2")==0){
                        if(data.size()>=5){
                            QString numStr = data[1];
                            int num = numStr.toInt();
                            QString workModeStr = data[2];
                            WorkMode workMode = WorkMode(workModeStr.toInt());
                            QString windStr = data[3];
                            Wind wind = Wind(windStr.toInt());
                            QString tempStr = data[4];
                            float temperature = tempStr.toFloat();
                            Slave * temp = slaveList.findSlave(num);
                            if(temp == nullptr || temp->connectionStatus == ConnectionStatus::disconnected || temp->connectionStatus == ConnectionStatus::lostconnect){
                                response = "2+0+"+numStr;
                            }
                            else if((workMode != master.workMode) ||
                                    (workMode == WorkMode::Cold && (temperature > 25 || temperature < 18)) ||
                                    (workMode == WorkMode::Hot && (temperature < 25 || temperature > 30))){
                                response = "2+0+"+numStr;
                            }
                            else{
                                Request request(num, master.workMode, wind, temperature, false, QDateTime::currentDateTime());
                                requestList.updateRequest(request);
                                response = "2+1+"+numStr;
                            }
                            QByteArray datagram;
                            datagram = response.toLatin1();
                            socket->writeDatagram(datagram,address,port);
                        }
                    }
                    else if(dataType.compare(dataType, "3") == 0){
                        if(data.size()>=4){
                            QString numStr = data[1];
                            int num = numStr.toInt();
                            QString targetTempStr = data[2];
                            float targetTemp = targetTempStr.toFloat();
                            QString nowTempStr = data[3];
                            float nowTemp = nowTempStr.toFloat();
                            slaveList.updateSlave(num, targetTemp, nowTemp, QDateTime::currentDateTime());
                        }
                    }
                    else if(dataType.compare(dataType, "4") == 0){
                        if(data.size()>=2){
                            QString numStr = data[1];
                            int num = numStr.toInt();
                            slaveList.logoffSlave(num);
                            writeDownPowerOff(num);
                        }

                    }
                    qDebug()<<QTime::currentTime().toString()+" Output: "+response;
                }
            }
            catch(...){
                qDebug()<<"Error when processing incoming data.";
            }
        }
        else{
            QByteArray datagram;
            datagram.resize(socket->pendingDatagramSize());
            QHostAddress address;
            quint16 port = 0;
            socket->readDatagram(datagram.data(),datagram.size(), &address, &port);
            QString str = QString(datagram);
            qDebug()<<QTime::currentTime().toString()+" Incomming: "+str;
        }
    }
}

void MainWindow::initLabels(){
    slaveDisplayVector.append(SlaveDisplay(ui->roomLabel_1, ui->statusLabel_1, ui->currentLabel_1, ui->targetLabel_1,ui->usageLabel_1,ui->windLabel_1));
    slaveDisplayVector.append(SlaveDisplay(ui->roomLabel_2, ui->statusLabel_2, ui->currentLabel_2, ui->targetLabel_2, ui->usageLabel_2,ui->windLabel_2));
    slaveDisplayVector.append(SlaveDisplay(ui->roomLabel_3, ui->statusLabel_3, ui->currentLabel_3, ui->targetLabel_3, ui->usageLabel_3,ui->windLabel_3));
    slaveDisplayVector.append(SlaveDisplay(ui->roomLabel_4, ui->statusLabel_4, ui->currentLabel_4, ui->targetLabel_4, ui->usageLabel_4,ui->windLabel_4));
    slaveDisplayVector.append(SlaveDisplay(ui->roomLabel_5, ui->statusLabel_5, ui->currentLabel_5, ui->targetLabel_5, ui->usageLabel_5,ui->windLabel_5));
    slaveDisplayVector.append(SlaveDisplay(ui->roomLabel_6, ui->statusLabel_6, ui->currentLabel_6, ui->targetLabel_6, ui->usageLabel_6,ui->windLabel_6));
}

void MainWindow::on_buttonReport_clicked()
{
    reportDialog * dialog = new reportDialog();
    dialog->show();
}

void MainWindow::closeEvent(QCloseEvent * event){
    if(master.isWorking){
        QMessageBox::warning(NULL, "提示","请先关机", QMessageBox::Yes | QMessageBox::Yes);
        event->ignore();
    }
    else if(QMessageBox::question(NULL, "退出","确认退出？\n您的配置将保存至本地。",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes){
        slaveList.saveToDatabase();
        saveSetting();
        event->accept();
    }
    else{
        event->ignore();
    }
}

void MainWindow::on_buttonSetting_clicked()
{
    configDialog * dialog = new configDialog(this);
    connect(dialog, SIGNAL(configChanged()), this, SLOT(configChanged()));
    dialog->show();
}

void MainWindow::on_buttonChekOut_clicked()
{
    checkOutDialog * dialog = new checkOutDialog();
    dialog->show();
}

void MainWindow::configChanged(){
    ui->labelWorkMode->setText(convertWorkModeToQStr(master.workMode));
    ui->labelRefresh->setText(QString::number(master.refreshFreq));
    ui->labelStrategy->setText(convertStrategyToQStr(master.strategyMode));
    if(master.isWorking){
        if(master.workMode == WorkMode::Hot){
            changeColor(2);
        }
        else{
            changeColor(1);
        }
    }
}

void MainWindow::changeColor(int color){
    QString styleSheetWord = "";
    switch(color){
    case 0: //off
        styleSheetWord = "background-color: rgb(55, 55, 55);\ncolor: rgb(255, 255, 255);";
        break;
    case 1: //cold
        styleSheetWord = "background-color: rgb(0, 108, 255);\ncolor: rgb(255, 255, 255);";
        break;
    case 2: //hot
        styleSheetWord = "background-color: rgb(255, 85, 0);\ncolor: rgb(255, 255, 255);";
    }
    ui->labelBG_2->setStyleSheet(styleSheetWord);
}

void MainWindow::saveSetting(){
    QFile text("config.ini");
    text.close();
    if(!text.open(QIODevice::Text|QIODevice::ReadWrite)){
        QMessageBox::warning(NULL, "提示","无法打开配置文件，请联系管理员。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QTextStream out(&text);
    try{
        out<<(int)master.workMode<<endl;
        out<<(int)master.strategyMode<<endl;
        out<<master.refreshFreq<<endl;
        out<<windCost<<endl;
        out<<windUsage[(int)Wind::low]<<endl;
        out<<windUsage[(int)Wind::medium]<<endl;
        out<<windUsage[(int)Wind::high]<<endl;
        out<<master.maxNum<<endl;
        text.close();
    }
    catch(...){
    }
}
