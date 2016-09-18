#include "configdialog.h"
#include "ui_configdialog.h"

#include "master.h"
#include "wind.h"
#include "slavelist.h"
#include "requestlist.h"

#include <QRegExp>
#include <QMessageBox>

extern Master master;
extern bool isInitialed;
extern SlaveList slaveList;
extern RequestList requestList;
extern float windUsage[4];
extern float windCost;

configDialog::configDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configDialog)
{
    ui->setupUi(this);
    QRegExp regExpFreq("[0-9]{1,3}[\.][0-9]{2}");
    QRegExp regExpFreq2("[1-3]");
    ui->lineEditFreq->setValidator(new QRegExpValidator(regExpFreq2,this));
    ui->lineEditCost->setValidator(new QRegExpValidator(regExpFreq,this));
    ui->lineEditHigh->setValidator(new QRegExpValidator(regExpFreq,this));
    ui->lineEditMedium->setValidator(new QRegExpValidator(regExpFreq,this));
    ui->lineEditLow->setValidator(new QRegExpValidator(regExpFreq,this));
    ui->lineEditFreq->setText(QString::number(master.refreshFreq));
    ui->lineEditCost->setText(QString::number(windCost));
    ui->lineEditLow->setText(QString::number(windUsage[(int)Wind::low]));
    ui->lineEditMedium->setText(QString::number(windUsage[(int)Wind::medium]));
    ui->lineEditHigh->setText(QString::number(windUsage[(int)Wind::high]));
    if(master.workMode == WorkMode::Cold){
        ui->buttonCold->setEnabled(false);
        ui->buttonCold->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 108, 255);\nborder-style: solid");
        isCold = true;
    }
    else{
        ui->buttonHot->setEnabled(false);
        ui->buttonHot->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(225, 85, 0);\nborder-style: solid");
        isCold = false;
    }
    if(master.strategyMode == Strategy::FIFS){
        ui->buttonFIFS->setEnabled(false);
        ui->buttonFIFS->setStyleSheet("color: rgb(255, 204, 1);\nbackground-color: rgb(55, 55, 55);\nborder-style: solid");
        isFIFS = true;
    }
    else{
        ui->buttonHP->setEnabled(false);
        ui->buttonHP->setStyleSheet("color: rgb(255, 204, 1);\nbackground-color: rgb(55, 55, 55);\nborder-style: solid");
        isFIFS = false;
    }
}

configDialog::~configDialog()
{
    delete ui;

}

void configDialog::closeEvent(QCloseEvent *){
    this->reject();
}

void configDialog::on_buttonOk_clicked()
{
    float refreshFreq = 0;
    float cost = 0;
    float highWind = 0;
    float mediumWind = 0;
    float lowWind = 0;
    if(ui->lineEditFreq->text().isEmpty() || ui->lineEditCost->text().isEmpty() || ui->lineEditHigh->text().isEmpty()
            || ui->lineEditMedium->text().isEmpty() || ui->lineEditLow->text().isEmpty()){
        QMessageBox::warning(NULL, "提示","设置不能为空。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    refreshFreq = ui->lineEditFreq->text().toFloat();
    cost = ui->lineEditCost->text().toFloat();
    highWind = ui->lineEditHigh->text().toFloat();
    mediumWind = ui->lineEditMedium->text().toFloat();
    lowWind = ui->lineEditLow->text().toFloat();
    if(refreshFreq == 0 || cost ==0 || highWind ==0 || mediumWind ==0 || lowWind==0){
        QMessageBox::warning(NULL, "提示","设置不能为0。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    if(isInitialed){
        if((isCold && master.workMode == WorkMode::Hot) || (!isCold && master.workMode == WorkMode::Cold)){
            if(QMessageBox::question(NULL, "提示","您将改变工作模式，这需要先进入待机状态，是否继续？",
                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes){
                slaveList.stopAllSlaves();
                requestList.setAllRequestDealt();
            }
            else{
                return;
            }
        }
    }
    if(isCold){
        master.workMode = WorkMode::Cold;
    }
    else{
        master.workMode = WorkMode::Hot;
    }
    if(isFIFS){
        master.strategyMode = Strategy::FIFS;
    }
    else{
        master.strategyMode = Strategy::HP;
    }
    master.refreshFreq = refreshFreq;
    windCost = cost;
    windUsage[(int)Wind::low] = lowWind;
    windUsage[(int)Wind::medium] = mediumWind;
    windUsage[(int)Wind::high] = highWind;
    if(!isInitialed){
        isInitialed = true;
    }
    this->accept();
}

void configDialog::on_buttonCancel_clicked()
{
    this->reject();
}

void configDialog::on_buttonCold_clicked()
{
   ui->buttonCold->setEnabled(false);
   ui->buttonCold->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 108, 255);\nborder-style: solid");
   ui->buttonHot->setStyleSheet("");
   ui->buttonHot->setEnabled(true);
   isCold = true;
}

void configDialog::on_buttonHot_clicked()
{
   ui->buttonHot->setEnabled(false);
   ui->buttonHot->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(255, 85, 0);\nborder-style: solid");
   ui->buttonCold->setStyleSheet("");
   ui->buttonCold->setEnabled(true);
   isCold = false;
}

void configDialog::on_buttonFIFS_clicked()
{
    ui->buttonFIFS->setEnabled(false);
    ui->buttonFIFS->setStyleSheet("color: rgb(255, 204, 1);\nbackground-color: rgb(55, 55, 55);\nborder-style: solid");
    ui->buttonHP->setStyleSheet("");
    ui->buttonHP->setEnabled(true);
    isFIFS = true;
}

void configDialog::on_buttonHP_clicked()
{
    ui->buttonFIFS->setEnabled(true);
    ui->buttonHP->setStyleSheet("color: rgb(255, 204, 1);\nbackground-color: rgb(55, 55, 55);\nborder-style: solid");
    ui->buttonFIFS->setStyleSheet("");
    ui->buttonHP->setEnabled(false);
    isFIFS = false;
}

void configDialog::accept(){
    emit configChanged();
    QDialog::accept();
}


