#include "checkin.h"
#include "datastruct.h"
#include "mainwindow.h"
#include "checkindoubledialog.h"
#include <QtGui>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QRegExp>
#include <QSpinBox>
#include <QTimer>
#include <QSqlError>
#include <QString>
#include <QDateTime>

extern struct roomNode * roomList;
extern struct guestNode * guestList;

extern int price[5];

extern int now;
extern QString nowFormat;

void logPrint(QString str);


checkinDialog::checkinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkinDialog)
{//点击主界面入住按钮弹出此窗口
    ui->setupUi(this);
    setFixedSize(340,495);//控制窗口大小
    setWindowTitle(tr("登记入住"));
    ui->spinBoxdays->setRange(1,100);
    ui->spinBoxdays->setValue(1);
    QRegExp regExp1("[0-9]{17}[0-9A-Za-z]");
    QRegExp regExp2("[0-9]{11}");
    ui->lineEditid->setValidator(new QRegExpValidator(regExp1,this));//控制身份证号输入格式
    ui->lineEditphone->setValidator(new QRegExpValidator(regExp2,this));//控制手机号输入格式

    QDateTime temp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
    temp=temp.addSecs(qint64(now));
    ui->dateEditCheckIn->setDate(temp.date());
    ui->dateEditCheckIn->setMinimumDate(temp.date());
    ui->dateEditCheckIn->setMaximumDate(temp.date());

    connect(ui->checkBoxman,SIGNAL(clicked()),this,SLOT(setman()));
    connect(ui->checkBoxwoman,SIGNAL(clicked()),this,SLOT(setwoman()));
    connect(ui->checkBoxno,SIGNAL(clicked()),this,SLOT(setno()));
    connect(ui->checkBoxyes,SIGNAL(clicked()),this,SLOT(setyes()));

    connect(ui->pushButtoncheckin,SIGNAL(clicked()),this,SLOT(checkinto()));
    connect(ui->pushButtoncancel,SIGNAL(clicked()),this,SLOT(close()));
}

