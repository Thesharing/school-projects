#include "cancelreservedialog.h"
#include "ui_cancelreservedialog.h"
#include <QMessageBox>
#include <QSqlQuery>

extern void logPrint(QString str);

cancelreservedialog::cancelreservedialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cancelreservedialog)
{
    ui->setupUi(this);
    QRegExp regExp1("[0-9]{13}"); //这里为手机号的输入进行限制，只允许输入13位数字
    ui->lineEditPhoneNum->setValidator(new QRegExpValidator(regExp1,this));
}

cancelreservedialog::~cancelreservedialog()
{
    delete ui;
}

void cancelreservedialog::on_cancelButton_clicked()
{
    cancelreservedialog::close(); //当按下“取消”按钮时，关闭对话框
}

//取消用户的预约，由“顾客界面”访问到
//将已有的用户预约删除
void cancelreservedialog::on_okButton_clicked() //当按下“确定”按钮时，对输入的信息进行处理
{
    QString name=ui->lineEditName->text(); //name是输入的姓名
    QString phoneNum=ui->lineEditPhoneNum->text(); //phoneNum是输入的电话号码
    if(name.isEmpty() || phoneNum.isEmpty()){
        //当输入信息不完整时提示错误
        QMessageBox::warning(NULL, "错误","您输入的信息不完整，请检查。", QMessageBox::Yes | QMessageBox::Yes);
    }
    else{
        //先查找是否有对应的预约，如果这个客人有预约，则删除掉这个预约，然后对这个房间的状态进行修改：如果这个房间已经没有预约了，从“已预约”状态
        //或“已预约且有入住”状态转为”空闲“或”已入住“状态
        //如果这个房间还有预约，则状态不变
        //如果这个客人没有预约，则提示没有预约
        QSqlQuery query;
        QSqlQuery query2;
        QSqlQuery query3;
        query.exec("SELECT * FROM bookInfo WHERE name = '"+name+"' AND phoneNum = '"+phoneNum+"'");
        if(query.next()){
            QMessageBox::information(NULL, "取消预约","已经成功取消您的预约。", QMessageBox::Yes | QMessageBox::Yes);
        }
        else{
            QMessageBox::warning(NULL, "错误","未找到您的预约。", QMessageBox::Yes | QMessageBox::Yes);
            return;
        }
        query.exec("SELECT room FROM bookInfo WHERE name = '"+name+"' AND phoneNum= '"+phoneNum+"'");
        while(query.next()){
            query2.exec("SELECT * FROM bookInfo WHERE room = '"+query.value(0).toString()+"' AND name != '"+name+"'");
            if(!query2.next()){
                query2.exec("SELECT status FROM roomInfo WHERE num ="+query.value(0).toString());
                while(query2.next()){
                    if(query2.value(0).toString()=="booked")
                    {
                        query3.exec("UPDATE roomInfo SET status = 'empty' WHERE num ="+query.value(0).toString());
                    }
                    else if(query2.value(0).toString()=="checkandbook")
                        query3.exec("UPDATE roomInfo SET status = 'checked' WHERE num ="+query.value(0).toString());
                }
            }
        }
        query.exec("DELETE FROM bookInfo WHERE name = '"+name+"' AND phoneNum = '"+phoneNum+"'");
        logPrint("顾客 "+name+" 取消了他的预约。");//这里将事件写入日志
        cancelreservedialog::close();//关闭窗口
    }
}
