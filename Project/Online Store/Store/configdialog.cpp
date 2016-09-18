#include "configdialog.h"
#include "ui_configdialog.h"

#include <QRegExp>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

/* 管理员后台修改满减活动、打折等优惠 */

extern bool isEvent;
extern int eventReq;
extern int eventFav;

configDialog::configDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configDialog)
{
    ui->setupUi(this);
    if(isEvent){
        ui->checkBoxMJ->setChecked(true);
    }
    else{
        ui->checkBoxMJ->setChecked(false);
    }
    QRegExp regExpNum("[0-9]{16}");
    ui->lineEditMJ1->setValidator(new QRegExpValidator(regExpNum,this));
    ui->lineEditMJ2->setValidator(new QRegExpValidator(regExpNum,this));
    QRegExp regExpNum2("[0-9]\.[0-9]{2}");
    ui->lineEditBook->setValidator(new QRegExpValidator(regExpNum2,this));
    ui->lineEditCloth->setValidator(new QRegExpValidator(regExpNum2,this));
    ui->lineEditFood->setValidator(new QRegExpValidator(regExpNum2,this));
    ui->lineEditElectro->setValidator(new QRegExpValidator(regExpNum2,this));

    ui->lineEditMJ1->setText(QString::number(eventReq));
    ui->lineEditMJ2->setText(QString::number(eventFav));
    QSqlQuery query;
    query.exec("SELECT rate FROM productInfo WHERE type = 'Book'");
    if(query.next()){
        rateBook=query.value(0).toFloat();
    }
    query.exec("SELECT rate FROM productInfo WHERE type = 'Cloth'");
    if(query.next()){
        rateCloth=query.value(0).toFloat();
    }
    query.exec("SELECT rate FROM productInfo WHERE type = 'Food'");
    if(query.next()){
        rateFood=query.value(0).toFloat();
    }
    query.exec("SELECT rate FROM productInfo WHERE type = 'Electro'");
    if(query.next()){
        rateElectro=query.value(0).toFloat();
    }
    ui->lineEditBook->setText(QString::number(rateBook,'f',2));
    ui->lineEditCloth->setText(QString::number(rateCloth,'f',2));
    ui->lineEditFood->setText(QString::number(rateFood,'f',2));
    ui->lineEditElectro->setText(QString::number(rateElectro,'f',2));
}

configDialog::~configDialog()
{
    delete ui;
}

void configDialog::on_pushButton_clicked()
{
    if(ui->lineEditMJ1->text().isEmpty() || ui->lineEditMJ2->text().isEmpty() || ui->lineEditBook->text().isEmpty() || ui->lineEditCloth->text().isEmpty() || ui->lineEditFood->text().isEmpty() || ui->lineEditElectro->text().isEmpty()){
        QMessageBox::warning(NULL, "提示","请填写全部信息。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    int mj1=ui->lineEditMJ1->text().toInt();
    int mj2=ui->lineEditMJ2->text().toInt();
    if(mj1<mj2){
        QMessageBox::warning(NULL, "提示","满减金额填写错误。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    if(ui->checkBoxMJ->isChecked()){
        isEvent=true;
    }
    else{
        isEvent=false;
    }
    eventReq=mj1;
    eventFav=mj2;
    QSqlQuery query;
    rateBook=ui->lineEditBook->text().toFloat();
    query.exec("UPDATE productInfo SET rate = "+QString::number(rateBook,'f',2)+" where type = 'Book'");
    rateCloth=ui->lineEditCloth->text().toFloat();
    query.exec("UPDATE productInfo SET rate = "+QString::number(rateCloth,'f',2)+" where type = 'Cloth'");
    rateFood=ui->lineEditFood->text().toFloat();
    query.exec("UPDATE productInfo SET rate = "+QString::number(rateFood,'f',2)+" where type = 'Food'");
    rateElectro=ui->lineEditElectro->text().toFloat();
    query.exec("UPDATE productInfo SET rate = "+QString::number(rateElectro,'f',2)+" where type = 'Electro'");
    QFile text("store.ini");
    text.close();
    if(!text.open(QIODevice::Text|QIODevice::ReadWrite)){
        QMessageBox::warning(NULL, "提示","无法打开配置文件，请联系管理员。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QTextStream out(&text);
    out<<(int)isEvent<<endl;
    out<<eventReq<<endl;
    out<<eventFav<<endl;
    close();
}

void configDialog::on_pushButton_2_clicked()
{
    close();
}
