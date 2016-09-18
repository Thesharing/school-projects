#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "guestdialog.h"
#include "ui_guestdialog.h"
#include "roominfodialog.h"
#include "ui_roominfodialog.h"
#include "backupdialog.h"
#include "ui_backupdialog.h"
#include "restoredialog.h"
#include "ui_restoredialog.h"
#include "setting.h"
#include "ui_setting.h"
#include "helpdialog.h"
#include "ui_helpdialog.h"
#include "checkin.h"
#include "checkout.h"

#include "datastruct.h"

#include <QtGui>
#include <QAction>
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QFile>
#include <QTextStream>
#include <QCloseEvent>

extern int now;
extern QString nowFormat;
extern int layerShow;
extern int price[5];
extern int itemPrice[5];
extern int interval;
extern int searchmodel;

extern void logPrint(QString str);
extern void sqlInitialize();

int convStrToIntForStatus(QString str);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(100,100,855,650); //设置界面的初始位置

    //弹出对话框询问是否要从备份中还原，如果需要则执行还原函数，如果不需要则执行初始化函数
    QMessageBox msgBox;//弹出提示是否从原有备份中还原
    msgBox.setWindowTitle(tr("旅店管理系统"));
    msgBox.setText(tr("是否从原有备份中还原？"));
    msgBox.setStandardButtons(QMessageBox::Yes
                              | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    //在这里初始定义了数据库
    QFileInfo dbFileInfo("data.db");
    bool isExist=dbFileInfo.exists();
    db=QSqlDatabase:: addDatabase("QSQLITE");
    db.setDatabaseName("data.db");
    createConnection(isExist);

    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes://点击yes则还原
        if(startRestoreFunction()==1){//如果备份数据有误则初始化
            initialFunction();
        }
        break;
    case QMessageBox::No://点击no则直接初始化
        initialFunction();
        break;
    }

    //接下来通过将数据库中的表项和图形界面中的列表模型相关联起来，将数据库的信息显示在列表中
    //第一个是顾客信息列表
    model = new QSqlTableModel;
    model->setTable("guestInfo");
    model->setHeaderData(1, Qt::Horizontal, "姓名");
    model->setHeaderData(2, Qt::Horizontal, "性别");
    model->setHeaderData(3, Qt::Horizontal, "身份证号");
    model->setHeaderData(4, Qt::Horizontal, "工作单位");
    model->setHeaderData(5, Qt::Horizontal, "手机号码");
    model->setHeaderData(6, Qt::Horizontal, "消费金额");
    model->setHeaderData(9, Qt::Horizontal, "入住时间");
    model->setHeaderData(10, Qt::Horizontal, "离店时间");
    model->setHeaderData(11, Qt::Horizontal, "房间号");
    model->select();
    ui->tableGuest->setModel(model);
    ui->tableGuest->setColumnHidden(0, true);
    ui->tableGuest->setColumnHidden(7, true);
    ui->tableGuest->setColumnHidden(8, true);
    ui->tableGuest->setColumnWidth(9,150);
    ui->tableGuest->setColumnWidth(10,150);;
    ui->tableGuest->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QHeaderView *header = ui->tableGuest->horizontalHeader();
    header->setStretchLastSection(true);
    ui->tableGuest->show();

    //第二个是日志列表
    modelLog=new QSqlTableModel;
    modelLog->setTable("log");
    modelLog->setHeaderData(1, Qt::Horizontal, "时间");
    modelLog->setHeaderData(2, Qt::Horizontal, "日志记录");
    modelLog->select();
    ui->tableLog->setModel(modelLog);
    ui->tableLog->setColumnHidden(0, true);
    ui->tableLog->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableLog->setColumnWidth(1,150);
    QHeaderView *header2 = ui->tableLog->horizontalHeader();
    header2->setStretchLastSection(true);
    ui->tableLog->show();

    //第三个是预约列表
    modelReserve=new QSqlTableModel;
    modelReserve->setTable("bookInfo");
    modelReserve->setHeaderData(1, Qt::Horizontal, "姓名");
    modelReserve->setHeaderData(2, Qt::Horizontal, "手机号");
    modelReserve->setHeaderData(5, Qt::Horizontal, "预计入住时间");
    modelReserve->setHeaderData(6, Qt::Horizontal, "预计离店时间");
    modelReserve->setHeaderData(7, Qt::Horizontal, "房间号");
    modelReserve->select();
    ui->tableReserve->setModel(modelReserve);
    ui->tableReserve->setColumnHidden(0, true);
    ui->tableReserve->setColumnHidden(3, true);
    ui->tableReserve->setColumnHidden(4, true);
    ui->tableReserve->setColumnWidth(1,150);
    ui->tableReserve->setColumnWidth(2,100);
    ui->tableReserve->setColumnWidth(5,150);
    ui->tableReserve->setColumnWidth(6,150);
    ui->tableReserve->setColumnWidth(7,100);
    ui->tableReserve->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QHeaderView *header3 = ui->tableLog->horizontalHeader();
    header3->setStretchLastSection(true);
    ui->tableReserve->show();

    //这里初始化进度条并开始计时，通过progressUpdate来推进时间并在特定时间处理相应事务（提醒退房、删除过期预约）
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(progressUpdate()));
    timer->setSingleShot(false);
    timer->start(interval);

    //显示统计信息
    MainWindow::count();

    //将界面上的元素和它对应函数连接起来
    //功能按键
    connect(ui->buttonGuestFunction,SIGNAL(clicked()),this,SLOT(guestFunction()));
    connect(ui->buttonCheckIn,SIGNAL(clicked()),this,SLOT(login()));
    connect(ui->buttonCheckOut,SIGNAL(clicked()),this,SLOT(logout()));

    //菜单栏
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionConfig,SIGNAL(triggered()),this,SLOT(settingFunction()));
    connect(ui->actionBackup,SIGNAL(triggered()),this,SLOT(backupFunction()));
    connect(ui->actionRestore,SIGNAL(triggered()),this,SLOT(restoreFunction()));
    connect(ui->actionHelp,SIGNAL(triggered()),this,SLOT(helpFunction()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(aboutFunction()));

    //三个列表
    connect(ui->lineEditGuest,SIGNAL(textChanged(QString)),this,SLOT(search()));
    connect(ui->lineEditReserve,SIGNAL(textChanged(QString)),this,SLOT(searchReserve()));
    connect(ui->lineEditLog,SIGNAL(textChanged(QString)),this,SLOT(searchLog()));
    connect(ui->clearButton,SIGNAL(clicked()),this,SLOT(searchclear()));
    connect(ui->clearButton2,SIGNAL(clicked()),this,SLOT(searchclearReserve()));
    connect(ui->clearButton3,SIGNAL(clicked()),this,SLOT(searchclearLog()));

}

