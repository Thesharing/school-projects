#include "reservedialog.h"
#include "ui_reservedialog.h"
#include "datastruct.h"
#include <QDateTime>
#include <QDateEdit>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

extern int now;
extern QString nowFormat;

QString intToStringForType(int type);
extern void logPrint(QString str);

//用户预约，通过“顾客界面”可以打开
//将用户输入的预约信息记录到数据库中
reserveDialog::reserveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reserveDialog)
{
    ui->setupUi(this);

    QRegExp regExp("[0-9]{11}");//对手机号输入进行格式限制，只允许输入13位数字
    ui->lineEditPhoneNum->setValidator(new QRegExpValidator(regExp,this));
    //对预定天数和房间数进行限制
    ui->spinBoxDayNum->setMinimum(1);//最少预定1天
    ui->spinBoxDayNum->setMaximum(14); //最多允许预定14天
    ui->spinBoxRoomNum->setMinimum(1);//最少预定1间
    ui->spinBoxRoomNum->setMaximum(3);//最多预定3间
    ui->dateEditCheckIn->setSelectedSection(QDateTimeEdit::DaySection);
    //下面是将输入的日期进行格式转换
    QDateTime temp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
    temp=temp.addSecs(qint64(now));

    QDateTime min=temp.addDays(1);//最少是预定明天的房间
    QDateTime max=temp.addMonths(12);//最多允许预定12个月以内的房间
    ui->dateEditCheckIn->setMinimumDate(min.date());
    ui->dateEditCheckIn->setMaximumDate(max.date());
}

reserveDialog::~reserveDialog()
{
    delete ui;
}

void reserveDialog::on_cancelButton_clicked()
{
    reserveDialog::close();
}



