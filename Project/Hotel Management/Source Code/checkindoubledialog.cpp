#include "checkindoubledialog.h"
#include "mainwindow.h"
#include "datastruct.h"
#include <QtSql>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QtGui>
#include <QDateTime>
#include <QTimer>
#include <QSpinBox>
#include <QString>

extern int now;
extern QString nowFormat;
void logPrint(QString str);

checkindoubleDialog::checkindoubleDialog(int roomnum,int days, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkindoubleDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("登记入住"));
    ui->spinBoxdays->setRange(1,100);
    ui->spinBoxdays->setValue(1);
    QRegExp regExp1("[0-9]{17}[0-9A-Za-z]");
    QRegExp regExp2("[0-9]{11}");
    ui->lineEditid->setValidator(new QRegExpValidator(regExp1,this));//控制身份证号输入格式
    ui->lineEditphone->setValidator(new QRegExpValidator(regExp2,this));//控制手机号输入格式
    ui->labelnum->setText(QString::number(roomnum));
    ui->spinBoxdays->setValue(days);
    ui->spinBoxdays->setMaximum(days);
    ui->spinBoxdays->setMinimum(days);
    QDateTime temp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
    temp=temp.addSecs(qint64(now));
    ui->dateEditCheckIn->setDate(temp.date());
    ui->dateEditCheckIn->setMinimumDate(temp.date());
    ui->dateEditCheckIn->setMaximumDate(temp.date());

    connect(ui->checkBoxman,SIGNAL(clicked()),this,SLOT(setman()));
    connect(ui->checkBoxwoman,SIGNAL(clicked()),this,SLOT(setwoman()));
    connect(ui->pushButtoncheckin1,SIGNAL(clicked()),this,SLOT(checkinto1()));
    connect(ui->pushButtoncancel1,SIGNAL(clicked()),this,SLOT(close()));
}

void checkindoubleDialog::checkinto1()//登记
{
    qDebug()<<"111";
    int roomnum = ui->labelnum->text().toInt();
    QString nametext = ui->lineEditname->text();
    QString workplacetext = ui->lineEditworkplace->text();
    QString idtext = ui->lineEditid->text();
    QString phonetext = ui->lineEditphone->text();
    QString sextext;
    if(ui->checkBoxman->checkState() == Qt::Checked){//选中checkbox
        sextext = "男";
    }
    else if(ui->checkBoxwoman->checkState() == Qt::Checked){
        sextext = "女";
    }
    int days = ui->spinBoxdays->value();
    int checkintime = now;//获取当前时间作为入住时间
    int checkouttime = now-now%(24*60*60)+24*60*60*days+12*60*60;//离店时间
    QDateTime checkInTemp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
    checkInTemp=checkInTemp.addSecs(qint64(checkintime));
    QDateTime checkOutTemp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
    checkOutTemp=checkOutTemp.addSecs(qint64(checkouttime));
    qDebug()<<QString::number(roomnum);
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
        query.exec("INSERT INTO guestInfo (name, gender, idNum, workplace, phoneNum, consume, checkInTime, checkOutTime, checkInTimeText, checkOutTimeText, room)"
             "VALUES('"+nametext+"', '"+sextext+"', '"+idtext+"', '"+workplacetext+"', '"+phonetext+"', 0, "+QString::number(checkintime)+", "+QString::number(checkouttime)+", '"+checkInTemp.toString(nowFormat)+"', '"+checkOutTemp.toString(nowFormat)+"', "+QString::number(roomnum)+")");
        //将顾客信息插入顾客信息表中
        query.lastError();
        checkindoubleDialog::checkinsucess();//弹出提示入住成功
        this->hide();//退出时隐藏此窗口
        logPrint("顾客 "+nametext+" 登记入住于 "+QString::number(roomnum)+" 房间");//将事件写入日志文件
    }

}


void checkindoubleDialog::setman()//确保性别选项只能选择一个
{
    ui->checkBoxman->setChecked(TRUE);
    ui->checkBoxwoman->setChecked(FALSE);
}

void checkindoubleDialog::setwoman()//确保性别选项只能选择一个
{
    ui->checkBoxman->setChecked(FALSE);
    ui->checkBoxwoman->setChecked(TRUE);
}

void checkindoubleDialog::checkinsucess()//提示信息
{
    QMessageBox *messagebox = new QMessageBox;
    QString str;
    str += "添加成功     ";
    messagebox->setText(str);
    messagebox->exec();
}


