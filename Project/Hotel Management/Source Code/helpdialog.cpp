#include "helpdialog.h"
#include "ui_helpdialog.h"

helpdialog::helpdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpdialog)
{//点击主界面关于帮助时弹出此窗口
    ui->setupUi(this);
    setFixedSize(270,390);
    setWindowTitle("帮助");
    ui->label_17->setText("该旅店系统有预约，入住，离店，查询等功能");
    ui->label_18->setText("旅店分二楼至六楼共五层：");
    ui->label_7->setText("     二楼为标准间");
    ui->label_8->setText("     三楼为双人间");
    ui->label_9->setText("     四楼为大床房");
    ui->label_10->setText("     五楼为商务房");
    ui->label_11->setText("     六楼为VIP贵宾间");
    ui->label_19->setText("客房信息中房间颜色代表信息分别为：");
    ui->label_12->setText("空闲");
    ui->label_13->setText("被预约");
    ui->label_14->setText("已入住");
    ui->label_15->setText("已入住且被预约");
    ui->label_16->setText("入住超时");


}

helpdialog::~helpdialog()
{
    delete ui;
}
