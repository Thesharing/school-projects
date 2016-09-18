#include "dialogconsume.h"
#include "ui_dialogconsume.h"
#include "datastruct.h"
#include <QSqlQuery>
#include <QMessageBox>

extern int itemPrice[5];
extern int convStrToIntForStatus(QString str);

dialogConsume::dialogConsume(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogConsume)
{
    ui->setupUi(this);

    QRegExp regExp1("[2-6][0-1][0-9]");//对房间号的输入进行限制，只允许输入2楼至6楼的房间号
    ui->lineEdit->setValidator(new QRegExpValidator(regExp1,this));

    ui->labelWater->setText(QString::number(itemPrice[0])+"元");//在窗口上显示价格信息
    ui->labelCookie->setText(QString::number(itemPrice[1])+"元");
    ui->labelNoodle->setText(QString::number(itemPrice[2])+"元");
    ui->labelFruit->setText(QString::number(itemPrice[3])+"元");
    ui->labelDinner->setText(QString::number(itemPrice[4])+"元");
}

dialogConsume::~dialogConsume()
{
    delete ui;
}

void dialogConsume::on_cancelButton_clicked()
{
    dialogConsume::close();//关闭顾客消费的窗口
}

void dialogConsume::on_okButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()){//如果输入为空，提示错误
       QMessageBox::warning(NULL, "错误","请输入房间号。", QMessageBox::Yes | QMessageBox::Yes);
       return;
    }
    else if(((ui->lineEdit->text().length())<3)|| //如果房间号输入错误，提示错误
            ((ui->lineEdit->text()).at(1)=='1'&& (ui->lineEdit->text()).at(2)>='9') ||
            ((ui->lineEdit->text()).at(1)=='0'&& (ui->lineEdit->text()).at(2)=='0')){
       QMessageBox::warning(NULL, "错误","房间号输入有误，请检查。", QMessageBox::Yes | QMessageBox::Yes);
       return;
    }

    //查询该房间是否有客人，如果没有客人入住则提示错误
    QSqlQuery query;
    query.exec("SELECT status, cost FROM roomInfo WHERE num = '"+ui->lineEdit->text()+"'");
    query.next();
    int cost=query.value(1).toInt();
    int consume=0;
    if(convStrToIntForStatus(query.value(0).toString())==empty || convStrToIntForStatus(query.value(0).toString())==booked){
        QMessageBox::warning(NULL, "错误","该房间未住客人，请检查后重新输入。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    else if(ui->checkBox1->checkState() == Qt::Unchecked && ui->checkBox2->checkState() == Qt::Unchecked &&
       ui->checkBox3->checkState() == Qt::Unchecked && ui->checkBox4->checkState() == Qt::Unchecked &&
       ui->checkBox5->checkState() == Qt::Unchecked){
        //如果未选择任何一项物品，则提示错误
       QMessageBox::warning(NULL, "错误","请至少选择一项。", QMessageBox::Yes | QMessageBox::Yes);
    }
    else{
        query.exec("SELECT consume FROM guestInfo WHERE room = '"+ui->lineEdit->text()+"'");
        int prevConsume=0;
        if(query.next()){
            prevConsume=+query.value(0).toInt();
        }
        //将选中的项目的价格加到对应房间的消费金额里去
        if(ui->checkBox1->checkState() == Qt::Checked){
            consume+=itemPrice[0];
        }
        if(ui->checkBox2->checkState() == Qt::Checked){
            consume+=itemPrice[1];
        }
        if(ui->checkBox3->checkState() == Qt::Checked){
            consume+=itemPrice[2];
        }
        if(ui->checkBox4->checkState() == Qt::Checked){
            consume+=itemPrice[3];
        }
        if(ui->checkBox5->checkState() == Qt::Checked){
            consume+=itemPrice[4];
        }
        QMessageBox::information(NULL, "提示","将消费"+QString::number(consume)+"元，请确认。", QMessageBox::Yes | QMessageBox::Yes);

        if(cost<consume){
             QMessageBox::warning(NULL, "提示","您的房间押金余额不足，请到前台缴费后重试。", QMessageBox::Yes | QMessageBox::Yes);
             return;
        }
        //更新数据库里的信息
        query.exec("UPDATE roomInfo SET cost="+QString::number(cost-consume)+" WHERE num = '"+ui->lineEdit->text()+"'");
        query.exec("UPDATE guestInfo SET consume="+QString::number(prevConsume+consume)+" WHERE room = '"+ui->lineEdit->text()+"'");
        dialogConsume::close(); //关闭窗口
    }
}
