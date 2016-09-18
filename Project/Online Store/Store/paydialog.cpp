#include "paydialog.h"
#include "ui_paydialog.h"
#include "mainwindow.h"
#include "defineres.h"

#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>

extern bool isPayed;
extern QString account;

payDialog::payDialog(QWidget *parent, int num, float total) :
    QDialog(parent),
    ui(new Ui::payDialog)
{
    ui->setupUi(this);
    isPayed = false;
    this->num=num;
    this->total=total;
    ui->labelNum->setText("此次您一共购买了"+QString::number(this->num)+"种商品。" );
    ui->labelTotal->setText("共计"+QString::number(this->total,'f',2)+"元。");
    QSqlQuery query;
    query.exec("SELECT * FROM card"+account);
    if(!query.next()){
        ui->labelCard->setText("您需要先绑定一张银行卡。");
        ui->buttonOk->setDisabled(true);
        ui->lineEditPassword->setDisabled(true);
        ui->comboBoxCard->setDisabled(true);
    }
    else{
        query.exec("SELECT * FROM card"+account);
        while(query.next()){
            ui->comboBoxCard->addItem(query.value(0).toString());
        }
    }
}

payDialog::~payDialog()
{
    delete ui;
}

//MainWindow.cpp下是建立连接的函数askBank
//建立连接以后通信，根据银行返回值确定支付结果
void payDialog::on_buttonOk_clicked()
{
    QString str = "PY:"+QString::number(this->total)+";ID:"+account+";PW:"+ui->lineEditPassword->text();
    int res=MainWindow::askBank(PAYMENT,str);
    if(res==SUCCESS){
        QMessageBox::information(NULL, "完成","支付成功，您的订单将于近日发货。", QMessageBox::Yes | QMessageBox::Yes);
        isPayed=true;
        close();
    }
    else if(res == NOSERVER){
        QMessageBox::warning(NULL, "注意","未连接到服务器，请稍后重试。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    else if(res == LOSTCONNECTION){
        QMessageBox::warning(NULL, "注意","与服务器失去连接，请重试。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    else if(res == LACKMONEY){
        QMessageBox::warning(NULL, "注意","您的账户余额不足，支付失败。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    else{
        QMessageBox::warning(NULL, "注意","密码错误，请重试。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
}

void payDialog::on_buttonCancel_clicked()
{
    close();
}
