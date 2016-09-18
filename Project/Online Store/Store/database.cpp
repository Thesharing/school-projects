#include <QtSql>
#include <QWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datastruct.h"

/* 建立数据库 */

int stringToEnum(QString str);
QString enumToString(int type);

extern bool isEvent;
extern int eventReq;
extern int eventFav;
extern struct Store store;

/* accountInfo是账户信息
 * id是账户名
 * password是密码
 * cartnum是当前购物车商品数量
 * productInfo是商品信息
 * id是商品序号
 * name是商品名称
 * type是商品类型，description是描述
 * fullprice是类型，stock是存量
 * rate是打折比率 other是其他信息 */
void MainWindow::createConnection(bool isExist){
    if(!db.open()){
        //如果数据库未打开，则报错
        QMessageBox::critical(0,QObject::tr("数据库错误"),db.lastError().text());
        return;
    }
    QSqlQuery query;
    if(!isExist){//如果数据库是第一次创建，则写入表项
        query.exec("CREATE TABLE accountInfo ("
                    "id NVARCHAR PRIMARY KEY,"
                    "password NVARCHAR,"
                    "cartnum INT)"
                    );
        query.exec("CREATE TABLE productInfo ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "name NVARCHAR,"
                   "type NVARCHAR,"
                   "description NVARCHAR,"
                   "fullprice REAL,"
                   "stock INT,"
                   "rate REAL,"
                   "other NVARCHAR)"
                    );
    }
}

/* initialize将数据库中商品信息读取至内存的store结构中，store将存储所有商品信息 */
void MainWindow::initialize(){
    store.ProductCount=0;
    store.BookCount=0;
    store.ClothCount=0;
    store.FoodCount=0;
    store.ElectroCount=0;
    QSqlQuery query;
    query.exec("SELECT * FROM productInfo" );
    while(query.next()){
        QString name=query.value(1).toString();
        QString description=query.value(3).toString();
        float fullprice=query.value(4).toFloat();
        int stock=query.value(5).toInt();
        float rate=query.value(6).toFloat();
        QString other=query.value(7).toString();
        int type=stringToEnum(query.value(2).toString());
        switch (type) {
        case BOOK:
            store.Book[store.BookCount]=new book(name,description,fullprice,stock,rate,other);
            store.Product[store.ProductCount]=store.Book[store.BookCount];
            store.BookCount++;
            store.ProductCount++;
            break;
        case CLOTH:
            store.Cloth[store.ClothCount]=new cloth(name,description,fullprice,stock,rate,other);
            store.Product[store.ProductCount]=store.Cloth[store.ClothCount];
            store.ClothCount++;
            store.ProductCount++;
            break;
        case FOOD:
            store.Food[store.FoodCount]=new food(name,description,fullprice,stock,rate,other);
            store.Product[store.ProductCount]=store.Food[store.FoodCount];
            store.FoodCount++;
            store.ProductCount++;
            break;
        case ELECTRO:
            store.Electro[store.ElectroCount]=new electro(name,description,fullprice,stock,rate,other);
            store.Product[store.ProductCount]=store.Electro[store.ElectroCount];
            store.ElectroCount++;
            store.ProductCount++;
            break;
        default:
            break;
        }
    }
}

int stringToEnum(QString str){
    //将类型信息从字符串转换成int
    if(str=="Food"){
        return FOOD;
    }
    else if(str=="Cloth"){
        return CLOTH;
    }
    else if(str=="Book"){
        return BOOK;
    }
    else if(str=="Electro"){
        return ELECTRO;
    }
    return -1;
}

QString enumToString(int type){
    //将类型信息从int转换成字符串
    switch (type){
    case 0:
        return "Food";
        break;
    case 1:
        return "Cloth";
        break;
    case 2:
        return "Book";
        break;
    case 3:
        return "Electro";
    default:
        return "Fault";
        break;
    }
}