void MainWindow::count()//统计函数
{
    QSqlQuery query;
    int countcheck[5] = {0};//统计各类型房间的入住情况
    int countbook[5] = {0};//统计各类型房间的预约情况
    int countempty[5] = {0};//统计各类型房间的空房情况
    double countlv[6] = {0};//统计各类型房间及总计的入住率
    QString roomstatus;
    query.exec("SELECT num, status FROM roomInfo WHERE num > 200 AND num < 300");
    while(query.next()){
        roomstatus = query.value(1).toString();
        if(roomstatus == "checked" || roomstatus == "checkandbook")
            countcheck[0]++;
        if(roomstatus == "booked" || roomstatus == "checkandbook")
            countbook[0]++;
        if(roomstatus == "empty")
            countempty[0]++;
    }

    query.exec("SELECT num, status FROM roomInfo WHERE num > 300 AND num < 400");
    while(query.next()){
        roomstatus = query.value(1).toString();
        if(roomstatus == "checked" || roomstatus == "checkandbook")
            countcheck[1]++;
        if(roomstatus == "booked" || roomstatus == "checkandbook")
            countbook[1]++;
        if(roomstatus == "empty")
            countempty[1]++;
    }

    query.exec("SELECT num, status FROM roomInfo WHERE num > 400 AND num < 500");
    while(query.next()){
        roomstatus = query.value(1).toString();
        if(roomstatus == "checked" || roomstatus == "checkandbook")
            countcheck[2]++;
        if(roomstatus == "booked" || roomstatus == "checkandbook")
            countbook[2]++;
        if(roomstatus == "empty")
            countempty[2]++;
    }

    query.exec("SELECT num, status FROM roomInfo WHERE num > 500 AND num < 600");
    while(query.next()){
        roomstatus = query.value(1).toString();
        if(roomstatus == "checked" || roomstatus == "checkandbook")
            countcheck[3]++;
        if(roomstatus == "booked" || roomstatus == "checkandbook")
            countbook[3]++;
        if(roomstatus == "empty")
            countempty[3]++;
    }

    query.exec("SELECT num, status FROM roomInfo WHERE num > 600 AND num < 700");
    while(query.next()){
        roomstatus = query.value(1).toString();
        if(roomstatus == "checked" || roomstatus == "checkandbook")
            countcheck[4]++;
        if(roomstatus == "booked" || roomstatus == "checkandbook")
            countbook[4]++;
        if(roomstatus == "empty")
            countempty[4]++;
    }
    int countcheckall = countcheck[0] + countcheck[1] + countcheck[2] + countcheck[3] + countcheck[4];
    countlv[0] = (int)(countcheck[0]*100 / 18);
    countlv[1] = (int)(countcheck[1]*100 / 18);
    countlv[2] = (int)(countcheck[2]*100 / 18);
    countlv[3] = (int)(countcheck[3]*100 / 18);
    countlv[4] = (int)(countcheck[4]*100 / 18);
    countlv[5] = (int)(countcheckall*100 / 90);

    ui->label_8->setText("一共18间房间，已入住"+QString::number(countcheck[0])+"间，已预订"+QString::number(countbook[0])+"间，空房剩余"+QString::number(countempty[0])+"间");
    ui->label_9->setText("一共18间房间，已入住"+QString::number(countcheck[1])+"间，已预订"+QString::number(countbook[1])+"间，空房剩余"+QString::number(countempty[1])+"间");
    ui->label_10->setText("一共18间房间，已入住"+QString::number(countcheck[2])+"间，已预订"+QString::number(countbook[2])+"间，空房剩余"+QString::number(countempty[2])+"间");
    ui->label_11->setText("一共18间房间，已入住"+QString::number(countcheck[3])+"间，已预订"+QString::number(countbook[3])+"间，空房剩余"+QString::number(countempty[3])+"间");
    ui->label_12->setText("一共18间房间，已入住"+QString::number(countcheck[4])+"间，已预订"+QString::number(countbook[4])+"间，空房剩余"+QString::number(countempty[4])+"间");

    ui->label_20->setText(QString::number(countlv[0])+"%");
    ui->label_21->setText(QString::number(countlv[1])+"%");
    ui->label_22->setText(QString::number(countlv[2])+"%");
    ui->label_23->setText(QString::number(countlv[3])+"%");
    ui->label_24->setText(QString::number(countlv[4])+"%");
    ui->label_25->setText(QString::number(countlv[5])+"%");

    int countguest = 0;//统计入住顾客人数
    int man = 0;//统计入住男性顾客人数
    int woman = 0;//统计入住女性顾客人数
    query.exec("SELECT gender FROM guestInfo WHERE room > 200");
    while(query.next()){
        QString sex = query.value(0).toString();
        if(sex == "男")    man++;
        else if(sex == "女")    woman++;
        countguest++;
    }
    ui->label_13->setText("一共已入住"+QString::number(countguest)+"人，其中男性"+QString::number(man)+"人，女性"+QString::number(woman)+"人");

}

