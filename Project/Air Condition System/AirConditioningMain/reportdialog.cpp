#include "reportdialog.h"
#include "ui_reportdialog.h"
#include "slavelist.h"
#include <QButtonGroup>
#include <QVector>
#include <QSqlQuery>
#include <QVariant>

extern SlaveList slaveList;
const QString DATETIMEFORMAT="yyyy-MM-dd HH:mm:ss";

reportDialog::reportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reportDialog)
{
    ui->setupUi(this);
    QButtonGroup * group = new QButtonGroup();
    group->addButton(ui->dayRadioButton);
    group->addButton(ui->weekRadioButton);
    group->addButton(ui->monthRadioButton);
    ui->dayRadioButton->setChecked(true);

    ui->dateTimeEdit->setMaximumDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    model = new QSqlTableModel;
    model->setTable("request");
    model->setHeaderData(1,Qt::Horizontal,"房间号");
    model->setHeaderData(2,Qt::Horizontal,"起始时间");
    model->setHeaderData(3,Qt::Horizontal,"终止时间");
    model->setHeaderData(4,Qt::Horizontal,"起始温度");
    model->setHeaderData(5,Qt::Horizontal,"终止温度");
    model->setHeaderData(6,Qt::Horizontal,"风量");
    model->setHeaderData(7,Qt::Horizontal,"费用");
    //model->select();
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(8, true);
    ui->tableView->setColumnWidth(1,50);
    ui->tableView->setColumnWidth(2,160);
    ui->tableView->setColumnWidth(3,160);
    ui->tableView->setColumnWidth(4,60);
    ui->tableView->setColumnWidth(5,60);
    ui->tableView->setColumnWidth(6,60);
    ui->tableView->setColumnWidth(7,60);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QHeaderView * header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(true);
    ui->tableView->show();

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

reportDialog::~reportDialog()
{
    delete ui;
}

void reportDialog::on_buttonOk_clicked()
{
    QDateTime dateTime = ui->dateTimeEdit->dateTime();
    QDateTime maxTime;
    QDateTime minTime;
    if(ui->dayRadioButton->isChecked()){
        minTime = QDateTime(dateTime.date(),QTime(0,0));
        maxTime = minTime.addDays(1);
    }
    else if(ui->weekRadioButton->isChecked()){
        minTime = QDateTime(dateTime.date(),QTime(0,0));
        maxTime = minTime.addDays(7);
    }
    else if(ui->monthRadioButton->isChecked()){
        minTime = QDateTime(QDate(dateTime.date().year(),dateTime.date().month(),1),QTime(0,0));
        maxTime = minTime.addMonths(1);
    }
    int roomNum = ui->roomNumComboBox->currentText().toInt();
    model->setFilter("roomNum="+QString::number(roomNum)+" and begintime<'"+maxTime.toString(DATETIMEFORMAT)+"' and begintime>'"+minTime.toString(DATETIMEFORMAT)+"'");
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->show();

    QSqlQuery query;
    query.exec("SELECT cost FROM request WHERE roomNum="+QString::number(roomNum)+" and begintime<'"+maxTime.toString(DATETIMEFORMAT)+"' and begintime>'"+minTime.toString(DATETIMEFORMAT)+"'");
    float cost = 0;
    while(query.next()){
        cost+=query.value(0).toFloat();
    }
    ui->costLabel->setText(QString::number(cost));

    query.exec("SELECT * FROM event WHERE num="+QString::number(roomNum)+" and time<'"+maxTime.toString(DATETIMEFORMAT)+"' and time>'"+minTime.toString(DATETIMEFORMAT)+"' and event='开机'");
    int powerOnNum = 0;
    while(query.next()){
        powerOnNum++;
    }

    query.exec("SELECT * FROM event WHERE num="+QString::number(roomNum)+" and time<'"+maxTime.toString(DATETIMEFORMAT)+"' and time>'"+minTime.toString(DATETIMEFORMAT)+"' and event='关机'");
    int powerOffNum = 0;
    while(query.next()){
        powerOffNum++;
    }
    ui->powerLabel->setText(QString::number(powerOnNum)+"/"+QString::number(powerOffNum));
}
