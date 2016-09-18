#include "withdrawdialog.h"
#include "ui_withdrawdialog.h"
#include "account.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QRegExp>
#include <QtGui>

/* 取款界面 */

//账户信息
extern account * accountPtr;

withdrawDialog::withdrawDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::withdrawDialog)
{
    ui->setupUi(this);
    QSqlQuery query;
    QRegExp regExp("[0-9]{1,4}[\.][0-9]{2}");
    ui->lineEdit->setValidator(new QRegExpValidator(regExp,this));
}

withdrawDialog::~withdrawDialog()
{
    delete ui;
}

/* 当点击“确定”按钮时，检查余额是否足够取款，足够则进行取款 */
void withdrawDialog::on_okButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
        QMessageBox::warning(NULL, "错误","请填写金额。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    float deposit = ui->lineEdit->text().toFloat();
    if(deposit > accountPtr->getBalance()){
        QMessageBox::warning(NULL, "错误","您的余额不足。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QMessageBox::information(NULL, "提示","取款成功。", QMessageBox::Yes | QMessageBox::Yes);
    float balance = accountPtr->getBalance()-deposit;
    accountPtr->subBalance(deposit);
    QString num=accountPtr->getNum();
    QSqlQuery query;
    query.exec("UPDATE accountInfo SET balance = "+QString ::number(balance,'f',2)+" WHERE num = '"+num+"'" );
    close();
}

void withdrawDialog::on_cancelButton_clicked()
{
    close();
}