void MainWindow::login()//登记函数
{
    MainWindow::timer->stop();
    checkinDialog *checkindialog = new checkinDialog(this);
    checkindialog->exec();
    MainWindow::timer->start(interval);
    MainWindow::roomButtonChange();//刷新客房信息
    MainWindow::count();//刷新统计信息
    model->select();//刷新顾客信息
    modelReserve->select();//刷新预约信息
    modelLog->select();//刷新日志信息
}

void MainWindow::logout()//离店函数
{
    MainWindow::timer->stop();
    checkoutDialog *checkoutdialog = new checkoutDialog(this);
    checkoutdialog->exec();
    MainWindow::timer->start(interval);
    MainWindow::roomButtonChange();//刷新客房信息
    MainWindow::count();//刷新统计信息
    model->select();//刷新顾客信息
    modelLog->select();//刷新日志信息
}

void MainWindow::search()//顾客信息查找函数
{
    QString searchtype = ui->comboBox->currentText();
    QString searchinfo = ui->lineEditGuest->text();
    QHeaderView *header = ui->tableGuest->horizontalHeader();
    header->setStretchLastSection(true);
    if(searchtype == "姓名"){//按姓名查找，searchmodel值为1是为模糊查找，值为0是为精确查找
        if(searchinfo == ""){
            model->setFilter("");
            model->select();
            ui->tableGuest->setModel(model);
            ui->tableGuest->show();
        }
        else{
            if(searchmodel == 0)
                model->setFilter("name = '"+searchinfo+"'");
            else if(searchmodel == 1)
                model->setFilter("name like '%"+searchinfo+"%'");
            model->select();
            ui->tableGuest->setModel(model);
            ui->tableGuest->show();
        }
    }
    else if(searchtype == "性别"){//按姓别查找，searchmodel值为1是为模糊查找，值为0是为精确查找
        if(searchinfo == ""){
            model->setFilter("");
            model->select();
            ui->tableGuest->setModel(model);
            ui->tableGuest->show();
        }
        else{
            model->setFilter("gender = '"+searchinfo+"'");
            model->select();
            ui->tableGuest->setModel(model);
            ui->tableGuest->show();
        }
    }
    else if(searchtype == "身份证号"){//按身份证号查找，searchmodel值为1是为模糊查找，值为0是为精确查找
        if(searchinfo == ""){
            model->setFilter("");
            model->select();
            ui->tableGuest->setModel(model);
            ui->tableGuest->show();
        }
        else{
            if(searchmodel == 0)
                model->setFilter("idNum = '"+searchinfo+"'");
            else if(searchmodel == 1)
                model->setFilter("idNum like '%"+searchinfo+"%'");
            model->select();
            ui->tableGuest->setModel(model);
            ui->tableGuest->show();
        }
    }
    else if(searchtype == "工作单位"){//按工作单位查找，searchmodel值为1是为模糊查找，值为0是为精确查找
        if(searchinfo == ""){
            model->setFilter("");
            model->select();
            ui->tableGuest->setModel(model);
            ui->tableGuest->show();
        }
        else{
            if(searchmodel == 0)
                model->setFilter("workplace = '"+searchinfo+"'");
            else if(searchmodel == 1)
                model->setFilter("workplace like '%"+searchinfo+"%'");
            model->select();
            ui->tableGuest->setModel(model);
            ui->tableGuest->show();
        }
    }
    else if(searchtype == "手机号码"){//按手机号码查找，searchmodel值为1是为模糊查找，值为0是为精确查找
        if(searchinfo == ""){
            model->setFilter("");
            model->select();
            ui->tableGuest->setModel(model);
            ui->tableGuest->show();
        }
        else{
            if(searchmodel == 0)
                model->setFilter("phoneNum = '"+searchinfo+"'");
            else if(searchmodel == 1)
                model->setFilter("phoneNum like '%"+searchinfo+"%'");
            model->select();
            ui->tableGuest->setModel(model);
            ui->tableGuest->show();
        }
    }
    else if(searchtype == "房间号"){//按房间号查找，searchmodel值为1是为模糊查找，值为0是为精确查找
        if(searchinfo == ""){
            model->setFilter("");
            model->select();
            ui->tableGuest->setModel(model);
            ui->tableGuest->show();
        }
        else{
            if(searchmodel == 0)
                model->setFilter("room = "+searchinfo+"");
            else if(searchmodel == 1)
                model->setFilter("room like '%"+searchinfo+"%'");
            model->select();
            ui->tableGuest->setModel(model);
            ui->tableGuest->show();
        }
    }
}


