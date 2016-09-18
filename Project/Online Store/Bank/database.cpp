#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql>
#include <QWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

/* 这里创建了银行系统要用的数据库
 * accountInfo保存了所有账户信息
 * num是账号 password是密码 bank是银行
 * id是身份证号 balance是账户余额
 * contact是联系信息
 * 而bankInfo保存了银行的名称
 * name是银行的名称，而num是银行的序号 */

void MainWindow::createConnection(bool isExist){
    if(!db.open()){
        //如果数据库未打开，则报错
        QMessageBox::critical(0,QObject::tr("数据库错误"),db.lastError().text());
        return;
    }
    QSqlQuery query;
    if(!isExist){//如果数据库是第一次创建，则写入表项
        query.exec("CREATE TABLE accountInfo ("
                    "num NVARCHAR PRIMARY KEY,"
                    "password NVARCHAR,"
                    "bank NVARCHAR,"
                    "id NVARCHAR,"
                    "balance REAL,"
                    "contact NVARCHAR)"
                    );
        query.exec("CREATE TABLE bankInfo ("
                   "name NVARCHAR PRIMARY KEY,"
                   "num INT)"
                    );
    }
}
