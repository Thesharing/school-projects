#include "carteditdialog.h"
#include "ui_carteditdialog.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QString>
#include <QMessageBox>

/* 编辑购物车 */

extern int cartnum;
extern QString account;

//初始化编辑界面，设定元素属性
cartEditDialog::cartEditDialog(QWidget *parent,int num) :
    QDialog(parent),
    ui(new Ui::cartEditDialog)
{
    ui->setupUi(this);
    QSqlQuery query;
    query.exec("SELECT * FROM cart"+account);
    for(int i=0;i<num;i++)
        query.next();
    this->name = query.value(1).toString();
    QString type = query.value(3).toString();
    QString price = QString::number(query.value(4).toFloat(),'f',2);
    int quantity = query.value(5).toInt();
    query.exec("SELECT stock FROM productInfo WHERE name = '"+name+"'");
    int stock;
    if(query.next()){
        stock=query.value(0).toInt();
    }
    else{
        stock=0;
    }
    if(stock==0){
        ui->spinBox->setValue(0);
        ui->spinBox->setMaximum(0);
        ui->spinBox->setMinimum(0);
        query.exec("DELETE FROM cart"+account+" WHERE name = '"+this->name+"'");
        cartnum--;
        query.exec("UPDATE accountInfo SET cartnum="+QString::number(cartnum)+" WHERE id='"+account+"'");
        ui->buttonOk->setVisible(false);
        ui->buttonDelete->setVisible(false);
    }
    else{
        ui->spinBox->setValue(quantity);
        ui->spinBox->setMaximum(stock);
        ui->spinBox->setMinimum(1);
    }
    ui->labelName->setText(name);
    ui->labelType->setText(type);
    ui->labelCount->setText(QString::number(stock));
    ui->labelPrice->setText(price);
}

cartEditDialog::~cartEditDialog()
{
    delete ui;
}

//从购物车删除某件商品
void cartEditDialog::on_buttonDelete_clicked()
{
    QSqlQuery query;
    query.exec("DELETE FROM cart"+account+" WHERE name = '"+this->name+"'");
    QMessageBox::information(NULL, "删除成功","已从购物车中删除该商品。", QMessageBox::Yes | QMessageBox::Yes);
    cartnum--;
    query.exec("UPDATE accountInfo SET cartnum="+QString::number(cartnum)+" WHERE id='"+account+"'");
    close();
}

//将修改后的信息写入购物车中
void cartEditDialog::on_buttonOk_clicked()
{
    int quantity=ui->spinBox->value();
    QSqlQuery query;
    query.exec("UPDATE cart"+account+" SET quantity="+QString::number(quantity)+" WHERE name='"+name+"'");
    close();
}

void cartEditDialog::on_buttonCancel_clicked()
{
    close();
}
