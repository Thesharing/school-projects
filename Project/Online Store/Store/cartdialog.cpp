#include "cartdialog.h"
#include "ui_cartdialog.h"
#include "datastruct.h"
#include "carteditdialog.h"
#include "paydialog.h"

#include <QSqlQuery>
#include <QMessageBox>

/* 购物车界面 */

/* account是账户名
 * cartnum是购物车数量
 * store是商品信息
 * isEvent是是否有满减活动
 * eventReq和eventFav是满减金额 */
extern QString account;
extern int cartnum;
extern struct Store store;
extern bool isEvent;
extern int eventReq;
extern int eventFav;

//stringToNum用以将从数据库读取到的商品类型字符串转换成int
extern int stringToEnum(QString str);

//是否支付成功
bool isPayed = false;

/* 初始化购物车界面，将购物车信息显示至表格中 */
cartDialog::cartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cartDialog)
{
    ui->setupUi(this);

    isPayed=false;

    model = new QSqlTableModel;
    model->setTable("cart"+account);
    model->setHeaderData(1,Qt::Horizontal,"商品名称");
    model->setHeaderData(4,Qt::Horizontal,"价格");
    model->setHeaderData(5,Qt::Horizontal,"数量");
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setColumnWidth(1,200);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(2, true);
    ui->tableView->setColumnHidden(3, true);
    QHeaderView *header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(true);
    ui->tableView->show();

    QSqlQuery query;
    QSqlQuery query1;
    QSqlQuery query2;
    query.exec("SELECT * FROM cart"+account);
    total=0;
    sale=0;
    int lost = 0;
    int empty = 0;
    int change = 0;
    QString name;
    while(query.next()){
        name=query.value(1).toString();
        query1.exec("SELECT * FROM productInfo WHERE name='"+name+"'");
        if(!query1.next()){
            lost++;
            query2.exec("DELETE FROM cart"+account+" WHERE name='"+name+"'");
            cartnum--;
            query2.exec("UPDATE accountInfo SET cartnum="+QString::number(cartnum)+" WHERE id='"+account+"'");
        }
        else{
            if(query1.value(5).toInt()==0){
                empty++;
                query2.exec("DELETE FROM cart"+account+" WHERE name='"+name+"'");
                cartnum--;
                query2.exec("UPDATE accountInfo SET cartnum="+QString::number(cartnum)+" WHERE id='"+account+"'");
            }
            else{
                if(query1.value(4).toFloat()!=query.value(4).toFloat()){
                    query2.exec("UPDATE cart"+account+" SET price="+query1.value(4).toString()+" WHERE name='"+name+"'");
                    store.Product[query.value(2).toInt()]->setFullPrice(query1.value(4).toFloat());
                    change++;
                }
                sale+=store.Product[query.value(2).toInt()]->getPrice()*query.value(5).toInt();
                total+=query.value(4).toFloat()*query.value(5).toInt();
            }
        }
    }
    if(isEvent){
        if(sale>=eventReq){
            sale-=eventFav;
            ui->labelEvent->setVisible(true);
            ui->labelEvent->setText("(满"+QString::number(eventReq)+"减"+QString::number(eventFav)+")");
        }
        else{
            ui->labelEvent->setVisible(false);
        }
    }
    else{
        ui->labelEvent->setVisible(false);
    }
    if(lost!=0 || change!=0 || empty!=0){
        QMessageBox::information(NULL, "注意","有"+QString::number(lost)+"项丢失，"+QString::number(empty)+"项库存为空，"+QString::number(change)+"项被修改。", QMessageBox::Yes | QMessageBox::Yes);
        model->select();
    }
    ui->labelSale->setText(QString::number(sale)+"元");
    ui->labelTotal->setText(QString::number(total)+"元");
    ui->labelCount->setText("共计"+QString::number(cartnum)+"件商品。");
    if(cartnum==0){
        ui->buttonComfirm->setDisabled(true);
        ui->buttonClear->setDisabled(true);
    }
}

cartDialog::~cartDialog()
{
    delete ui;
}

void cartDialog::on_buttonCancel_clicked()
{
    close();
}

//清空购物车操作
void cartDialog::on_buttonClear_clicked()
{
    QSqlQuery query;
    query.exec("DELETE FROM cart"+account);
    cartnum=0;
    query.exec("UPDATE accountInfo SET cartnum="+QString::number(cartnum)+" WHERE id='"+account+"'");
    updateCart();
}

//当点击购物车某项时，修改该项信息
void cartDialog::on_tableView_clicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    int num = index.row()+1;
    cartEditDialog * cartedditdialog = new cartEditDialog(this, num);
    cartedditdialog->exec();
    updateCart();
}

//当修改完购物车后更新购物车当前视图
void cartDialog::updateCart(){
    model->select();
    QSqlQuery query;
    total=0;
    sale=0;
    query.exec("SELECT * FROM cart"+account);
    while(query.next()){
        sale+=store.Product[query.value(2).toInt()]->getPrice()*query.value(5).toInt();
        total+=query.value(4).toFloat()*query.value(5).toInt();
    }
    if(isEvent){
        if(sale>=eventReq){
            sale-=eventFav;
            ui->labelEvent->setVisible(true);
            ui->labelEvent->setText("(满"+QString::number(eventReq)+"减"+QString::number(eventFav)+")");
        }
        else{
            ui->labelEvent->setVisible(false);
        }
    }
    else{
        ui->labelEvent->setVisible(false);
    }
    ui->labelSale->setText(QString::number(sale)+"元");
    ui->labelTotal->setText(QString::number(total)+"元");
    ui->labelCount->setText("共计"+QString::number(cartnum)+"件商品。");
    if(cartnum==0){
        ui->buttonComfirm->setDisabled(true);
        ui->buttonClear->setDisabled(true);
    }
}

//当点击结算时，进入结算页面，结算成功，则清空购物车
void cartDialog::on_buttonComfirm_clicked()
{
    payDialog * paydialog = new payDialog(this, cartnum ,sale);
    paydialog->exec();
    if(isPayed==true){
        QSqlQuery query;
        QSqlQuery query1;
        query.exec("SELECT * FROM cart"+account);
        while(query.next()){
            store.Product[query.value(2).toInt()]->setStock(store.Product[query.value(2).toInt()]->getStock()-query.value(5).toInt());
            query1.exec("UPDATE productInfo SET stock="+QString::number(store.Product[query.value(2).toInt()]->getStock())+" WHERE name='"+query.value(1).toString()+"'");
        }
        query.exec("DELETE FROM cart"+account);
        cartnum=0;
        query.exec("UPDATE accountInfo SET cartnum="+QString::number(cartnum)+" WHERE id='"+account+"'");
        updateCart();
    }
}
