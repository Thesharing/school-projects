#include "guestdialog.h"
#include "ui_guestdialog.h"
#include "dialogconsume.h"
#include "ui_dialogconsume.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reservedialog.h"
#include "ui_reservedialog.h"
#include "cancelreservedialog.h"
#include "ui_cancelreservedialog.h"

guestDialog::guestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::guestDialog)
{
    ui->setupUi(this);
}

guestDialog::~guestDialog()
{
    delete ui;
}

//顾客界面，当顾客点击相应按钮时跳转到相应的模块

//“客房消费”，详见:dialogconsum.cpp
void guestDialog::on_consumeButton_clicked()
{
    dialogConsume * dialogConsume1=new dialogConsume(this);
    dialogConsume1->exec();

}

//“我要预约”，详见reservedialog.cpp
void guestDialog::on_reserveButton_clicked()
{
    reserveDialog * reserveDialog1=new reserveDialog(this);
    reserveDialog1->exec();
}

//“取消预约”，详见cancelreservedialog.cpp
void guestDialog::on_cancelButton_clicked()
{
    cancelreservedialog * cancelreservedialog1=new cancelreservedialog(this);
    cancelreservedialog1->exec();
}
