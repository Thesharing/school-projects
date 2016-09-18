#include "checkout.h"
#include "datastruct.h"
#include <QtGui>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QRegExp>

void logPrint(QString str);
extern int price[5];
extern int now;

int spend;
int cost;
checkoutDialog::checkoutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkoutDialog)

{//点击主界面离店按钮弹出此窗口
    ui->setupUi(this);
    setFixedSize(290,180);//控制窗口大小
    setWindowTitle(tr("离店结算"));
    ui->pushButtoncheckout->setEnabled(FALSE);//设置按钮不可编辑
    QRegExp regExp3("[2-6][0-1][0-9]");
    ui->lineEditnumber->setValidator(new QRegExpValidator(regExp3,this));//控制房间号输入格式

    connect(ui->lineEditnumber,SIGNAL(textChanged(const QString&)),this,SLOT(enablepushButtoncheckout(const QString&)));
    connect(ui->pushButtoncheckout,SIGNAL(clicked()),this,SLOT(checkoutto()));
    connect(ui->pushButtoncancel,SIGNAL(clicked()),this,SLOT(close()));

}

void checkoutDialog::checkoutto()//离店
{
    QString roomnumbertext = ui->lineEditnumber->text();
    int roomnumber = roomnumbertext.toInt();
    int checkintimeguest;
    int checkouttimeguest;
    int temp;
    int consu;
    double days;
    QRegExp regExp3("[2-6][0-1][0-9]");
    if(!regExp3.exactMatch(roomnumbertext) || roomnumber == 200 || roomnumber == 300 || roomnumber == 400 || roomnumber == 500
            || roomnumber == 600 || roomnumber == 219 || roomnumber == 319 || roomnumber == 419 || roomnumber == 519 || roomnumber == 619)
    {//如果输入房间号为错误房间号则提示
        QMessageBox *messagebox5 = new QMessageBox;
        QString str;
        str += "输入信息有误     ";
        messagebox5->setText(str);
        messagebox5->exec();
    }
    else{
        QSqlQuery query;
        query.exec("SELECT num,status FROM roomInfo WHERE num = "+QString::number(roomnumber)+"");//根据房间号检索房间信息表
        query.next();
        QString roomstatus = query.value(1).toString();//获取该房间的状态
        if(roomstatus == "empty"){//如果需要离店的房间为空房则提示
            QMessageBox *messagebox6 = new QMessageBox;
            QString str;
            str += "所选房间为空房     ";
            messagebox6->setText(str);
            messagebox6->exec();
        }
        else{
            query.exec("SELECT consume,checkInTime,room FROM guestInfo WHERE room = "+QString::number(roomnumber)+"");
            query.next();
            checkintimeguest = query.value(1).toInt();//获取客户入住时间
            temp = checkintimeguest%(24*60*60);
            checkintimeguest = checkintimeguest-temp;//将入住时间变为日期，抹去后面的时，分，秒
            checkouttimeguest = now-now%(24*60*60);//获取当前时间抹去时，分，秒变为离店日期
            days = (checkouttimeguest - checkintimeguest) / (24*60*60);//计算出客户入住的天数
            query.exec("SELECT num,cost FROM roomInfo WHERE num = "+QString::number(roomnumber)+"");
            query.next();
            cost = query.value(1).toInt();
            if(days < 0 || (days == 0 && now%(24*60*60) < temp)){//如果天数不正确则提示
                QMessageBox *messagebox12 = new QMessageBox;
                QString str;
                str += "输入信息有误     ";
                messagebox12->setText(str);
                messagebox12->exec();
            }
            else{
                if(roomstatus == "checked"){//如果房间状态为checked则将其变为empty
                    query.exec("UPDATE roomInfo SET status = 'empty' WHERE num = "+QString::number(roomnumber)+"");
                }
                else if(roomstatus == "checkandbook"){//如果房间状态为checkandbook则将其变为booked
                    query.exec("UPDATE roomInfo SET status = 'booked' WHERE num = "+QString::number(roomnumber)+"");
                }
                query.exec("UPDATE roomInfo SET checkInTime = 0 WHERE num = "+QString::number(roomnumber)+"");
                query.exec("UPDATE roomInfo SET checkOutTime = 0 WHERE num = "+QString::number(roomnumber)+"");
                query.exec("UPDATE roomInfo SET checkInTimeText = '' WHERE num = "+QString::number(roomnumber)+"");
                query.exec("UPDATE roomInfo SET checkOutTimeText = '' WHERE num = "+QString::number(roomnumber)+"");
                query.exec("UPDATE roomInfo SET cost = 0 WHERE num = "+QString::number(roomnumber)+"");//更新房间信息表中各个状态
                if(days == 0 && now%(24*60*60) < 12*60*60){//如果顾客当天入住且当天12点前退房则入住天数按半天计
                    days = days + 0.5;
                }
                else if(now%(24*60*60) < 12*60*60){//如果顾客当天入住且隔天或之后几天12点前退房则入住天数不变
                    days = days;
                }
                else if(now%(24*60*60) >= 12*60*60 && now%(24*60*60) < 18*60*60){//如果顾客12点后18点前退房则入住天数加半天
                    days = days + 0.5;
                }
                else if(now%(24*60*60) >= 18*60*60){//如果顾客18点后退房则入住天数加一天
                    days = days + 1;
                }
                switch(roomnumber / 100){//得到各个房间类型的房价
                    case 2:consu = price[standard];break;
                    case 3:consu = price[doublebed];break;
                    case 4:consu = price[largebed];break;
                    case 5:consu = price[business];break;
                    case 6:consu = price[vip];break;
                }
                spend = consu * days;//计算顾客总消费
                query.exec("DELETE FROM guestInfo WHERE room = "+QString::number(roomnumber)+"");//从顾客信息表中删除该顾客信息
                checkoutDialog::checkoutsuccess();//提示信息
                logPrint("房间 "+QString::number(roomnumber)+" 已退房");//将事件写入日志文件
                this->hide();
            }
        }
    }
}

void checkoutDialog::checkoutsuccess()//提示信息
{
    QMessageBox *messagebox4 = new QMessageBox;
    QString str;
    QString spendd;
    spendd = QString::number(spend);
    if(spend >= cost){
        int xiaofei = spend - cost;
        QString xiaofei1 = QString::number(xiaofei);
        str += "离店成功          \n";
        str += "押金共 ";
        str += QString::number(cost);
        str += " 元      \n";
        str += "总计消费 ";
        str += spendd;
        str += " 元      \n";
        str += "还需支付 ";
        str += xiaofei1;
        str += " 元               \n";
    }
    else if(spend < cost){
        int xiaofei = cost - spend;
        QString xiaofei1 = QString::number(xiaofei);
        str += "离店成功          \n";
        str += "押金共 ";
        str += QString::number(cost);
        str += " 元      \n";
        str += "总计消费 ";
        str += spendd;
        str += " 元      \n";
        str += "共需退还 ";
        str += xiaofei1;
        str += " 元               \n";
    }


    messagebox4->setText(str);
    messagebox4->exec();
}

void checkoutDialog::enablepushButtoncheckout(const QString &text)//控制离店按钮可编辑
{
    ui->pushButtoncheckout->setEnabled(!text.isEmpty());

}

