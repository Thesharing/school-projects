#include "configdialog.h"
#include "ui_configdialog.h"

/* “管理”界面，可以查看目前已经有注册了那些账户 */

configDialog::configDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configDialog)
{
    ui->setupUi(this);

    //这里将accountInfo数据库与表格绑定数据
    model = new QSqlTableModel;
    model->setTable("accountInfo");
    model->setHeaderData(0,Qt::Horizontal,"卡号");
    model->setHeaderData(1,Qt::Horizontal,"密码");
    model->setHeaderData(2,Qt::Horizontal,"银行");
    model->setHeaderData(3,Qt::Horizontal,"身份证号");
    model->setHeaderData(4,Qt::Horizontal,"余额");
    model->setHeaderData(5,Qt::Horizontal,"联系方式");
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->show();
}

configDialog::~configDialog()
{
    delete ui;
}