void MainWindow::searchclear()//重置顾客查找信息
{
    ui->lineEditGuest->setText("");
    model->setFilter("");
    model->select();
}

void MainWindow::searchReserve() //在预约列表中进行搜索
{
    //首先获得输入的信息和想要查询的类别，然后通过searchtype来控制是否模糊查询
    //（0为精确查询，1为模糊查询），将输入的信息
    //然后将搜索得到的结果显示在表格中
    QString searchtype = ui->comboBox2->currentText();
    QString searchinfo = ui->lineEditReserve->text();
    modelReserve = new QSqlTableModel;
    modelReserve->setTable("bookInfo");
    modelReserve->setHeaderData(1, Qt::Horizontal, "姓名");
    modelReserve->setHeaderData(2, Qt::Horizontal, "手机号");
    modelReserve->setHeaderData(5, Qt::Horizontal, "预计入住时间");
    modelReserve->setHeaderData(6, Qt::Horizontal, "预计离店时间");
    modelReserve->setHeaderData(7, Qt::Horizontal, "房间号");
    if(searchinfo==""){
        modelReserve->setFilter("");
    }
    else{
        if(searchtype == "全部"){
            if(searchmodel == 0)
                modelReserve->setFilter("name = '"+searchinfo+"' or "+"phoneNum = '"+searchinfo+"' or "+"checkInTimeText = '"+searchinfo+"' or "+"checkOutTimeText = '"+searchinfo+"' or "+"room = '"+searchinfo+"'");
            else if(searchmodel == 1)
                modelReserve->setFilter("name like '%"+searchinfo+"%' or "+"phoneNum like '%"+searchinfo+"%' or "+"checkInTimeText like '%"+searchinfo+"%' or "+"checkOutTimeText like '%"+searchinfo+"%' or "+"room like '%"+searchinfo+"%'");
        }
        else if(searchtype=="姓名"){
            if(searchmodel == 0)
                modelReserve->setFilter("name = '"+searchinfo+"'");
            else if(searchmodel == 1)
                modelReserve->setFilter("name like '%"+searchinfo+"%'");
        }
        else if(searchtype=="手机号"){
            if(searchmodel == 0)
                modelReserve->setFilter("phoneNum = '"+searchinfo+"'");
            else if(searchmodel == 1)
                modelReserve->setFilter("phoneNum like '%"+searchinfo+"%'");
        }
        else if(searchtype=="入住时间")
            if(searchmodel == 0)
                modelReserve->setFilter("checkInTimeText = '"+searchinfo+"'");
            else if(searchmodel == 1)
                modelReserve->setFilter("checkInTimeText like '%"+searchinfo+"%'");
    }
    modelReserve->select();
    ui->tableReserve->setModel(modelReserve);
    ui->tableReserve->show();
}

void MainWindow::searchclearReserve()//通过“清除所有”按钮将搜索条件清除
{
    //即清空输入框，将搜索条件置空
    ui->lineEditReserve->setText("");
    modelReserve->setFilter("");
    modelReserve->select();
}

void MainWindow::searchLog()//在预约列表中进行搜索
{
    //首先获得输入的信息和想要查询的类别，然后通过searchtype来控制是否模糊查询
    //（0为精确查询，1为模糊查询），将输入的信息
    //然后将搜索得到的结果显示在表格中
    QString searchtype = ui->comboBox3->currentText();
    QString searchinfo = ui->lineEditLog->text();
    modelLog = new QSqlTableModel;
    modelLog->setTable("log");
    modelLog->setHeaderData(1, Qt::Horizontal, "时间");
    modelLog->setHeaderData(2, Qt::Horizontal, "事件详情");
    if(searchinfo==""){
        modelLog->setFilter("");
    }
    else{
        if(searchtype == "时间"){
            if(searchmodel == 0)
                modelReserve->setFilter("time = '"+searchinfo+"'");
            else if(searchmodel == 1)
                modelLog->setFilter("time like '%"+searchinfo+"%'");
        }
        else if(searchtype == "内容"){
            if(searchmodel == 0)
                modelReserve->setFilter("string = '"+searchinfo+"'");
            else if(searchmodel == 1)
                modelLog->setFilter("string like '%"+searchinfo+"%'");
        }
    }
    modelLog->select();
    ui->tableLog->setModel(modelLog);
    ui->tableLog->show();
}

void MainWindow::searchclearLog()//通过“清除所有”按钮将搜索条件清除
{
    //即清空输入框，将搜索条件置空
    ui->lineEditLog->setText("");
    modelLog->setFilter("");
    modelLog->select();
}

