#include "checkoutdialog.h"
#include "ui_checkoutdialog.h"
#include "slavelist.h"
#include "requestlist.h"
#include "writedown.h"

extern SlaveList slaveList;
extern RequestList requestList;

checkOutDialog::checkOutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkOutDialog)
{
    ui->setupUi(this);
    QVector<Slave> * list = slaveList.getSalves();
    if(list->size()>0){
        Slave * it = list->begin();
        while(it!=list->end()){
            ui->roomNumComboBox->addItem(QString::number(it->no));
            it++;
        }
    }
    else{
        ui->roomNumComboBox->setEnabled(false);
        ui->buttonOk->setEnabled(false);
    }
}

checkOutDialog::~checkOutDialog()
{
    delete ui;
}

void checkOutDialog::on_buttonOk_clicked()
{
    int roomNum = ui->roomNumComboBox->currentText().toInt();
    Slave * slave = slaveList.findSlave(roomNum);
    ui->labelCost->setText(QString::number(slave->cost));
    slave->cost = 0;
    slave->usage = 0;
    if(slave->wind != Wind::stop){
        slave->wind = Wind::stop;
        writeDownEnd(slave->no,slave->currentTemperature);
    }
    requestList.setRequestDealt(roomNum);
}

void checkOutDialog::on_buttoCancel_clicked()
{
    this->close();
}
