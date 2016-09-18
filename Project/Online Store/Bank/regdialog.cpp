#include "regdialog.h"
#include "ui_regdialog.h"
#include "account.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QRegExp>
#include <QtGui>

/* “注册”界面 */

//账户信息
extern account * accountPtr;


regDialog::regDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::regDialog)
{
    ui->setupUi(this);
    QSqlQuery query;
    query.exec("SELECT name FROM bankInfo" );
    QString bankName;
    while(query.next()){
        bankName=query.value(0).toString();
        ui->comboBoxbank->addItem(bankName);
    }
    QRegExp regExpNum("[0-9]{16}");
    QRegExp regExpId("[0-9]{17}[0-9A-Za-z]");
    QRegExp regExpContact("[0-9]{13}");
    ui->lineEditNum->setValidator(new QRegExpValidator(regExpNum,this));
    ui->lineEditId->setValidator(new QRegExpValidator(regExpId,this));
    ui->lineEditContact->setValidator(new QRegExpValidator(regExpContact,this));
}

regDialog::~regDialog()
{
    delete ui;
}

/* 当点击“提交”时，检查信息是否正确，
 * 正确则将账户信息写入数据库，回到“登录”界面 */
void regDialog::on_buttonSubmit_clicked()
{
    if(ui->lineEditContact->text().isEmpty() || ui->lineEditId->text().isEmpty() || ui->lineEditNum->text().isEmpty() || ui->lineEditPassword->text().isEmpty()){
        QMessageBox::warning(NULL, "错误","信息未填写完整，请检查。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QString num=ui->lineEditNum->text();
    QSqlQuery query;
    query.exec("SELECT * FROM accountInfo WHERE num = '"+num+"'" );
    if(query.next()){
       QMessageBox::warning(NULL, "错误"," 卡号已注册。", QMessageBox::Yes | QMessageBox::Yes);
       return;
    }
    QString password=ui->lineEditPassword->text();
    QString bank=ui->comboBoxbank->currentText();
    QString id=ui->lineEditId->text();
    QString balance=QString::number(0);
    QString contact=ui->lineEditContact->text();
    query.exec("INSERT INTO accountInfo (num, password, bank, id, balance, contact) VALUES( '"+num+"','"+password+"','"+bank+"','"+id+"','"+balance+"','"+contact+"')" );
    QMessageBox::information(NULL, "注册成功","注册成功，请登录。", QMessageBox::Yes | QMessageBox::Yes);
    close();
}

//“清空”按钮
void regDialog::on_buttonClear_clicked()
{
    ui->lineEditContact->setText("");
    ui->lineEditId->setText("");
    ui->lineEditNum->setText("");
    ui->lineEditPassword->setText("");
}

//“取消”按钮
void regDialog::on_buttonCancel_clicked()
{
    close();
}