void MainWindow::roomButtonChange(){//将“房间信息”中各按钮状态进行更新
    //首先将所有按钮放进数组中方便for循环
    QPushButton * roomButtonPtr[18];
    roomButtonPtr[0]=ui->roomButton1;
    roomButtonPtr[1]=ui->roomButton2;
    roomButtonPtr[2]=ui->roomButton3;
    roomButtonPtr[3]=ui->roomButton4;
    roomButtonPtr[4]=ui->roomButton5;
    roomButtonPtr[5]=ui->roomButton6;
    roomButtonPtr[6]=ui->roomButton7;
    roomButtonPtr[7]=ui->roomButton8;
    roomButtonPtr[8]=ui->roomButton9;
    roomButtonPtr[9]=ui->roomButton10;
    roomButtonPtr[10]=ui->roomButton11;
    roomButtonPtr[11]=ui->roomButton12;
    roomButtonPtr[12]=ui->roomButton13;
    roomButtonPtr[13]=ui->roomButton14;
    roomButtonPtr[14]=ui->roomButton15;
    roomButtonPtr[15]=ui->roomButton16;
    roomButtonPtr[16]=ui->roomButton17;
    roomButtonPtr[17]=ui->roomButton18;

    //然后查询相应房间的状态，空房间设为浅蓝色
    //入住房间设为绿色，预定房间设为深蓝色，预定且入住房间设为橙色
    //入住且超时的房间设为红色以提醒
    QSqlQuery query;
    query.exec("SELECT status, checkOutTime FROM roomInfo WHERE num like '"+QString::number(layerShow)+"%'");
    int i=0;
    while(query.next()){
        switch(convStrToIntForStatus(query.value(0).toString())){
        case empty:
            roomButtonPtr[i]->setStyleSheet("QPushButton{font-size:14px;background:paleturquoise;border-style: solid}");
            break;
        case checked:
            if(query.value(1).toInt()<=now)
                roomButtonPtr[i]->setStyleSheet("QPushButton{font-size:14px;background:red;border-style: solid;}");
            else
                roomButtonPtr[i]->setStyleSheet("QPushButton{font-size:14px;background:green;border-style: solid;}");
            break;
        case booked:
                roomButtonPtr[i]->setStyleSheet("QPushButton{font-size:14px;background:dodgerblue;border-style: solid;}");
            break;
        case checkandbook:
            if(query.value(1).toInt()<=now)
                roomButtonPtr[i]->setStyleSheet("QPushButton{font-size:14px;background:red;border-style: solid;}");
            else
                roomButtonPtr[i]->setStyleSheet("QPushButton{font-size:14px;background:orange;border-style: solid;}");
            break;
        default:
            break;
        }
        if(i<9)
            roomButtonPtr[i]->setText(QString::number(layerShow, 10)+"0"+QString::number(i+1, 10));
        else
            roomButtonPtr[i]->setText(QString::number(layerShow, 10)+QString::number(i+1, 10));
        i++;
    }
}

void MainWindow::progressUpdate(){
    //时间函数，通过进度条的推进来推动时间前进
    //首先设置一个timer定时器来使进度条不断前进，当进度条满时时间+20分钟，以此达到每个时间间隔时间递增20分钟
   ui->progressBar->setValue(ui->progressBar->value()+1);
   if(ui->progressBar->value()>=100){
       now+=1200;
       ui->progressBar->reset();
   }

   //这里将int的时间转换为标准时间格式方便输出
   QDateTime temp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
   temp=temp.addSecs(qint64(now));
   ui->labelDate->setText("时间："+temp.toString("yyyy/MM/dd"));
   ui->labelTime->setText(temp.toString("hh:mm:ss"));

   //当午夜0点时查找过期预约并删除之，更新删除预约的房间的状态（详见取消预约函数(cancelreservedialog.cpp))
   if(temp.time()==QTime(0,0,0) && ui->progressBar->value()==0){
       QSqlQuery query;
       QSqlQuery query2;
       QSqlQuery query3;
       int num=0;
       query.exec("SELECT room, name FROM bookInfo WHERE checkInTime <= "+QString::number(now));
       while(query.next()){
           query2.exec("SELECT * FROM bookInfo WHERE room = '"+query.value(0).toString()+"' AND name != '"+query.value(1).toString()+"'");
           if(!query2.next()){
               query2.exec("SELECT status FROM roomInfo WHERE num ="+query.value(0).toString());
               while(query2.next()){
                   if(query2.value(0).toString()=="booked")
                   {
                       query3.exec("UPDATE roomInfo SET status = 'empty' WHERE num ="+query.value(0).toString());
                   }
                   else if(query2.value(0).toString()=="checkandbook")
                       query3.exec("UPDATE roomInfo SET status = 'booked' WHERE num ="+query.value(0).toString());
               }
           }
           num++;
       }
       //如果有预约过期，删除之并弹出提示、记入日志
       if(num>0){
           QMessageBox::information(NULL, "提示","有"+QString::number(num)+"个顾客的预约已经过期，已删除。", QMessageBox::Yes | QMessageBox::Yes);
           logPrint(QString::number(num)+"名顾客的预约因超时而取消。");
       }
       query.exec("DELETE FROM bookInfo WHERE checkInTime <= "+QString::number(now));
       //更新主界面各状态
       MainWindow::roomButtonChange();
       MainWindow::count();
       modelReserve->select();
       modelLog->select();
   }

   //当中午12点时查询是否有房间超时，有的话弹出提醒要求催促其退房
   if(temp.time()==QTime(12,0,0) && ui->progressBar->value()==0){
       QSqlQuery query;
       int num=0;
       QString name="";
       query.exec("SELECT num FROM roomInfo WHERE checkOutTime <= "+QString::number(now)+" AND status = 'checked'");
       while(query.next()){
           num++;
           name=name+" "+QString::number(query.value(0).toInt())+"房间";
       }
       query.exec("SELECT num FROM roomInfo WHERE checkOutTime <= "+QString::number(now)+" AND status = 'checkandbook'");
       while(query.next()){
           num++;
           name=name+" "+QString::number(query.value(0).toInt())+"房间";
       }
       if(num!=0){
          QMessageBox::information(NULL, "提示","有"+QString::number(num)+"个顾客的房间已超时，请提醒其退房。", QMessageBox::Yes | QMessageBox::Yes);
          MainWindow::roomButtonChange();
       }
   }
}