void reserveDialog::on_okButton_clicked(){
   //这里提交预定信息
    //如果信息未填写完整，则返回错误信息
   if(ui->lineEditName->text().isEmpty() || ui->lineEditPhoneNum->text().isEmpty()){
       QMessageBox::warning(NULL, "错误","信息未填写完整，请检查。", QMessageBox::Yes | QMessageBox::Yes);
       return;
   }
   else{
       //将客人入住的时间转换格式
       QDateTime checkIn(ui->dateEditCheckIn->date(),QTime(12,0,1));
       QDateTime temp=QDateTime::fromString("2015/01/01 00:00:00",nowFormat);
       int checkInTime=(int)temp.secsTo(checkIn);
       int checkOutTime=checkInTime+(ui->spinBoxDayNum->value())*60*60*24-1;

       int avlbRoom[5*18];//可用房间的列表
       int avlbRoomNum=0;//可用房间数
       int roomInNeed=ui->spinBoxRoomNum->value();
       QString type=intToStringForType(ui->comboBoxType->currentIndex());

       QSqlQuery query1;
       QSqlQuery query2;

       //先优先安排空房，查询到可用的空房将直接加入可用房间列表
       query1.exec("SELECT num FROM roomInfo WHERE status = 'empty' AND type = '"+type+"'");
       while(query1.next()){
           avlbRoom[avlbRoomNum]=query1.value(0).toInt();
           avlbRoomNum++;
       }

       //其次安排预定过的房间，查询到预定过的房间以后，确定该房间此时间段没有被预定，加入可用房间列表
       query1.exec("SELECT num FROM roomInfo WHERE status = 'booked' AND type ='"+type+"'");
       while(query1.next()){
           query2.exec("SELECT checkInTime, checkOutTime FROM bookInfo WHERE room = "+QString::number(query1.value(0).toInt()));
           int isAvlb=1;
           while(query2.next()){
               if(!((query2.value(1).toInt()<checkOutTime && query2.value(1).toInt()<checkInTime) || (query2.value(0).toInt()>checkInTime && query2.value(0).toInt()>checkOutTime))){
                   isAvlb=0;
                   break;
               }
           }
           if(isAvlb==1){
               avlbRoom[avlbRoomNum]=query1.value(0).toInt();
               avlbRoomNum++;
           }
       }

       //再者安排登记过的房间，只要预约日期在离店日期之后就加入可用房间列表
       query1.exec("SELECT num, checkInTime, checkOutTime FROM roomInfo WHERE status = 'checked' AND type ='"+type+"'");
       while(query1.next()){
           if(query1.value(2).toInt()<checkInTime){
               avlbRoom[avlbRoomNum]=query1.value(0).toInt();
               avlbRoomNum++;
           }
       }

       //最后安排登记并预约过的房间，要求预约日期在离店日期之后且不与其他预约冲突
       query1.exec("SELECT num, checkInTime, checkOutTime FROM roomInfo WHERE status = 'checkandbook' AND type ='"+type+"'");
       while(query1.next()){
           query2.exec("SELECT checkInTime, checkOutTime FROM bookInfo WHERE room = "+QString::number(query1.value(0).toInt()));
           if(query1.value(2).toInt()<checkInTime){
               int isAvlb=1;
               while(query2.next()){
                   if(!((query2.value(1).toInt()<checkOutTime && query2.value(1).toInt()<checkInTime) || (query2.value(0).toInt()>checkInTime && query2.value(0).toInt()>checkOutTime))){
                       isAvlb=0;
                       break;
                   }
               }
               if(isAvlb==1){
                    avlbRoom[avlbRoomNum]=query1.value(0).toInt();
                    avlbRoomNum++;
               }
           };
       }

       //如果找到的房间数不足，则返回提示信息
       //如果找到的房间数够，则按照优先顺序将旅客的预约信息登记，并更新房间状态
       if(avlbRoomNum<roomInNeed){
          QMessageBox::warning(NULL, "提示","仅找到"+QString::number(avlbRoomNum)+"间房间，未能找到足够的符合要求的房间，请修改时间或房间类型。", QMessageBox::Yes | QMessageBox::Yes);
       }
       else{
          QMessageBox::information(NULL, "提示","找到"+QString::number(avlbRoomNum)+"间房间，已经为您预约"+QString::number(roomInNeed)+"间房间。", QMessageBox::Yes | QMessageBox::Yes);
          QString name=ui->lineEditName->text();
          QString phoneNum=ui->lineEditPhoneNum->text();
          for(int i=0;i<roomInNeed;i++){
              query1.exec("SELECT status FROM roomInfo WHERE num = "+QString::number(avlbRoom[i]));
              while(query1.next()){
                  if(query1.value(0)=="empty"){
                      query1.exec("UPDATE roomInfo SET status = 'booked' WHERE num = "+QString::number(avlbRoom[i]));
                  }
                  else if(query1.value(0)=="checked"){
                      query1.exec("UPDATE roomInfo SET status = 'checkandbook' WHERE num = "+QString::number(avlbRoom[i]));
                  }
              }
              query1.exec("INSERT INTO bookInfo (name, phoneNum, checkInTime, checkOutTime, checkInTimeText, checkOutTimeText, room)"
                   "VALUES('"+name+"', '"+phoneNum+"', "+QString::number(checkInTime)+", "+QString::number(checkOutTime)+", '"+temp.addSecs(checkInTime).toString(nowFormat)+"', '"+temp.addSecs(checkOutTime).toString(nowFormat)+"', "+QString::number(avlbRoom[i])+")");
          }
          reserveDialog::close();
          QString roomStr="";
          for(int i=0;i<roomInNeed;i++){
              roomStr=roomStr+" "+QString::number(avlbRoom[i])+"房间";
          }
          //预约成功，将事件记入日志
          logPrint("顾客 "+ui->lineEditName->text()+" 预约了 "+temp.addSecs(checkInTime).toString(nowFormat)+" 的"+roomStr);
       }
   }
}

QString intToStringForType(int type){//将整数型的房间类型转换为字符串返回
    switch(type){
        case 0:
            return "standard";
            break;
        case 1:
            return "doublebed";
            break;
        case 2:
            return "largebed";
            break;
        case 3:
            return "business";
            break;
        case 4:
            return "vip";
            break;
        default:
            return " ";
    }
}
