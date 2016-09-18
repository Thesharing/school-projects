#include "logindialog.h"
#include "ui_logindialog.h"
#include "regdialog.h"
#include "configdialog.h"

#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QRegExp>
#include <QString>
#include <QMessageBox>

extern bool isLogged;
extern QString account;
extern int cartnum;

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::on_buttonReg_clicked()
{
    regDialog * regdialog = new regDialog(this);
    regdialog->exec();
}

void loginDialog::on_buttonClear_clicked()
{
    ui->lineEditAccount->setText("");
    ui->lineEditPassword->setText("");
}

void loginDialog::on_buttonLogin_clicked()
{
    QString idInput=ui->lineEditAccount->text();
    QString passwordInput=ui->lineEditPassword->text();
    QSqlQuery query;
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("电子商务平台"));
    msgBox.setText(tr("空。"));
    msgBox.setStandardButtons(QMessageBox::Yes);
    if(idInput=="" || passwordInput == ""){
        msgBox.setText(tr("请输入账号和密码。"));
        msgBox.exec();
        return;
    }
    query.exec("SELECT * FROM accountInfo WHERE id ='"+idInput+"'" );
    if(query.next()){
        if(query.value(1)==passwordInput){
            //msgBox.setText(tr("登录成功。"));
            isLogged=true;
            //msgBox.exec();
            account=query.value(0).toString();
            cartnum=query.value(2).toInt();
            close();
        }
        else{
            msgBox.setText(tr("密码错误。"));
            msgBox.exec();
            ui->lineEditPassword->setText("");
        }
    }
    else{
        msgBox.setText(tr("未找到该用户。"));
        msgBox.exec();
    }
}

void loginDialog::on_buttonSetting_clicked()
{
    configDialog * configdialog = new configDialog(this);
    configdialog->exec();
}