//以下三个函数用于在鼠标移动、鼠标按键和键盘按键时重置进度条以达到时间停止效果
void MainWindow::mouseMoveEvent(QMouseEvent * event){
    if(event)
        ui->progressBar->setValue(0);
}

void MainWindow::mousePressEvent(QMouseEvent * event){
    if(event)
        ui->progressBar->setValue(0);
}

void MainWindow::keyPressEvent(QMouseEvent * event){
    if(event)
        ui->progressBar->setValue(0);
}

//在“客房信息”中可以改变当前显示的楼层数
void MainWindow::on_layerComboBox_currentIndexChanged(int index)
{
    layerShow=index+2;
    roomButtonChange();
}

//当点击“顾客界面”时弹出顾客界面并停止计时，等到客户操作完成后恢复计时，将主界面状态更新
//“顾客界面”详见guestdialog.cpp
void MainWindow::guestFunction(){
    MainWindow::timer->stop();

    guestDialog * guestDialog1=new guestDialog(this);
    guestDialog1->exec();
    MainWindow::timer->start(interval);
    MainWindow::roomButtonChange();
    MainWindow::count();
    modelReserve->select();
    ui->tableLog->resizeColumnsToContents();
    modelLog->select();
}

//将房间状态的英文字符串转换成枚举
int convStrToIntForStatus(QString str){
    switch(str.at(0).toLatin1()){
        case 'e':
            return 0;
            break;
        case 'c':
            if(str.at(5).toLatin1()=='e')
                return 1;
            else
                return 3;
            break;
        case 'b':
            return 2;
        default:
            return -1;
    }
}

//设置函数，在这里打开“设置”界面，设置的功能函数详见setting.cpp
void MainWindow::settingFunction(){
    MainWindow::timer->stop();
    setting * setting1=new setting(this);
    setting1->exec();
    MainWindow::timer->start(interval);
    modelLog->select();
}

