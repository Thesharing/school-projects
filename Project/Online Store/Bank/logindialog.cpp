#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regdialog.h"
#include "account.h"
#include "configdialog.h"

#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QRegExp>
#include <QString>
#include <QMessageBox>

/* “登录”界面 */

/* isLogged，当登陆完成以后isLogged为true
 * 当登录失败时会直接退出程序而非进入主界面
 * 而accountPtr是账户信息 */
extern bool isLogged;
extern account * accountPtr;

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
    QRegExp regExpNum("[0-9]{16}");
    ui->lineEditAccount->setValidator(new QRegExpValidator(regExpNum,this));

}

loginDialog::~loginDialog()
{
    delete ui;
}

//当按下“登录”按钮时，检查信息是否正确，正确则将信息保存至accountPtr，进入主界面
void loginDialog::on_buttonLogin_clicked()
{
    QString idInput=ui->lineEditAccount->text();
    QString passwordInput=ui->lineEditPassword->text();
    QSqlQuery query;
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("银行管理系统"));
    msgBox.setText(tr("空。"));
    msgBox.setStandardButtons(QMessageBox::Yes);
    if(idInput=="" || passwordInput == ""){
        msgBox.setText(tr("请输入账号和密码。"));
        msgBox.exec();
        return;
    }
    query.exec("SELECT * FROM accountInfo WHERE num ='"+idInput+"'" );
    if(query.next()){
        //登录成功，将信息保存到accountPtr中
        if(query.value(1)==passwordInput){
            msgBox.setText(tr("登录成功。"));
            isLogged=true;
            msgBox.exec();
            QString num=query.value(0).toString();
            QString password=query.value(1).toString();
            QString bank=query.value(2).toString();
            QString id=query.value(3).toString();
            float balance=query.value(4).toFloat();
            QString contact=query.value(5).toString();
            accountPtr = new account (num,password,bank,id,contact,balance);
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

//“注册”按钮，打开注册界面
void loginDialog::on_buttonReg_clicked()
{
    regDialog * regdialog=new regDialog(this);
    regdialog->exec();
}

//“清空”按钮
void loginDialog::on_buttonClear_clicked()
{
    ui->lineEditAccount->setText("");
    ui->lineEditPassword->setText("");
}

//“设置”按钮
void loginDialog::on_buttonSetting_clicked()
{
    configDialog * configdialog = new configDialog(this);
    configdialog->exec();
}
