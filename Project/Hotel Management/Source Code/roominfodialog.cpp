#include "roominfodialog.h"
#include "ui_roominfodialog.h"
#include "checkin1dialog.h"
#include "mainwindow.h"
#include "datastruct.h"
#include <QtSql>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>

QString roomNum;
int spend1;
int cost1;
extern int now;
void logPrint(QString str);
extern int price[5];

extern int stringToIntForType(QString typeStr);
QString roomInfoConv(QString str);

roominfodialog::roominfodialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::roominfodialog)
{
    ui->setupUi(this);
    //这里提供指定房间入住和离店功能
    connect(ui->checkInButton,SIGNAL(clicked()),this,SLOT(checkin1()));
    connect(ui->checkOutButton,SIGNAL(clicked()),this,SLOT(checkout1()));
}



void roominfodialog::updateRoomInfoDialog(int num){
    //这里显示房间的信息
    //显示房间号，房间类型，房间价格，房间状态
    ui->labelNum->setText("房间号"+QString::number(num));
    QSqlQuery query;
    query.exec("SELECT type, status, cost FROM roomInfo WHERE num = "+QString::number(num));
    while(query.next()){
        ui->labelNum->setText("房间号："+QString::number(num));
        ui->labelType->setText("房间类型："+roomInfoConv(query.value(0).toString()));
        ui->labelPrice->setText("房间价格："+QString::number(stringToIntForType(query.value(0).toString()))+"元");
        ui->labelStatus->setText("房间状态："+roomInfoConv(query.value(1).toString()));
        //如果房间为“空房”，则可以入住
        if(query.value(1).toString()=="empty"){
            ui->checkInButton->setEnabled(true);
            ui->checkOutButton->setEnabled(false);
        }
        //如果房间为“入住”，则可以退房
        else if(query.value(1).toString()=="checked"){
            ui->checkInButton->setEnabled(false);
            ui->checkOutButton->setEnabled(true);
        }
        //如果其他状态则不提供入住/退房
        else{
            ui->checkInButton->setEnabled(false);
            ui->checkOutButton->setEnabled(false);
        }
        //如果房间为空则只提示房间为空
        if(query.value(1).toString()=="empty" || query.value(1).toString()=="booked"){
            ui->labelName->setText("目前房间为空。");
            ui->labelID->setText(" ");
            ui->labelGender->setText(" ");
            ui->labelWorkplace->setText(" ");
            ui->labelPhoneNum->setText(" ");
            ui->labelCost->setText(" ");
            ui->labelCheckIn->setText(" ");
            ui->labelCheckOut->setText(" ");
        }
        else{
            //查询住客的信息并显示出来
            query.exec("SELECT name, gender, idNum, workplace, phoneNum, consume, checkInTimeText, checkOutTimeText FROM guestInfo WHERE room = "+QString::number(num));
            if(query.next()){
                ui->labelName->setText("姓名："+query.value(0).toString());
                ui->labelID->setText("身份证号："+query.value(2).toString());
                ui->labelGender->setText("性别："+query.value(1).toString());
                ui->labelWorkplace->setText("工作单位："+query.value(3).toString());
                ui->labelPhoneNum->setText("手机号："+query.value(4).toString());
                ui->labelCost->setText("已消费金额："+query.value(5).toString()+"元");
                ui->labelCheckIn->setText("入住时间："+query.value(6).toString());
                ui->labelCheckOut->setText("离店时间："+query.value(7).toString());
            }
        }
    }
}

QString roomInfoConv(QString str){
    //将英文的房间状态信息转换为中文以便显示
    switch(str.at(0).toLatin1()){
        case 'e':
            return "空房";
            break;
        case 'c':
            if(str.at(5).toLatin1()=='e')
                return "已入住";
            else
                return "已入住且有预约";
            break;
        case 'b':
            if(str.at(1).toLatin1()=='o')
                return "已预约";
            else
                return "商务间";
            break;
        case 's':
            return "标准间";
            break;
        case 'd':
            return "双人间";
            break;
        case 'v':
            return "贵宾间";
            break;
        case 'l':
            return "大床房";
            break;
        default:
            return " ";
    }
}

void roominfodialog::checkin1()
{

    roomNum = ui->labelNum->text();
    roomNum = roomNum.right(roomNum.length() - 4);
    checkin1Dialog *checkin11 = new checkin1Dialog(this);
    checkin11->exec();
    this->hide();
}

