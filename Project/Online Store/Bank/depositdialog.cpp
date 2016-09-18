#include "depositdialog.h"
#include "ui_depositdialog.h"
#include "account.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QRegExp>
#include <QtGui>

/* “取款”界面 */

//全局变量 当前登录账户的账户信息
extern account * accountPtr;

depositDialog::depositDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::depositDialog)
{
    ui->setupUi(this);

    QSqlQuery query;
    QRegExp regExp("[0-9]{1,4}[\.][0-9]{2}");
    ui->lineEdit->setValidator(new QRegExpValidator(regExp,this));
}

depositDialog::~depositDialog()
{
    delete ui;
}

/* 当“确认”按钮按下时，检查金额是否正确，正确就进行存款 */
void depositDialog::on_okButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
        QMessageBox::warning(NULL, "提示","请填写金额。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    float deposit = ui->lineEdit->text().toFloat();
    QMessageBox::information(NULL, "提示","存款成功。", QMessageBox::Yes | QMessageBox::Yes);
    float balance = accountPtr->getBalance()+deposit;
    accountPtr->addBalance(deposit);
    QString num=accountPtr->getNum();
    QSqlQuery query;
    query.exec("UPDATE accountInfo SET balance = "+QString ::number(balance,'f',2)+" WHERE num = '"+num+"'" );
    close();
}

void depositDialog::on_buttonCancel_clicked()
{
    close();
}