//备份函数，这里是备份的主要功能函数
void MainWindow::backupFunction(){
    //将数据库备份为dataBackup.db
    MainWindow::timer->stop();
    QString fileName="dataBackup.db";
    //首先关闭内存数据库调用，将文件复制为备份文件。
    //如果备份文件不存在或复制失败都会报错
    db.close();
    QFile dbBackup(fileName);
    if(dbBackup.exists()){
        dbBackup.remove();
    }
    QFile dbOrigin("data.db");
    if(!dbOrigin.exists()){
        QMessageBox::warning(NULL, "提示","数据库文件不存在。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    dbOrigin.copy(fileName);
    if(!dbBackup.exists()){
        QMessageBox::warning(NULL, "提示","备份失败，请联系管理员。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    dbBackup.close();
    db.open();//重新打开数据库的调用

    //将系统参数以文本格式保存到hotel.ini中方便管理员修改
    QFile text("hotel.ini");
    text.close();
    if(!text.open(QIODevice::Text|QIODevice::ReadWrite)){
        QMessageBox::warning(NULL, "提示","无法打开配置文件，请联系管理员。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QTextStream out(&text);
    out<<QString::number(now)<<endl;
    for(int i=0;i<5;i++){
        out<<QString::number(price[i])<<endl;
    }
    for(int i=0;i<5;i++){
        out<<QString::number(itemPrice[i])<<endl;
    }
    out<<QString::number(searchmodel)<<endl;
    out<<QString::number(layerShow)<<endl;
    out<<QString::number(interval)<<endl;

    //渲染界面
    backupDialog * backupDialog1=new backupDialog(this);
    backupDialog1->backupStatus();
    backupDialog1->exec();
    MainWindow::timer->start(interval);
}

//界面开始时还原函数
int MainWindow::startRestoreFunction(){
    //将数据库还原为data.db
    db.close();
    QString fileName="data.db";
    //首先关闭内存数据库调用，还原文件。
    //如果备份文件不存在或还原失败会直接返回1，不执行该函数下面的操作
    QFile dbBackup("dataBackup.db");
    if(!dbBackup.exists()){
        QMessageBox::warning(NULL, "提示","备份文件不存在。", QMessageBox::Yes | QMessageBox::Yes);
        db.open();
        return 1;
    }
    QFile dbOrigin(fileName);
    if(dbOrigin.exists()){
        dbOrigin.remove();
    }
    dbBackup.copy(fileName);
    if(!dbOrigin.exists()){
        QMessageBox::warning(NULL, "提示","还原失败，请联系管理员。", QMessageBox::Yes | QMessageBox::Yes);
        db.open();
        return 1;
    }
    dbOrigin.close();
    db.open();//重新打开数据库的调用
    //将系统参数从文本文件hotel.ini中还原
    QFile text("hotel.ini");
    text.close();
    if(!text.open(QIODevice::Text|QIODevice::ReadWrite)){
        QMessageBox::warning(NULL, "提示","无法打开配置文件，请联系管理员。", QMessageBox::Yes | QMessageBox::Yes);
        return 1;
    }
    QTextStream in(&text);
    in>>now;
    for(int i=0;i<5;i++){
        in>>price[i];
    }
    for(int i=0;i<5;i++){
        in>>itemPrice[i];
    }
    in>>searchmodel;
    in>>layerShow;
    in>>interval;
    //渲染界面
    restoreDialog * restoreDialog1=new restoreDialog(this);
    restoreDialog1->restoreStatus();
    restoreDialog1->exec();
    return 0;
}

void MainWindow::initialFunction(){//初始化函数，将数据库中原有数据全部初始化
    price[standard]=200;//重置各个设置数据
    price[doublebed]=300;
    price[largebed]=300;
    price[business]=400;
    price[vip]=800;
    itemPrice[0]=1;
    itemPrice[1]=3;
    itemPrice[2]=5;
    itemPrice[3]=8;
    itemPrice[4]=10;
    searchmodel = 1;
    layerShow=2;
    interval=100;

    QDateTime temp2=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
    QDateTime temp3=QDateTime::currentDateTime();
    temp3=temp3.addSecs((qint64)(0-temp3.time().minute()*60-temp3.time().second()));
    now=(int)temp2.secsTo(temp3);

    QSqlQuery query;
    query.exec("DELETE FROM guestInfo");//将顾客信息表中数据从数据库中删除
    query.exec("DELETE FROM roomInfo");//将房间信息表中数据从数据库中删除
    query.exec("DELETE FROM bookInfo");//将预约信息表中数据从数据库中删除
    query.exec("DELETE FROM log");//将日志信息表中数据从数据库中删除

    //重新初始化房间信息表
    for(int j=1;j<=18;j++){
        query.exec("INSERT INTO roomInfo (num, type, status, checkInTime, checkOutTime, cost)"
            "VALUES("+QString::number(200+j)+", 'standard', 'empty', 0, 0, 0)");
    }
    for(int j=1;j<=18;j++){
        query.exec("INSERT INTO roomInfo (num, type, status, checkInTime, checkOutTime, cost)"
            "VALUES("+QString::number(300+j)+", 'doublebed', 'empty', 0, 0, 0)");
    }
    for(int j=1;j<=18;j++){
        query.exec("INSERT INTO roomInfo (num, type, status, checkInTime, checkOutTime, cost)"
            "VALUES("+QString::number(400+j)+", 'largebed', 'empty', 0, 0, 0)");
    }
    for(int j=1;j<=18;j++){
        query.exec("INSERT INTO roomInfo (num, type, status, checkInTime, checkOutTime, cost)"
            "VALUES("+QString::number(500+j)+", 'business', 'empty', 0, 0, 0)");
    }
    for(int j=1;j<=18;j++){
        query.exec("INSERT INTO roomInfo (num, type, status, checkInTime, checkOutTime, cost)"
            "VALUES("+QString::number(600+j)+", 'vip', 'empty', 0, 0, 0)");
    }
}

//还原函数，这里是还原的主要功能函数
void MainWindow::restoreFunction(){
    //将数据库还原为data.db
    MainWindow::timer->stop();
    QString fileName="data.db";
    db.close();
    //首先关闭内存数据库调用，还原文件。
    //如果备份文件不存在或还原失败都会报错
    QFile dbOrigin(fileName);
    if(dbOrigin.exists()){
        dbOrigin.remove();
    }
    QFile dbBackup("dataBackup.db");
    if(!dbBackup.exists()){
        QMessageBox::warning(NULL, "提示","备份文件不存在。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    dbBackup.copy(fileName);
    if(!dbOrigin.exists()){
        QMessageBox::warning(NULL, "提示","还原失败，请联系管理员。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    dbOrigin.close();
    db.open();//重新打开数据库的调用
    //将系统参数从文本文件hotel.ini中还原
    QFile text("hotel.ini");
    text.close();
    if(!text.open(QIODevice::Text|QIODevice::ReadWrite)){
        QMessageBox::warning(NULL, "提示","无法打开配置文件，请联系管理员。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QTextStream in(&text);
    in>>now;
    for(int i=0;i<5;i++){
        in>>price[i];
    }
    for(int i=0;i<5;i++){
        in>>itemPrice[i];
    }
    in>>searchmodel;
    in>>layerShow;
    in>>interval;
    //渲染界面
    restoreDialog * restoreDialog1=new restoreDialog(this);
    restoreDialog1->restoreStatus();
    restoreDialog1->exec();
    MainWindow::timer->start(interval);
    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
    modelReserve->select();
}

void MainWindow::helpFunction(){//帮助函数
    MainWindow::timer->stop();
    helpdialog *help = new helpdialog;
    help->exec();
    /*
    QMessageBox::about(this,tr("系统帮助"),
                       tr("<h2>旅馆帮助系统</h2>"
                          "<p>该旅店系统有预约，入住，离店，查询等功能"
                          "<p>旅店分二楼至六楼共五层："
                          "<p>一层为标准间"
                          "<p>二层为双人间"
                          "<p>三层为大床房"
                          "<p>四层为商务房"
                          "<p>五层为VIP贵宾间"
                          "<p>客房信息中房间颜色代表信息分别为："
                          "<p>天蓝：空闲"
                          "<p>蓝：被预约"
                          "<p>绿：已入住"
                          "<p>橘黄：已入住且被预约"
                          "<p>红：入住超时"));*/
    MainWindow::timer->start(interval);
}

void MainWindow::aboutFunction(){//关于函数
    MainWindow::timer->stop();
    QMessageBox::about(this,tr("关于旅馆管理系统"),
                       tr("<h2>旅馆管理系统1.0</h2>"
                          "<p>感谢您选用本旅馆管理系统。"
                          "<p>此管理系统坚持以人为本的设计理念。"
                          "<p><p>由洪志龙、黄天烁、薛瑞共同开发。"));
    MainWindow::timer->start(interval);
}


//当点击“客房信息”中的房间按钮时打开“房间信息”的窗口显示房间信息
//由于存在入住或退房，因而关闭窗口时更新一下主界面的状态
//“房间信息”对话框的函数updateRoomInfoDialog(int num)详见roominfodialog.cpp
void MainWindow::on_roomButton1_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);
    roominfodialog1->updateRoomInfoDialog(100*layerShow+1);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}


//接下来对应其他17个按钮，其实现方式相同
void MainWindow::on_roomButton2_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);
    roominfodialog1->updateRoomInfoDialog(100*layerShow+2);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}


void MainWindow::on_roomButton3_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);
    roominfodialog1->updateRoomInfoDialog(100*layerShow+3);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}



void MainWindow::on_roomButton4_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);
    roominfodialog1->updateRoomInfoDialog(100*layerShow+4);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton5_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);
    roominfodialog1->updateRoomInfoDialog(100*layerShow+5);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton6_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);
    roominfodialog1->updateRoomInfoDialog(100*layerShow+6);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton7_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);
    roominfodialog1->updateRoomInfoDialog(100*layerShow+7);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton8_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);
    roominfodialog1->updateRoomInfoDialog(100*layerShow+8);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton9_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);
    roominfodialog1->updateRoomInfoDialog(100*layerShow+9);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton10_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);
    roominfodialog1->updateRoomInfoDialog(100*layerShow+10);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton11_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);
    roominfodialog1->updateRoomInfoDialog(100*layerShow+11);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton12_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);
    roominfodialog1->updateRoomInfoDialog(100*layerShow+12);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton13_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);    
    roominfodialog1->updateRoomInfoDialog(100*layerShow+13);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton14_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);    
    roominfodialog1->updateRoomInfoDialog(100*layerShow+14);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton15_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);    
    roominfodialog1->updateRoomInfoDialog(100*layerShow+15);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton16_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true);   
    roominfodialog1->updateRoomInfoDialog(100*layerShow+16);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton17_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true); 
    roominfodialog1->updateRoomInfoDialog(100*layerShow+17);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}

