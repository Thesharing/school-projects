#include "logindialog.h"
#include "ui_logindialog.h"
#include "regaccountdialog.h"
#include "master.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QString>

extern Master master;

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
    regAccountDialog * dialog = new regAccountDialog(this);
    dialog->exec();
}

void loginDialog::on_buttonOk_clicked()
{
    if(ui->lineEditAccount->text().isEmpty() || ui->lineEditPassword->text().isEmpty()){
        QMessageBox::warning(NULL, "错误","信息未填写完整，请检查。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QSqlQuery query;
    query.exec();
    QString accountText = ui->lineEditAccount->text();
    QString passwordText = ui->lineEditPassword->text();
    query.exec("SELECT password FROM accountInfo WHERE account ='"+accountText+"'");
    if(query.next()){
        if(query.value(0)==passwordText){
            QMessageBox::information(NULL, "提示","登录成功。", QMessageBox::Yes | QMessageBox::Yes);
            master.account = accountText;
            this->accept();
        }
        else{
            QMessageBox::warning(NULL, "错误","密码不正确，请检查。", QMessageBox::Yes | QMessageBox::Yes);
        }
    }
    else{
        QMessageBox::warning(NULL, "错误","账号未注册，请检查。", QMessageBox::Yes | QMessageBox::Yes);
    }
}

void loginDialog::on_buttonCancel_clicked()
{
    this->reject();
}

void loginDialog::closeEvent(QCloseEvent *){
    this->reject();
}
