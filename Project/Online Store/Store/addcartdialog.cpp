#include "addcartdialog.h"
#include "ui_addcartdialog.h"
#include "type.h"

#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QtSql>
#include <QString>

/* “添加购物车”界面 */

//全局变量 account是账户名，cartnum是购物车数量
extern QString account;
extern int cartnum;

addCartDialog::addCartDialog(QWidget *parent,int index,int type) :
    QDialog(parent),
    ui(new Ui::addCartDialog)
{
    num=index;
    this->type=type;
    ui->setupUi(this);
    QSqlQuery query;
    switch(type){
        case 0:
            query.exec("SELECT * FROM productInfo");
            break;
        case BOOK:
            query.exec("SELECT * FROM productInfo WHERE type = 'Book'");
            break;
        case FOOD:
            query.exec("SELECT * FROM productInfo WHERE type = 'Food'");
            break;
        case CLOTH:
            query.exec("SELECT * FROM productInfo WHERE type = 'Cloth'");
            break;
        case ELECTRO:
            query.exec("SELECT * FROM productInfo WHERE type = 'Electro'");
            break;
        default:
            query.exec("SELECT * FROM productInfo");
            break;
    }
    for(int i=0;i<num;i++)
        query.next();
    ui->labelName->setText(query.value(1).toString());
    ui->labelType->setText(query.value(2).toString());
    ui->labelFullPrice->setText(QString::number(query.value(4).toFloat(),'f',2));
    ui->labelStock->setText(query.value(5).toString());
    ui->labelRate->setText(QString::number(query.value(6).toFloat(),'f',2));
    ui->spinBoxNum->setMaximum(query.value(5).toInt());
    ui->spinBoxNum->setMinimum(1);
}

addCartDialog::~addCartDialog()
{
    delete ui;
}

/* 当点击“确认”按钮时，按照当前所显示的物品种类
 * 来找到对应物品并添加至购物车 */
void addCartDialog::on_buttonOk_clicked()
{
    QSqlQuery query;
    switch(this->type){
        case 0:
            query.exec("SELECT * FROM productInfo");
            break;
        case BOOK:
            query.exec("SELECT * FROM productInfo WHERE type = 'Book'");
            break;
        case FOOD:
            query.exec("SELECT * FROM productInfo WHERE type = 'Food'");
            break;
        case CLOTH:
            query.exec("SELECT * FROM productInfo WHERE type = 'Cloth'");
            break;
        case ELECTRO:
            query.exec("SELECT * FROM productInfo WHERE type = 'Electro'");
            break;
        default:
            query.exec("SELECT * FROM productInfo");
            break;
    }
    for(int i=0;i<num;i++)
        query.next();

    QString name=query.value(1).toString();
    QString type=query.value(2).toString();
    float price=query.value(4).toFloat();
    int stock=query.value(5).toInt();
    int quantity=ui->spinBoxNum->value();

    //查看商品存货是否满足购物车数量
    query.exec("SELECT * FROM cart"+account+" WHERE name='"+name+"'");
    if(query.next()){
        quantity+=query.value(5).toInt();
        if(quantity>stock){
             QMessageBox::warning(NULL, "错误","商品余量不足，请更改购买数量。", QMessageBox::Yes | QMessageBox::Yes);
             return;
        }
        else{
            query.exec("UPDATE cart"+account+" SET quantity="+QString::number(quantity)+" WHERE name='"+name+"'");
        }
    }
    else{
        //将商品添加至购物车，写入数据库
        query.exec("INSERT INTO cart"+account+" (name, num,type,price,quantity) VALUES( '"+name+"','"+QString::number(num-1)+"','"+type+"',"+QString::number(price)+","+QString::number(quantity)+")" );
        cartnum++;
        query.exec("UPDATE accountInfo SET cartnum="+QString::number(cartnum)+" WHERE id='"+account+"'");
    }
    QMessageBox::information(NULL, "成功","加入购物车成功。", QMessageBox::Yes | QMessageBox::Yes);
    close();
}

void addCartDialog::on_buttonCancel_clicked()
{
    close();
}
