#include "backupdialog.h"
#include "ui_backupdialog.h"
#include <QTimer>
#include <QMessageBox>

backupDialog::backupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::backupDialog)
{
    ui->setupUi(this);
}

backupDialog::~backupDialog()
{
    delete ui;
}

//备份的功能函数在mainwindow.cpp中（void MainWindow::backupFunction()）
//这里只负责窗口绘制
void backupDialog::backupStatus(void){ //绘制备份时的进度窗口
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    ui->okButton->setEnabled(false);
    QTimer * timerBackup=new QTimer(this);
    connect(timerBackup,SIGNAL(timeout()),this,SLOT(backupStatusUpdate()));
    timerBackup->setSingleShot(false);
    timerBackup->start(20);
}
void backupDialog::backupStatusUpdate(void){ //控制备份进度条的前进
    ui->progressBar->setValue(ui->progressBar->value()+1);
    if(ui->progressBar->value()==100){
        ui->label->setText("备份完成。");
        ui->okButton->setEnabled(true);
    }
}

void backupDialog::on_okButton_clicked()
{
    backupDialog::close();//当ok被按下时关闭窗口
}
