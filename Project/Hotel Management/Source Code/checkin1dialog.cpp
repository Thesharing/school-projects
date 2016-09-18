#include "checkin1dialog.h"
#include "roominfodialog.h"
#include "mainwindow.h"
#include "datastruct.h"
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


extern int now;
extern QString roomNum;
extern QString nowFormat;
void logPrint(QString str);

extern int price[5];

checkin1Dialog::checkin1Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkin1Dialog)
{//点击房间按钮弹出人物信息界面的登记按钮弹出此窗口
    ui->setupUi(this);
    setWindowTitle(tr("登记入住"));
    ui->spinBoxdays->setRange(1,100);
    ui->spinBoxdays->setValue(1);
    QRegExp regExp1("[0-9]{17}[0-9A-Za-z]");
    QRegExp regExp2("[0-9]{11}");
    ui->lineEditid->setValidator(new QRegExpValidator(regExp1,this));//控制身份证号输入格式
    ui->lineEditphone->setValidator(new QRegExpValidator(regExp2,this));//控制手机号输入格式
    ui->labelnum->setText(roomNum);

    QDateTime temp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
    temp=temp.addSecs(qint64(now));
    ui->dateEditCheckIn->setDate(temp.date());
    ui->dateEditCheckIn->setMinimumDate(temp.date());
    ui->dateEditCheckIn->setMaximumDate(temp.date());

    connect(ui->checkBoxman,SIGNAL(clicked()),this,SLOT(setman()));
    connect(ui->checkBoxwoman,SIGNAL(clicked()),this,SLOT(setwoman()));
    connect(ui->pushButtoncheckin,SIGNAL(clicked()),this,SLOT(checkinto()));
    connect(ui->pushButtoncancel,SIGNAL(clicked()),this,SLOT(close()));
}

void checkin1Dialog::checkinto()//登记
{
    QString nametext = ui->lineEditname->text();
    QString workplacetext = ui->lineEditworkplace->text();
    QString idtext = ui->lineEditid->text();
    QString phonetext = ui->lineEditphone->text();
    QString sextext;
    bool sex;
    if(ui->checkBoxman->checkState() == Qt::Checked){//选中checkbox
        sex = TRUE;
        sextext = "男";
    }
    else if(ui->checkBoxwoman->checkState() == Qt::Checked){
        sex = FALSE;
        sextext = "女";
    }
    int days = ui->spinBoxdays->value();
    int checkintime = now;//获取当前时间作为入住时间
    int checkouttime = now-now%(24*60*60)+24*60*60*days+12*60*60;//离店时间
    QDateTime checkInTemp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
    checkInTemp=checkInTemp.addSecs(qint64(checkintime));
    QDateTime checkOutTemp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
    checkOutTemp=checkOutTemp.addSecs(qint64(checkouttime));

    if(nametext.isEmpty() || idtext.isEmpty() || workplacetext.isEmpty() || phonetext.isEmpty() ||
            (ui->checkBoxman->checkState() != Qt::Checked && ui->checkBoxwoman->checkState() != Qt::Checked))
    {//如果有信息未输入则提示
        QMessageBox *messagebox3 = new QMessageBox;
        QString str;
        str += "输入信息不完整！！     ";
        messagebox3->setText(str);
        messagebox3->exec();

    }
    else{
        QSqlQuery query;
        int consu,cost;
        switch(roomNum.toInt() / 100){
            case 2:consu = price[standard];break;
            case 3:consu = price[doublebed];break;
            case 4:consu = price[largebed];break;
            case 5:consu = price[business];break;
            case 6:consu = price[vip];break;
        }
        cost = days * consu + 300;

        query.exec("UPDATE roomInfo SET status = 'checked' WHERE num = "+roomNum+"");//根据房间号检索房间信息表，将该房间状态置为checked
        query.exec("UPDATE roomInfo SET checkInTime = "+QString::number(checkintime)+" WHERE num = "+roomNum+"");
        query.exec("UPDATE roomInfo SET checkOutTime = "+QString::number(checkouttime)+" WHERE num = "+roomNum+"");
        query.exec("UPDATE roomInfo SET checkInTimeText = '"+checkInTemp.toString(nowFormat)+"' WHERE num = "+roomNum+"");
        query.exec("UPDATE roomInfo SET checkOutTimeText = '"+checkOutTemp.toString(nowFormat)+"' WHERE num = "+roomNum+"");
        query.exec("UPDATE roomInfo SET cost = '"+QString::number(cost)+"' WHERE num = "+roomNum+"");//更新房间信息表中各个状态
        query.exec("INSERT INTO guestInfo (name, gender, idNum, workplace, phoneNum, consume, checkInTime, checkOutTime, checkInTimeText, checkOutTimeText, room)"
             "VALUES('"+nametext+"', '"+sextext+"', '"+idtext+"', '"+workplacetext+"', '"+phonetext+"', 0, "+QString::number(checkintime)+", "+QString::number(checkouttime)+", '"+checkInTemp.toString(nowFormat)+"', '"+checkOutTemp.toString(nowFormat)+"', "+roomNum+")");
        //将顾客信息插入顾客信息表中
        checkin1Dialog::checkinsucess(cost);//弹出提示入住成功
        if(roomNum.toInt() / 100 == 3 || roomNum.toInt() / 100 == 4){
            QMessageBox *messagebox = new QMessageBox;
            QString str;
            str += "您还可以再次添加入住信息     ";
            messagebox->setText(str);
            messagebox->exec();
            this->close();//退出时隐藏此窗口
            checkindoubleDialog *checkindouble = new checkindoubleDialog(roomNum.toInt(),days,this);
            checkindouble->exec();
        }
        else{
            this->close();
        }

        logPrint("顾客 "+nametext+" 登记入住于 "+roomNum+"房间");//将事件写入日志文件
    }

}


void checkin1Dialog::setman()//确保性别选项只能选择一个
{
    ui->checkBoxman->setChecked(TRUE);
    ui->checkBoxwoman->setChecked(FALSE);
}

void checkin1Dialog::setwoman()//确保性别选项只能选择一个
{
    ui->checkBoxman->setChecked(FALSE);
    ui->checkBoxwoman->setChecked(TRUE);
}

void checkin1Dialog::checkinsucess(int consume)//提示信息
{
    QMessageBox *messagebox = new QMessageBox;
    QString str;
    str += "已成功登记     \n";
    str += "押金共 ";
    str += QString::number(consume);
    str += " 元";
    messagebox->setText(str);
    messagebox->exec();
}