void roominfodialog::checkout1()
{
    roomNum = ui->labelNum->text();
    roomNum = roomNum.right(roomNum.length() - 4);

    int checkintimeguest;
    int checkouttimeguest;
    int temp;
    int consu;
    double days;
    QSqlQuery query;
    query.exec("SELECT num,status FROM roomInfo WHERE num = "+roomNum+"");
    query.next();
    QString roomstatus = query.value(1).toString();
    query.exec("SELECT consume,checkInTime,room FROM guestInfo WHERE room = "+roomNum+"");
    query.next();
    spend1 = query.value(0).toInt();
    checkintimeguest = query.value(1).toInt();
    temp = checkintimeguest%(24*60*60);
    checkintimeguest = checkintimeguest-temp;
    checkouttimeguest = now-now%(24*60*60);
    days = (checkouttimeguest - checkintimeguest) / (24*60*60);
    query.exec("SELECT num,cost FROM roomInfo WHERE num = "+roomNum+"");
    query.next();
    cost1 = query.value(1).toInt();


    if(days < 0 || (days == 0 && now%(24*60*60) < temp)){
        QMessageBox *messagebox12 = new QMessageBox;
        QString str;
        str += "信息有误     ";
        messagebox12->setText(str);
        messagebox12->exec();
    }
    else{
        if(roomstatus == "checked"){
            query.exec("UPDATE roomInfo SET status = 'empty' WHERE num = "+roomNum+"");
        }
        else if(roomstatus == "checkandbook"){
            query.exec("UPDATE roomInfo SET status = 'booked' WHERE num = "+roomNum+"");
        }
        query.exec("UPDATE roomInfo SET checkInTime = 0 WHERE num = "+roomNum+"");
        query.exec("UPDATE roomInfo SET checkOutTime = 0 WHERE num = "+roomNum+"");
        query.exec("UPDATE roomInfo SET checkInTimeText = '' WHERE num = "+roomNum+"");
        query.exec("UPDATE roomInfo SET checkOutTimeText = '' WHERE num = "+roomNum+"");
        query.exec("UPDATE roomInfo SET cost = 0 WHERE num = "+roomNum+"");
        if(days == 0 && now%(24*60*60) < 12*60*60){
            days = days + 0.5;
        }
        else if(now%(24*60*60) < 12*60*60){
            days = days;
        }
        else if(now%(24*60*60) >= 12*60*60 && now%(24*60*60) < 18*60*60){
            days = days + 0.5;
        }
        else if(now%(24*60*60) >= 18*60*60){
            days = days + 1;
        }
        switch(roomNum.toInt() / 100){
            case 2:consu = price[standard];break;
            case 3:consu = price[doublebed];break;
            case 4:consu = price[largebed];break;
            case 5:consu = price[business];break;
            case 6:consu = price[vip];break;
        }
        spend1 = consu * days;
        query.exec("DELETE FROM guestInfo WHERE room = "+roomNum+"");
        QMessageBox *messagebox4 = new QMessageBox;
        QString str;
        QString spendd;
        spendd = QString::number(spend1);
        if(spend1 >= cost1){
            int xiaofei = spend1 - cost1;
            QString xiaofei1 = QString::number(xiaofei);
            str += "离店成功          \n";
            str += "押金共 ";
            str += QString::number(cost1);
            str += " 元      \n";
            str += "总计消费 ";
            str += spendd;
            str += " 元      \n";
            str += "还需支付 ";
            str += xiaofei1;
            str += " 元                \n";
        }
        else if(spend1 < cost1){
            int xiaofei = cost1 - spend1;
            QString xiaofei1 = QString::number(xiaofei);
            str += "离店成功          \n";
            str += "押金共 ";
            str += QString::number(cost1);
            str += " 元      \n";
            str += "总计消费 ";
            str += spendd;
            str += " 元      \n";
            str += "共需退还 ";
            str += xiaofei1;
            str += " 元                \n";
        }

        messagebox4->setText(str);
        messagebox4->exec();
        logPrint("房间 "+roomNum+" 已退房");
        this->hide();
    }

}


void roominfodialog::on_okButton_clicked()
{
    roominfodialog::close();
}


roominfodialog::~roominfodialog()
{
    delete ui;
}