void MainWindow::on_roomButton18_clicked()
{
    roominfodialog * roominfodialog1=new roominfodialog(this);
    roominfodialog1->setAttribute(Qt::WA_ShowModal, true); 
    roominfodialog1->updateRoomInfoDialog(100*layerShow+18);
    roominfodialog1->exec();

    MainWindow::roomButtonChange();
    MainWindow::count();
    model->select();
    modelLog->select();
}


//当关闭主界面时询问是否需要备份，需要备份则进行备份
//备份函数为backupFunction()，在本cpp文件中
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(event){
        int r=QMessageBox::information(NULL, "旅馆管理系统","是否退出？", QMessageBox::Yes | QMessageBox::No);
        if(r==QMessageBox::Yes){
            QMessageBox msgBox1;
            msgBox1.setWindowTitle(tr("提示"));
            msgBox1.setText(tr("即将退出旅店管理系统。"));
            msgBox1.setInformativeText(tr("是否备份数据？"));
            msgBox1.setStandardButtons(QMessageBox::Yes
                                      | QMessageBox::No);
            msgBox1.setDefaultButton(QMessageBox::No);
            int ret = msgBox1.exec();
            switch (ret) {
            case QMessageBox::Yes:
                MainWindow::backupFunction();
                break;
            case QMessageBox::No:
                break;
            }
        }
        else
            event->ignore();
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
