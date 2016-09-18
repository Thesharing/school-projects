#include "restoredialog.h"
#include "ui_restoredialog.h"
#include <QTimer>

restoreDialog::restoreDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::restoreDialog)
{
    ui->setupUi(this);
}

restoreDialog::~restoreDialog()
{
    delete ui;
}

//还原的功能函数在mainwindow.cpp中（void MainWindow::restoreFunction()）
//这里只负责窗口绘制

void restoreDialog::restoreStatus(void){//绘制还原时的进度窗口
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    ui->okButton->setEnabled(false);
    QTimer * timerRestore=new QTimer(this);
    connect(timerRestore,SIGNAL(timeout()),this,SLOT(restoreStatusUpdate()));
    timerRestore->setSingleShot(false);
    timerRestore->start(20);
}

void restoreDialog::restoreStatusUpdate(void){//控制还原进度条的前进
    ui->progressBar->setValue(ui->progressBar->value()+1);
    if(ui->progressBar->value()==40){
        ui->label->setText("还原备份……");
    }
    if(ui->progressBar->value()==80){
        ui->label->setText("即将完成……");
    }
    if(ui->progressBar->value()==100){
        ui->label->setText("已完成。");
        ui->okButton->setEnabled(true);
    }
}

void restoreDialog::on_okButton_clicked()
{
    restoreDialog::close();//当ok被按下时关闭窗口
}
