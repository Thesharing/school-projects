#include "regdialog.h"
#include "ui_regdialog.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QRegExp>
#include <QtGui>

regDialog::regDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::regDialog)
{
    ui->setupUi(this);
}

regDialog::~regDialog()
{
    delete ui;
}

void regDialog::on_buttonClear_clicked()
{
    ui->lineEditId->setText("");
    ui->lineEditPassword->setText("");
}

/* 注册成功以后，将为该账户建立两个数据库
 * 一个是cart，对应该账户的购物车
 * 另一个是card，对应该账户绑定的银行卡
 * cart里保存id，商品序号
 * name，商品名称 num，商品在store结构里的序号
 * type，商品类型 price，商品价格
 * quantity，购买数量
 * 而card里面只保存银行卡账号 */
void regDialog::on_buttonSubmit_clicked()
{
    if(ui->lineEditId->text().isEmpty() || ui->lineEditPassword->text().isEmpty()){
        QMessageBox::warning(NULL, "错误","信息未填写完整，请检查。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QString id = ui->lineEditId->text();
    QSqlQuery query;
    query.exec("SELECT * FROM accountInfo WHERE id = '"+id+"'");
    if(query.next()){
        QMessageBox::warning(NULL, "错误"," 卡号已注册。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QString password=ui->lineEditPassword->text();
    query.exec("INSERT INTO accountInfo (id, password,cartnum) VALUES( '"+id+"','"+password+"',"+QString::number(0)+")" );
    QMessageBox::information(NULL, "注册成功","注册成功，请登录。", QMessageBox::Yes | QMessageBox::Yes);
    query.exec("CREATE TABLE cart"+id+" ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name NVARCHAR,"
                "num INT,"
                "type NVARCHAR,"
                "price REAL,"
                "quantity INT)"
                );
    query.exec("CREATE TABLE card"+id+" ("
               "num NVARCHAR PRIMARY KEY)"
                );
    close();
}

void regDialog::on_buttonCancel_clicked()
{
    close();
}