void checkinDialog::checkinto()//登记与预约转登记
{
    QString nametext = ui->lineEditname->text();
    QString workplacetext = ui->lineEditworkplace->text();
    QString idtext = ui->lineEditid->text();
    QString phonetext = ui->lineEditphone->text();
    QString sextext;

    bool sex;
    int finalRoom;//登记最后入住成功的房间

    if(ui->checkBoxman->checkState() == Qt::Checked){//选中checkbox
        sex = TRUE;
        sextext = "男";
    }
    else if(ui->checkBoxwoman->checkState() == Qt::Checked){
        sex = FALSE;
        sextext = "女";
    }
    bool isreserve;
    if(ui->checkBoxyes->checkState() == Qt::Checked){
        isreserve = TRUE;
    }
    else if(ui->checkBoxno->checkState() == Qt::Checked){
        isreserve = FALSE;
    }

    QString roomtypetext = ui->comboBoxroomtype->currentText();
    enum roomType roomtype;
    if(roomtypetext == "标准间")    roomtype = standard;
    else if(roomtypetext == "双人间")    roomtype = doublebed;
    else if(roomtypetext == "大床房")    roomtype = largebed;
    else if(roomtypetext == "商务房")    roomtype = business;
    else if(roomtypetext == "贵宾间")    roomtype = vip;
    int days = ui->spinBoxdays->value();

    int checkintime = now;//获取当前时间作为入住时间
    int checkouttime = now-now%(24*60*60)+24*60*60*days+12*60*60;//离店时间
    QDateTime checkInTemp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
    checkInTemp=checkInTemp.addSecs(qint64(checkintime));
    QDateTime checkOutTemp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
    checkOutTemp=checkOutTemp.addSecs(qint64(checkouttime));

    if(nametext.isEmpty() || idtext.isEmpty() ||idtext.size()<18|| workplacetext.isEmpty() || phonetext.isEmpty() ||
            (ui->checkBoxman->checkState() != Qt::Checked && ui->checkBoxwoman->checkState() != Qt::Checked) ||
            (ui->checkBoxno->checkState() != Qt::Checked && ui->checkBoxyes->checkState() != Qt::Checked))
    {//如果有信息未输入则提示
        QMessageBox *messagebox3 = new QMessageBox;
        QString str;
        str += "输入信息不完整     ";
        messagebox3->setText(str);
        messagebox3->exec();

    }
    else{
        QSqlQuery query;
        int consu;
        int cost;
        if(isreserve == TRUE){//如果顾客已经预约过
            int roomnumreserve;
            int isbooked1 = 1;//用来判断预约表是否有此顾客
            int checkintime1;
            int checkouttime1;
            query.exec("SELECT name,phoneNum,checkInTime,checkOutTime,room FROM bookInfo WHERE name = '"+nametext+"'");//从数据库预约表按名字检索
            while(query.next()){
                QString phonenum1 = query.value(1).toString();
                checkintime1 = query.value(2).toInt();
                checkouttime1 = query.value(3).toInt();
                roomnumreserve = query.value(4).toInt();
                if(phonenum1 == phonetext){
                    isbooked1 = 0;
                    break;
                }
            }
            switch(roomnumreserve / 100){
                case 2:consu = price[standard];break;
                case 3:consu = price[doublebed];break;
                case 4:consu = price[largebed];break;
                case 5:consu = price[business];break;
                case 6:consu = price[vip];break;
            }
            cost = days * consu + 300;

            if(isbooked1 == 1){//预约表没有顾客信息则提示
                QMessageBox *messagebox = new QMessageBox;
                QString str;
                str += "没有预约信息，请检查是否预约过     ";
                messagebox->setText(str);
                messagebox->exec();
            }
            else{
                if(checkintime1 > checkintime){//入住时间小于预约时间则提示
                    QMessageBox *messagebox = new QMessageBox;
                    QString str;
                    str += "预约的房间暂时不能入住，请仔细核对预约信息     ";
                    messagebox->setText(str);
                    messagebox->exec();
                }
                else{
                    query.exec("DELETE FROM bookInfo WHERE room = "+QString::number(roomnumreserve)+" AND name = '"+nametext+"'");//从预约表删除此顾客预约信息
                    query.exec("UPDATE roomInfo SET status = 'checked' WHERE num = "+QString::number(roomnumreserve)+"");
                    query.exec("UPDATE roomInfo SET checkInTime = "+QString::number(checkintime)+" WHERE num = "+QString::number(roomnumreserve)+"");
                    query.exec("UPDATE roomInfo SET checkOutTime = "+QString::number(checkouttime)+" WHERE num = "+QString::number(roomnumreserve)+"");
                    query.exec("UPDATE roomInfo SET checkInTimeText = '"+checkInTemp.toString(nowFormat)+"' WHERE num = "+QString::number(roomnumreserve)+"");
                    query.exec("UPDATE roomInfo SET checkOutTimeText = '"+checkOutTemp.toString(nowFormat)+"' WHERE num = "+QString::number(roomnumreserve)+"");
                    query.exec("UPDATE roomInfo SET cost = '"+QString::number(cost)+"' WHERE num = "+QString::number(roomnumreserve)+"");//更新房间信息表中各个状态
                    query.exec("INSERT INTO guestInfo (name, gender, idNum, workplace, phoneNum, consume, checkInTime, checkOutTime, checkInTimeText, checkOutTimeText, room)"
                         "VALUES('"+nametext+"', '"+sextext+"', '"+idtext+"', '"+workplacetext+"', '"+phonetext+"', 0, "+QString::number(checkintime)+", "+QString::number(checkouttime)+", '"+checkInTemp.toString(nowFormat)+"', '"+checkOutTemp.toString(nowFormat)+"', "+QString::number(roomnumreserve)+")");
                    //将顾客信息插入顾客信息表中
                    finalRoom=roomnumreserve;
                    if(roomnumreserve / 100 == 2 || roomnumreserve / 100 == 5 || roomnumreserve / 100 == 6){
                        checkinDialog::deletereverse(cost);//弹出提示已从预约表删除且入住成功
                        this->hide();//退出时隐藏此窗口
                        logPrint("顾客 "+nametext+" 登记入住于 "+QString::number(finalRoom)+"房间");//将事件写入日志文件
                    }
                    else if(roomnumreserve / 100 == 3 || roomnumreserve / 100 == 4){
                        checkinDialog::deletereverse(cost);//弹出提示已从预约表删除且入住成功
                        QMessageBox *messagebox = new QMessageBox;
                        QString str;
                        str += "您还可以再次添加入住信息     ";
                        messagebox->setText(str);
                        messagebox->exec();
                        this->close();//退出时隐藏此窗口
                        checkindoubleDialog *checkindouble = new checkindoubleDialog(roomnumreserve,days,this);
                        checkindouble->exec();

                        logPrint("顾客 "+nametext+" 登记入住于 "+QString::number(finalRoom)+"房间");//将事件写入日志文件
                    }

                }
            }

        }
        else{//如果没有预约过
            switch(roomtype){//判断顾客所选的房间类型
                case standard:{
                    int roomnum1;
                    int isfull1 = 1;//用来判断该房间类型的房间是否有空房及是否有已被预约但可以入住的房间
                    int checkintime1;
                    int checkouttime1;
                    query.exec("SELECT num,status FROM roomInfo WHERE  num > 200 AND num < 300");//检索房间信息表中此类型的房间
                    while(query.next()){
                        roomnum1 = query.value(0).toInt();
                        QString roomstatus = query.value(1).toString();
                        if(roomstatus == "empty"){//如果有空房则跳出循环
                            isfull1 = 0;
                            break;
                        }
                        else if(roomstatus == "booked"){
                            query.exec("SELECT checkInTime,checkOutTime,room FROM bookInfo WHERE room = "+QString::number(roomnum1)+"");
                            while(query.next()){
                                checkintime1 = query.value(0).toInt();
                                checkouttime1 = query.value(1).toInt();
                                if(checkouttime < checkintime1){//如果有已被预约但可以入住的房间则跳出循环
                                    isfull1 = 2;
                                    break;
                                }
                            }
                        }
                    }
                    switch(roomnum1 / 100){
                        case 2:consu = price[standard];break;
                        case 3:consu = price[doublebed];break;
                        case 4:consu = price[largebed];break;
                        case 5:consu = price[business];break;
                        case 6:consu = price[vip];break;
                    }
                    cost = days * consu + 300;
                    if(isfull1 == 1){//该房间类型没有空房则提示
                        QMessageBox *messagebox7 = new QMessageBox;
                        QString str;
                        str += "所选房间类型房间已满，请选择其他类型房间     ";
                        messagebox7->setText(str);
                        messagebox7->exec();
                    }
                    else{
                        if(isfull1 == 0){//如果是空房则将房间状态改为checked
                            query.exec("UPDATE roomInfo SET status = 'checked' WHERE num = "+QString::number(roomnum1)+"");
                        }
                        else if(isfull1 == 2){//如果是已被预约的房间则将房间状态改为checkandbook
                            query.exec("UPDATE roomInfo SET status = 'checkandbook' WHERE num = "+QString::number(roomnum1)+"");
                        }
                        query.exec("UPDATE roomInfo SET checkInTime = "+QString::number(checkintime)+" WHERE num = "+QString::number(roomnum1)+"");
                        query.exec("UPDATE roomInfo SET checkOutTime = "+QString::number(checkouttime)+" WHERE num = "+QString::number(roomnum1)+"");
                        query.exec("UPDATE roomInfo SET checkInTimeText = '"+checkInTemp.toString(nowFormat)+"' WHERE num = "+QString::number(roomnum1)+"");
                        query.exec("UPDATE roomInfo SET checkOutTimeText = '"+checkOutTemp.toString(nowFormat)+"' WHERE num = "+QString::number(roomnum1)+"");
                        query.exec("UPDATE roomInfo SET cost = '"+QString::number(cost)+"' WHERE num = "+QString::number(roomnum1)+"");//更新房间信息表中各个状态
                        query.exec("INSERT INTO guestInfo (name, gender, idNum, workplace, phoneNum, consume, checkInTime, checkOutTime, checkInTimeText, checkOutTimeText, room)"
                             "VALUES('"+nametext+"', '"+sextext+"', '"+idtext+"', '"+workplacetext+"', '"+phonetext+"', 0, "+QString::number(checkintime)+", "+QString::number(checkouttime)+", '"+checkInTemp.toString(nowFormat)+"', '"+checkOutTemp.toString(nowFormat)+"', "+QString::number(roomnum1)+")");
                        //将顾客信息插入顾客信息表中
                        finalRoom=roomnum1;
                        checkinDialog::undeletereverse(cost);//弹出提示入住成功
                        this->hide();
                        logPrint("顾客 "+nametext+" 登记入住于 "+QString::number(finalRoom)+"房间");//将事件写入日志文件
                    }
                    break;
                }
                case doublebed:{
                    int roomnum2;
                    int isfull2 = 1;
                    int checkintime2;
                    int checkouttime2;
                    query.exec("SELECT num,status FROM roomInfo WHERE  num > 300 AND num < 400");
                    while(query.next()){
                        roomnum2 = query.value(0).toInt();
                        QString roomstatus = query.value(1).toString();
                        if(roomstatus == "empty"){
                            isfull2 = 0;
                            break;
                        }
                        else if(roomstatus == "booked"){
                            query.exec("SELECT checkInTime,checkOutTime,room FROM bookInfo WHERE room = "+QString::number(roomnum2)+"");
                            while(query.next()){
                                checkintime2 = query.value(0).toInt();
                                checkouttime2 = query.value(1).toInt();
                                if(checkouttime < checkintime2){
                                    isfull2 = 2;
                                    break;
                                }
                            }
                        }
                    }
                    switch(roomnum2 / 100){
                        case 2:consu = price[standard];break;
                        case 3:consu = price[doublebed];break;
                        case 4:consu = price[largebed];break;
                        case 5:consu = price[business];break;
                        case 6:consu = price[vip];break;
                    }
                    cost = days * consu + 300;
                    if(isfull2 == 1){
                        QMessageBox *messagebox8 = new QMessageBox;
                        QString str;
                        str += "所选房间类型房间已满，请选择其他类型房间     ";
                        messagebox8->setText(str);
                        messagebox8->exec();
                    }
                    else{
                        if(isfull2 == 0){
                            query.exec("UPDATE roomInfo SET status = 'checked' WHERE num = "+QString::number(roomnum2)+"");
                        }
                        else if(isfull2 == 2){
                            query.exec("UPDATE roomInfo SET status = 'checkandbook' WHERE num = "+QString::number(roomnum2)+"");
                        }
                        query.exec("UPDATE roomInfo SET checkInTime = "+QString::number(checkintime)+" WHERE num = "+QString::number(roomnum2)+"");
                        query.exec("UPDATE roomInfo SET checkOutTime = "+QString::number(checkouttime)+" WHERE num = "+QString::number(roomnum2)+"");
                        query.exec("UPDATE roomInfo SET checkInTimeText = '"+checkInTemp.toString(nowFormat)+"' WHERE num = "+QString::number(roomnum2)+"");
                        query.exec("UPDATE roomInfo SET checkOutTimeText = '"+checkOutTemp.toString(nowFormat)+"' WHERE num = "+QString::number(roomnum2)+"");
                        query.exec("UPDATE roomInfo SET cost = '"+QString::number(cost)+"' WHERE num = "+QString::number(roomnum2)+"");
                        query.exec("INSERT INTO guestInfo (name, gender, idNum, workplace, phoneNum, consume, checkInTime, checkOutTime, checkInTimeText, checkOutTimeText, room)"
                             "VALUES('"+nametext+"', '"+sextext+"', '"+idtext+"', '"+workplacetext+"', '"+phonetext+"', 0, "+QString::number(checkintime)+", "+QString::number(checkouttime)+", '"+checkInTemp.toString(nowFormat)+"', '"+checkOutTemp.toString(nowFormat)+"', "+QString::number(roomnum2)+")");
                        finalRoom=roomnum2;
                        checkinDialog::undeletereverse(cost);
                        QMessageBox *messagebox = new QMessageBox;
                        QString str;
                        str += "您还可以再次添加入住信息     ";
                        messagebox->setText(str);
                        messagebox->exec();
                        this->close();
                        checkindoubleDialog *checkindouble = new checkindoubleDialog(roomnum2,days,this);
                        checkindouble->exec();
                        logPrint("顾客 "+nametext+" 登记入住于 "+QString::number(finalRoom)+"房间");//将事件写入日志文件
                    }
                    break;
                }
                case largebed:{
                    int roomnum3;
                    int isfull3 = 1;
                    int checkintime3;
                    int checkouttime3;
                    query.exec("SELECT num,status FROM roomInfo WHERE  num > 400 AND num < 500");
                    while(query.next()){
                        roomnum3 = query.value(0).toInt();
                        QString roomstatus = query.value(1).toString();
                        if(roomstatus == "empty"){
                            isfull3 = 0;
                            break;
                        }
                        else if(roomstatus == "booked"){
                            query.exec("SELECT checkInTime,checkOutTime,room FROM bookInfo WHERE room = "+QString::number(roomnum3)+"");
                            while(query.next()){
                                checkintime3 = query.value(0).toInt();
                                checkouttime3 = query.value(1).toInt();
                                if(checkouttime < checkintime3){
                                    isfull3 = 2;
                                    break;
                                }
                            }
                        }
                    }
                    switch(roomnum3 / 100){
                        case 2:consu = price[standard];break;
                        case 3:consu = price[doublebed];break;
                        case 4:consu = price[largebed];break;
                        case 5:consu = price[business];break;
                        case 6:consu = price[vip];break;
                    }
                    cost = days * consu + 300;
                    if(isfull3 == 1){
                        QMessageBox *messagebox9 = new QMessageBox;
                        QString str;
                        str += "所选房间类型房间已满，请选择其他类型房间     ";
                        messagebox9->setText(str);
                        messagebox9->exec();
                    }
                    else{
                        if(isfull3 == 0){
                            query.exec("UPDATE roomInfo SET status = 'checked' WHERE num = "+QString::number(roomnum3)+"");
                        }
                        else if(isfull3 == 2){
                            query.exec("UPDATE roomInfo SET status = 'checkandbook' WHERE num = "+QString::number(roomnum3)+"");
                        }
                        query.exec("UPDATE roomInfo SET checkInTime = "+QString::number(checkintime)+" WHERE num = "+QString::number(roomnum3)+"");
                        query.exec("UPDATE roomInfo SET checkOutTime = "+QString::number(checkouttime)+" WHERE num = "+QString::number(roomnum3)+"");
                        query.exec("UPDATE roomInfo SET checkInTimeText = '"+checkInTemp.toString(nowFormat)+"' WHERE num = "+QString::number(roomnum3)+"");
                        query.exec("UPDATE roomInfo SET checkOutTimeText = '"+checkOutTemp.toString(nowFormat)+"' WHERE num = "+QString::number(roomnum3)+"");
                        query.exec("UPDATE roomInfo SET cost = '"+QString::number(cost)+"' WHERE num = "+QString::number(roomnum3)+"");
                        query.exec("INSERT INTO guestInfo (name, gender, idNum, workplace, phoneNum, consume, checkInTime, checkOutTime, checkInTimeText, checkOutTimeText, room)"
                             "VALUES('"+nametext+"', '"+sextext+"', '"+idtext+"', '"+workplacetext+"', '"+phonetext+"', 0, "+QString::number(checkintime)+", "+QString::number(checkouttime)+", '"+checkInTemp.toString(nowFormat)+"', '"+checkOutTemp.toString(nowFormat)+"', "+QString::number(roomnum3)+")");
                        finalRoom=roomnum3;

                        checkinDialog::undeletereverse(cost);

                        QMessageBox *messagebox = new QMessageBox;
                        QString str;
                        str += "您还可以再次添加入住信息     ";
                        messagebox->setText(str);
                        messagebox->exec();

                        this->close();
                        checkindoubleDialog *checkindouble = new checkindoubleDialog(roomnum3,days,this);
                        checkindouble->exec();


                        logPrint("顾客 "+nametext+" 登记入住于 "+QString::number(finalRoom)+"房间");//将事件写入日志文件
                    }
                    break;
                }
                case business:{
                    int roomnum4;
                    int isfull4 = 1;
                    int checkintime4;
                    int checkouttime4;
                    query.exec("SELECT num,status FROM roomInfo WHERE  num > 500 AND num < 600");
                    while(query.next()){
                        roomnum4 = query.value(0).toInt();
                        QString roomstatus = query.value(1).toString();
                        if(roomstatus == "empty"){
                            isfull4 = 0;
                            break;
                        }
                        else if(roomstatus == "booked"){
                            query.exec("SELECT checkInTime,checkOutTime,room FROM bookInfo WHERE room = "+QString::number(roomnum4)+"");
                            while(query.next()){
                                checkintime4 = query.value(0).toInt();
                                checkouttime4 = query.value(1).toInt();
                                if(checkouttime < checkintime4){
                                    isfull4 = 2;
                                    break;
                                }
                            }
                        }
                    }
                    switch(roomnum4 / 100){
                        case 2:consu = price[standard];break;
                        case 3:consu = price[doublebed];break;
                        case 4:consu = price[largebed];break;
                        case 5:consu = price[business];break;
                        case 6:consu = price[vip];break;
                    }
                    cost = days * consu + 300;
                    if(isfull4 == 1){
                        QMessageBox *messagebox10 = new QMessageBox;
                        QString str;
                        str += "所选房间类型房间已满，请选择其他类型房间     ";
                        messagebox10->setText(str);
                        messagebox10->exec();
                    }
                    else{
                        if(isfull4 == 0){
                            query.exec("UPDATE roomInfo SET status = 'checked' WHERE num = "+QString::number(roomnum4)+"");
                        }
                        else if(isfull4 == 2){
                            query.exec("UPDATE roomInfo SET status = 'checkandbook' WHERE num = "+QString::number(roomnum4)+"");
                        }
                        query.exec("UPDATE roomInfo SET status = 'checked' WHERE num = "+QString::number(roomnum4)+"");
                        query.exec("UPDATE roomInfo SET checkInTime = "+QString::number(checkintime)+" WHERE num = "+QString::number(roomnum4)+"");
                        query.exec("UPDATE roomInfo SET checkOutTime = "+QString::number(checkouttime)+" WHERE num = "+QString::number(roomnum4)+"");
                        query.exec("UPDATE roomInfo SET checkInTimeText = '"+checkInTemp.toString(nowFormat)+"' WHERE num = "+QString::number(roomnum4)+"");
                        query.exec("UPDATE roomInfo SET checkOutTimeText = '"+checkOutTemp.toString(nowFormat)+"' WHERE num = "+QString::number(roomnum4)+"");
                        query.exec("UPDATE roomInfo SET cost = '"+QString::number(cost)+"' WHERE num = "+QString::number(roomnum4)+"");
                        query.exec("INSERT INTO guestInfo (name, gender, idNum, workplace, phoneNum, consume, checkInTime, checkOutTime, checkInTimeText, checkOutTimeText, room)"
                             "VALUES('"+nametext+"', '"+sextext+"', '"+idtext+"', '"+workplacetext+"', '"+phonetext+"', 0, "+QString::number(checkintime)+", "+QString::number(checkouttime)+", '"+checkInTemp.toString(nowFormat)+"', '"+checkOutTemp.toString(nowFormat)+"', "+QString::number(roomnum4)+")");
                        finalRoom=roomnum4;
                        checkinDialog::undeletereverse(cost);
                        this->hide();
                        logPrint("顾客 "+nametext+" 登记入住于 "+QString::number(finalRoom)+"房间");//将事件写入日志文件
                    }
                    break;
                }
                case vip:{
                    int roomnum5;
                    int isfull5 = 1;
                    int checkintime5;
                    int checkouttime5;
                    query.exec("SELECT num,status FROM roomInfo WHERE  num > 600 AND num < 700");
                    while(query.next()){
                        roomnum5 = query.value(0).toInt();
                        QString roomstatus = query.value(1).toString();
                        if(roomstatus == "empty"){
                            isfull5 = 0;
                            break;
                        }
                        else if(roomstatus == "booked"){
                            query.exec("SELECT checkInTime,checkOutTime,room FROM bookInfo WHERE room = "+QString::number(roomnum5)+"");
                            while(query.next()){
                                checkintime5 = query.value(0).toInt();
                                checkouttime5 = query.value(1).toInt();
                                if(checkouttime < checkintime5){
                                    isfull5 = 2;
                                    break;
                                }
                            }
                        }
                    }
                    switch(roomnum5 / 100){
                        case 2:consu = price[standard];break;
                        case 3:consu = price[doublebed];break;
                        case 4:consu = price[largebed];break;
                        case 5:consu = price[business];break;
                        case 6:consu = price[vip];break;
                    }
                    cost = days * consu + 300;
                    if(isfull5 == 1){
                        QMessageBox *messagebox11 = new QMessageBox;
                        QString str;
                        str += "所选房间类型房间已满，请选择其他类型房间     ";
                        messagebox11->setText(str);
                        messagebox11->exec();
                    }
                    else{
                        if(isfull5 == 0){
                            query.exec("UPDATE roomInfo SET status = 'checked' WHERE num = "+QString::number(roomnum5)+"");
                        }
                        else if(isfull5 == 2){
                            query.exec("UPDATE roomInfo SET status = 'checkandbook' WHERE num = "+QString::number(roomnum5)+"");
                        }
                        query.exec("UPDATE roomInfo SET status = 'checked' WHERE num = "+QString::number(roomnum5)+"");
                        query.exec("UPDATE roomInfo SET checkInTime = "+QString::number(checkintime)+" WHERE num = "+QString::number(roomnum5)+"");
                        query.exec("UPDATE roomInfo SET checkOutTime = "+QString::number(checkouttime)+" WHERE num = "+QString::number(roomnum5)+"");
                        query.exec("UPDATE roomInfo SET checkInTimeText = '"+checkInTemp.toString(nowFormat)+"' WHERE num = "+QString::number(roomnum5)+"");
                        query.exec("UPDATE roomInfo SET checkOutTimeText = '"+checkOutTemp.toString(nowFormat)+"' WHERE num = "+QString::number(roomnum5)+"");
                        query.exec("UPDATE roomInfo SET cost = '"+QString::number(cost)+"' WHERE num = "+QString::number(roomnum5)+"");
                        query.exec("INSERT INTO guestInfo (name, gender, idNum, workplace, phoneNum, consume, checkInTime, checkOutTime, checkInTimeText, checkOutTimeText, room)"
                             "VALUES('"+nametext+"', '"+sextext+"', '"+idtext+"', '"+workplacetext+"', '"+phonetext+"', 0, "+QString::number(checkintime)+", "+QString::number(checkouttime)+", '"+checkInTemp.toString(nowFormat)+"', '"+checkOutTemp.toString(nowFormat)+"', "+QString::number(roomnum5)+")");
                        finalRoom=roomnum5;
                        checkinDialog::undeletereverse(cost);
                        this->hide();
                        logPrint("顾客 "+nametext+" 登记入住于 "+QString::number(finalRoom)+"房间");//将事件写入日志文件
                    }
                    break;
                }
            }

        }
    }

}

