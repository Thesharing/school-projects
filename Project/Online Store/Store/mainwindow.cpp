#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "cartdialog.h"
#include "addcartdialog.h"
#include "settingdialog.h"
#include "type.h"
#include "defineres.h"

#include <QtNetwork>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QString>
#include <QMessageBox>

/* account是账户名
 * cartnum是购物车数量
 * isEvent是是否有满减活动
 * eventReq和eventFav是满减金额 */
extern int cartnum;
extern bool isEvent;
extern int eventReq;
extern int eventFav;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //先建立数据库
    QFileInfo dbFileInfo("data.db");
    bool isExist=dbFileInfo.exists();
    db=QSqlDatabase:: addDatabase("QSQLITE");
    db.setDatabaseName("data.db");
    createConnection(isExist);

    //然后从store.ini里读取打折信息
    QFile text("store.ini");
    text.close();
    if(!text.open(QIODevice::Text|QIODevice::ReadWrite)){
        QMessageBox::warning(NULL, "提示","无法打开配置文件，请联系管理员。", QMessageBox::Yes | QMessageBox::Yes);
        return;
    }
    QTextStream in(&text);
    int temp;
    in>>temp;
    isEvent=(bool)temp;
    in>>eventReq;
    in>>eventFav;

    //然后登录
    loginDialog * logindialog = new loginDialog(this);
    logindialog->exec();

    ui->setupUi(this);

    //然后将商品信息从数据库读入内存中
    initialize();
    //将商品信息显示在表格中
    model = new QSqlTableModel;
    model->setTable("productInfo");
    model->setHeaderData(1,Qt::Horizontal,"商品名称");
    model->setHeaderData(2,Qt::Horizontal,"类别");
    model->setHeaderData(3,Qt::Horizontal,"描述");
    model->setHeaderData(4,Qt::Horizontal,"价格");
    model->setHeaderData(5,Qt::Horizontal,"余量");
    model->setHeaderData(6,Qt::Horizontal,"优惠");
    model->setHeaderData(7,Qt::Horizontal,"其他");
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setColumnWidth(1,150);
    ui->tableView->setColumnWidth(2,60);
    ui->tableView->setColumnWidth(3,150);
    ui->tableView->setColumnWidth(4,50);
    ui->tableView->setColumnWidth(5,50);
    ui->tableView->setColumnWidth(6,40);
    ui->tableView->setColumnHidden(0, true);
    QHeaderView *header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(true);
    ui->tableView->show();
    ui->buttonCart->setText("购物车("+QString::number(cartnum)+")");

    connect(ui->comboBoxSearch,SIGNAL(currentIndexChanged(int)),this,SLOT(displayType()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//当点击商品列表中某项时，弹出窗口“添加购物车”
void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    int num=index.row()+1;
    int type=ui->comboBoxSearch->currentIndex();
    QSqlQuery query;
    switch(type){
        case 0:
            query.exec("SELECT * FROM productInfo");
            break;
        case BOOK:
            query.exec("SELECT * FROM productInfo WHERE type = 'Book'");
            break;
        case FOOD:
            query.exec("SELECT * FROM productInfo WHERE type = 'Food'");
            break;
        case CLOTH:
            query.exec("SELECT * FROM productInfo WHERE type = 'Cloth'");
            break;
        case ELECTRO:
            query.exec("SELECT * FROM productInfo WHERE type = 'Electro'");
            break;
        default:
            query.exec("SELECT * FROM productInfo");
            break;
    }
    for(int i=0;i<num;i++)
        query.next();
    if(query.value(5).toInt()>0){
        addCartDialog * addcartdialog = new addCartDialog(this,num,type);
        addcartdialog->exec();
        MainWindow::updateUI();
    }
    else{
       QMessageBox::warning(NULL, "抱歉","已缺货，请购买其他商品。", QMessageBox::Yes | QMessageBox::Yes);
    }
}

void MainWindow::on_buttonCart_clicked()
{
    cartDialog * cartdialog = new cartDialog(this);
    cartdialog->exec();
    MainWindow::updateUI();
}

//从购物车回到主界面时，更新购物车数量
void MainWindow::updateUI(){
    model->select();
    ui->buttonCart->setText("购物车("+QString::number(cartnum)+")");
}

void MainWindow::on_buttonExit_clicked()
{
    close();
}

//主界面下拉菜单选择不同，显示不同种类的商品
void MainWindow::displayType(){
    int type=ui->comboBoxSearch->currentIndex();
    switch (type){
        case 0:
            model->setFilter("");
            model->select();
            break;
        case FOOD:
            model->setFilter("type = 'Food'");
            model->select();
            break;
        case BOOK:
            model->setFilter("type = 'Book'");
            model->select();
            break;
        case CLOTH:
            model->setFilter("type = 'Cloth'");
            model->select();
            break;
        case ELECTRO:
            model->setFilter("type = 'Electro'");
            model->select();
            break;
        default:
            model->setFilter("");
            model->select();
            break;
    }
}

void MainWindow::on_buttonUserConfig_clicked()
{
    settingDialog * settingdialog = new settingDialog (this);
    settingdialog->exec();
}

//建立Tcp连接与银行服务器端交互
int MainWindow::askBank(int function, QString str){
    QTcpSocket * socket = new QTcpSocket();
    socket->connectToHost(QHostAddress::LocalHost,6665);
    if(socket->waitForConnected(1000)){
        socket->write(str.toUtf8());
        if(socket->waitForReadyRead(1000)){
             QString res=socket->readAll();
             if(res=="YES"){
                return SUCCESS;
             }
             else if(res=="NOID"){
                return NOID;
             }
             else if(res=="WRPW"){
                return WRONGPW;
             }
             else if(res=="LKMN"){
                return LACKMONEY;
             }
             else if(res=="ERROR"){
                 QMessageBox::warning(NULL, "抱歉","出现错误。", QMessageBox::Yes | QMessageBox::Yes);
                 return WRONGPW;
             }
        }
        else{
            return LOSTCONNECTION;
        }
    }
    else{
         return NOSERVER;
    }
}
