#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "mainwindow.h"
#include "defineres.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QRegExp>

extern QString account;

settingDialog::settingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingDialog)
{
    ui->setupUi(this);
    QRegExp regExpNum("[0-9]{16}");
    ui->lineEditID->setValidator(new QRegExpValidator(regExpNum,this));

}

settingDialog::~settingDialog()
{
    delete ui;
}

//修改密码
void settingDialog::on_buttonOk1_clicked()
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
    QSqlQuery query;
    query.exec("SELECT password FROM accountInfo WHERE id = '"+account+"'" );
    if(!query.next()){
        QMessageBox::warning(NULL, "错误","运行时错误。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QString oldPassword=query.value(0).toString();
    if(oldNum!=oldPassword){
        QMessageBox::warning(NULL, "错误","旧密码输入错误，请检查。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    query.exec("UPDATE accountInfo SET password='"+newNum1+"' WHERE id = '"+account+"'" );
    QMessageBox::information(NULL, "修改成功","修改成功，请记住您的新密码。", QMessageBox::Yes | QMessageBox::Yes);
    close();
}

void settingDialog::on_buttonCancel1_clicked()
{
    close();
}

void settingDialog::on_buttonCancel2_clicked()
{
    close();
}

/* 绑定银行卡，根据银行返回信息决定是否绑定成功
 * 绑定成功则将信息写入card数据库 */
void settingDialog::on_buttonOk2_clicked()
{
    QSqlQuery query;
    if(ui->lineEditID->text().isEmpty() || ui->lineEditPassword->text().isEmpty()){
        QMessageBox::warning(NULL, "注意","账号或密码未填写。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    query.exec("SELECT * FROM card"+account+" WHERE num='"+ui->lineEditID->text()+"'");
    if(query.next()){
        QMessageBox::warning(NULL, "注意","您已经绑定过该银行卡，可用于购物。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QString str="ID:"+ui->lineEditID->text()+";PW:"+ui->lineEditPassword->text();
    int res=MainWindow::askBank(0,str);
    if(res==SUCCESS){
        query.exec("INSERT INTO card"+account+" (num) VALUES ('"+ui->lineEditID->text()+"')");
        QMessageBox::information(NULL, "完成","银行卡已绑定。", QMessageBox::Yes | QMessageBox::Yes);
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
    else if(res == NOID){
        QMessageBox::warning(NULL, "注意","银行卡账号不存在。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    else{
        QMessageBox::warning(NULL, "注意","密码错误。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
}
