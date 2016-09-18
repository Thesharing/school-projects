#include "debugdialog.h"
#include "ui_debugdialog.h"
#include <QVector>
#include "requestlist.h"
#include <QTimer>

extern RequestList requestList;

debugDialog::debugDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::debugDialog)
{
    ui->setupUi(this);
    QTimer * timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setSingleShot(false);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateFunction()));
    timer->start();
    labels[0] = ui->label;
    labels[1] = ui->label_2;
    labels[2] = ui->label_3;
    labels[3] = ui->label_4;
    labels[4] = ui->label_5;
    labels[5] = ui->label_6;
}

debugDialog::~debugDialog()
{
    delete ui;
}

void debugDialog::updateFunction(){
    QVector<Request>* list = requestList.requestList;
    int i = 0;
    Request * it = list->begin();
    while(it != list->end()){
       labels[i]->setText(it->outputForDialog());
       i++;
       it++;
    }
}
