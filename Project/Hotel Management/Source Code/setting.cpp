#include "setting.h"
#include "ui_setting.h"
#include "datastruct.h"
#include <QRegExp>
#include <QMessageBox>


extern int price[5];
extern int itemPrice[5];
extern int searchmodel;
extern int interval;

extern void logPrint(QString str);

setting::setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setting)
{//点击主界面系统设置弹出此窗口
    ui->setupUi(this);
    QRegExp regExp("[1-9][0-9]{3}");
    ui->lineEditRoom1->setValidator(new QRegExpValidator(regExp,this));
    ui->lineEditRoom2->setValidator(new QRegExpValidator(regExp,this));
    ui->lineEditRoom3->setValidator(new QRegExpValidator(regExp,this));
    ui->lineEditRoom4->setValidator(new QRegExpValidator(regExp,this));
    ui->lineEditRoom5->setValidator(new QRegExpValidator(regExp,this));
    ui->lineEditConsume1->setValidator(new QRegExpValidator(regExp,this));
    ui->lineEditConsume2->setValidator(new QRegExpValidator(regExp,this));
    ui->lineEditConsume3->setValidator(new QRegExpValidator(regExp,this));
    ui->lineEditConsume4->setValidator(new QRegExpValidator(regExp,this));
    ui->lineEditConsume5->setValidator(new QRegExpValidator(regExp,this));
    ui->lineEditRoom1->setText(QString::number(price[standard]));
    ui->lineEditRoom2->setText(QString::number(price[doublebed]));
    ui->lineEditRoom3->setText(QString::number(price[largebed]));
    ui->lineEditRoom4->setText(QString::number(price[business]));
    ui->lineEditRoom5->setText(QString::number(price[vip]));
    ui->lineEditConsume1->setText(QString::number(itemPrice[0]));
    ui->lineEditConsume2->setText(QString::number(itemPrice[1]));
    ui->lineEditConsume3->setText(QString::number(itemPrice[2]));
    ui->lineEditConsume4->setText(QString::number(itemPrice[3]));
    ui->lineEditConsume5->setText(QString::number(itemPrice[4]));//控制输入格式
    ui->lineEditTime->setText(QString::number(interval));
    if(searchmodel == 1)
        ui->checkBoxSearch->setCheckState(Qt::Checked);//如果searchmodel为1则为模糊搜索，置checkbox状态为被点击
    else if(searchmodel == 0)
        ui->checkBoxSearch->setCheckState(Qt::Unchecked);//如果searchmodel为0则为模糊搜索，置checkbox状态为空
    connect(ui->resetButton,SIGNAL(clicked()),this,SLOT(pricereset()));
    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(pricechange()));
}

void setting::pricereset()//设置系统设置为默认值
{
    ui->lineEditRoom1->setText(QString::number(200));
    ui->lineEditRoom2->setText(QString::number(300));
    ui->lineEditRoom3->setText(QString::number(300));
    ui->lineEditRoom4->setText(QString::number(400));
    ui->lineEditRoom5->setText(QString::number(800));
    ui->lineEditConsume1->setText(QString::number(1));
    ui->lineEditConsume2->setText(QString::number(3));
    ui->lineEditConsume3->setText(QString::number(5));
    ui->lineEditConsume4->setText(QString::number(8));
    ui->lineEditConsume5->setText(QString::number(10));
    ui->lineEditTime->setText(QString::number(100));
    ui->checkBoxSearch->setCheckState(Qt::Checked);
}

void setting::pricechange()//改变系统设置信息
{
    if(ui->lineEditRoom1->text().isEmpty() || ui->lineEditRoom2->text().isEmpty() || ui->lineEditRoom3->text().isEmpty()
            || ui->lineEditRoom4->text().isEmpty() || ui->lineEditRoom5->text().isEmpty() || ui->lineEditConsume1->text().isEmpty()
            || ui->lineEditConsume2->text().isEmpty() || ui->lineEditConsume3->text().isEmpty() || ui->lineEditConsume4->text().isEmpty()
            || ui->lineEditConsume5->text().isEmpty() || ui->lineEditTime->text().isEmpty())
    {//输入不完整则提示
        QMessageBox *messagebox = new QMessageBox;
        QString str;
        str += "输入信息不完整，不可修改设置     ";
        messagebox->setText(str);
        messagebox->exec();
    }
    else{
        price[standard] = ui->lineEditRoom1->text().toInt();
        price[doublebed] = ui->lineEditRoom2->text().toInt();
        price[largebed] = ui->lineEditRoom3->text().toInt();
        price[business] = ui->lineEditRoom4->text().toInt();
        price[vip] = ui->lineEditRoom5->text().toInt();
        itemPrice[0] = ui->lineEditConsume1->text().toInt();
        itemPrice[1] = ui->lineEditConsume2->text().toInt();
        itemPrice[2] = ui->lineEditConsume3->text().toInt();
        itemPrice[3] = ui->lineEditConsume4->text().toInt();
        itemPrice[4] = ui->lineEditConsume5->text().toInt();
        interval = ui->lineEditTime->text().toInt();//获取输入的各个信息值
        if(ui->checkBoxSearch->checkState() == Qt::Checked){//如果模糊搜索选项被点击则置searchmodel为1
            searchmodel = 1;
        }
        else{//如果模糊搜索选项表不被点击则置searchmodel为0
            searchmodel = 0;
        }
        QMessageBox *messagebox1 = new QMessageBox;
        QString str;
        str += "设置信息修改成功     ";
        messagebox1->setText(str);
        messagebox1->exec();//提示信息
        this->close();//关闭此窗口
        logPrint("系统设置修改成功");//将事件写入日志文件
    }
}


setting::~setting()
{
    delete ui;
}

void setting::on_cancelButton_clicked()
{
    setting::close();
}