void checkinDialog::setman()//确保性别选项只能选择一个
{
    ui->checkBoxman->setChecked(TRUE);
    ui->checkBoxwoman->setChecked(FALSE);
}

void checkinDialog::setwoman()//确保性别选项只能选择一个
{
    ui->checkBoxman->setChecked(FALSE);
    ui->checkBoxwoman->setChecked(TRUE);
}

void checkinDialog::setno()//确保是否预约选项只能选择一个
{
    ui->checkBoxno->setChecked(TRUE);
    ui->checkBoxyes->setChecked(FALSE);
}

void checkinDialog::setyes()//确保是否预约选项只能选择一个
{
    ui->checkBoxno->setChecked(FALSE);
    ui->checkBoxyes->setChecked(TRUE);
}

void checkinDialog::deletereverse(int consume)//提示信息
{
    QMessageBox *messagebox1 = new QMessageBox;
    QString str;
    str += "已从预约信息中删除并成功登记     \n";
    str += "押金共 ";
    str += QString::number(consume);
    str += " 元";
    messagebox1->setText(str);
    messagebox1->exec();
}

void checkinDialog::undeletereverse(int consume)//提示信息
{
    QMessageBox *messagebox2 = new QMessageBox;
    QString str;
    str += "已成功登记     \n";
    str += "押金共 ";
    str += QString::number(consume);
    str += " 元";
    messagebox2->setText(str);
    messagebox2->exec();
}
