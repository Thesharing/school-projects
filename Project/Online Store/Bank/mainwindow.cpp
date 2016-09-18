#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "editdialog.h"
#include "depositdialog.h"
#include "withdrawdialog.h"
#include "account.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QString>

extern bool isLogged;
extern account* accountPtr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //在这里初始定义了数据库
    QFileInfo dbFileInfo("data.db");
    bool isExist=dbFileInfo.exists();
    db=QSqlDatabase:: addDatabase("QSQLITE");
    db.setDatabaseName("data.db");
    createConnection(isExist);

    //先显示“登录”界面，再进入主界面
    loginDialog * logindialog = new loginDialog(this);
    logindialog->exec();

    ui->setupUi(this);

    //“退出”按钮
    connect(ui->buttonExit,SIGNAL(clicked()),this,SLOT(close()));

    ui->labelID->setText(accountPtr->getId());
    ui->labelNum->setText(accountPtr->getNum());
    ui->labelBank->setText(accountPtr->getBank());
    ui->labelContact->setText(accountPtr->getContact());
    ui->labelBalance->setText(QString::number(accountPtr->getBalance(),'f',2));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//“存款”按钮
void MainWindow::on_buttonDeposit_clicked()
{
    depositDialog * depositdialog = new depositDialog(this);
    depositdialog->exec();
    updateInfo();
}

//“取款”按钮
void MainWindow::on_buttonWithdraw_clicked()
{
    withdrawDialog * withdrawdialog = new withdrawDialog(this);
    withdrawdialog->exec();
    updateInfo();
}

//“修改密码”按钮
void MainWindow::on_buttonEdit_clicked()
{
    editDialog * editdialog = new editDialog(this);
    editdialog->exec();
}

//每当存取款以后都要更新一下主界面
void MainWindow::updateInfo(){
    QSqlQuery query;
    QString num = accountPtr->getNum();
    query.exec("SELECT * FROM accountInfo WHERE num = '"+num+"'" );
    if(query.next())
        ui->labelBalance->setText(QString::number(query.value(4).toFloat(),'f',2));
}
