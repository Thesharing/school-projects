#include "regaccountdialog.h"
#include "ui_regaccountdialog.h"

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QMessageBox>

regAccountDialog::regAccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::regAccountDialog)
{
    ui->setupUi(this);
}

regAccountDialog::~regAccountDialog()
{
    delete ui;
}

void regAccountDialog::on_buttonOk_clicked()
{
    if(ui->lineEditAccount->text().isEmpty() || ui->lineEditPassword->text().isEmpty()){
        QMessageBox::warning(NULL, "错误","信息未填写完整，请检查。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QString account = ui->lineEditAccount->text();
    QSqlQuery query;
    query.exec("SELECT * FROM accountInfo WHERE account = '"+account+"'");
    if(query.next()){
        QMessageBox::warning(NULL, "错误","账号已注册。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QString password = ui->lineEditPassword->text();
    query.exec("INSERT INTO accountInfo (account, password) VALUES( '"+account+"','"+password+"')");
    QMessageBox::information(NULL, "注册成功","注册成功，请登录。", QMessageBox::Yes | QMessageBox::Yes);
    this->close();
}

void regAccountDialog::on_buttonCancel_clicked()
{
    close();
}
