#include "editdialog.h"
#include "ui_editdialog.h"
#include "account.h"

#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>

/* “修改密码”界面 */

//全局变量 当前登录账户的账户信息
extern account* accountPtr;

editDialog::editDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editDialog)
{
    ui->setupUi(this);
}

editDialog::~editDialog()
{
    delete ui;
}

/* 当“确认”按钮被按下时，检查信息是否正确，正确就修改账户信息中的密码 */
void editDialog::on_buttonOk_clicked()
{
    if(ui->lineEditNow1->text().isEmpty()||ui->lineEditNow2->text().isEmpty()||ui->lineEditOri->text().isEmpty()){
        QMessageBox::warning(NULL, "错误","请输入完整信息。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QString newNum1=ui->lineEditNow1->text();
    QString newNum2=ui->lineEditNow2->text();
    QString oldNum=ui->lineEditOri->text();
    if(newNum1 != newNum2){
        QMessageBox::warning(NULL, "错误","两遍输入的新密码不同，请检查。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QString num=accountPtr->getNum();
    QSqlQuery query;
    query.exec("SELECT password FROM accountInfo WHERE num = '"+num+"'" );
    if(!query.next()){
        QMessageBox::warning(NULL, "错误","运行时错误。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QString oldPassword=query.value(0).toString();
    if(oldNum!=oldPassword){
        QMessageBox::warning(NULL, "错误","旧密码输入错误，请检查。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    query.exec("UPDATE accountInfo SET password='"+newNum1+"' WHERE num = '"+num+"'" );
    QMessageBox::information(NULL, "修改成功","修改成功，请记住您的新密码。", QMessageBox::Yes | QMessageBox::Yes);
    close();
}

void editDialog::on_buttonCancel_clicked()
{
    close();
}
