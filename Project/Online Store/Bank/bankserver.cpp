#include "bankserver.h"
#include "account.h"
#include <QRegExp>
#include <QSqlQuery>

extern account * accountPtr;

bankServer::bankServer(QObject *parent)
    :QTcpServer(parent)
{
    connect(this,SIGNAL(newConnection()),this,SLOT(createConnection()));
}


/* 当有新客户端请求建立连接时（即newConnection()被唤醒时，建立连接 */
void bankServer::createConnection(){
    this->clientConnection = this->nextPendingConnection();
    connect(clientConnection, SIGNAL(readyRead()),this,SLOT(readClient()));
}


/* 建立连接后，如果有信息传入（即readyRead()信号被唤醒）
 * 开始读取信息并进行处理
 * 处理完信息后将处理结果再发回客户端 */
void bankServer::readClient(){
    QString str=clientConnection->readAll();
    QString res;
    QSqlQuery query;

    //正则表达式，通过ID:****;PW:****读取到账号和密码，此时为绑定操作
    //而PY:**;ID:****;PW:****为支付操作，PY后为要支付的金额
    QRegExp patternBindingCard("ID:(.*);PW:(.*)");
    QRegExp patternPayment("PY:(.*);ID:(.*);PW:(.*)");

    if(str.indexOf(patternBindingCard)==0){
        //此时为绑定操作
        QStringList strList = patternBindingCard.capturedTexts();
        QString id=strList.at(1);
        QString password=strList.at(2);
        //从收到的信息中识别到id和password以后，去数据库查询一下
        //根据不同结果返回YES-密码正确，WRPW-密码错误，NOID-账号不存在
        query.exec("SELECT password FROM accountInfo WHERE num='"+id+"'");
        if(query.next()){
            if(password==query.value(0).toString()){
                res="YES";
            }
            else{
                res="WRPW";
            }
        }
        else{
            res="NOID";
        }
    }
    else if(str.indexOf(patternPayment)==0){
        //此时为支付操作
        QStringList strList=patternPayment.capturedTexts();
        QString totalStr = strList.at(1);
        float total = totalStr.toFloat();
        QString id=strList.at(2);
        QString password=strList.at(3);
        query.exec("SELECT * FROM accountInfo WHERE num='"+id+"'");
        if(query.next()){
            if(password==query.value(1).toString()){
                if(total<=query.value(4).toFloat()){
                    //账户余额足够，支付成功
                    res="YES";
                    float balance=query.value(4).toFloat()-total;
                    query.exec("UPDATE accountInfo SET balance="+QString::number(balance)+" WHERE name='"+id+"'");
                    accountPtr->setBalance(balance);
                }
                //账户余额不足，支付失败
                else{res="LKMN";}
            }
            //密码错误
            else{res="WRPW";}
        }
        //账号不存在
        else{res="NOID";}
    }
    else{
        //字符串传入错误，客户端使用了不可识别的协议
        res="ERROR";
    }

    //将结果返回客户端
    clientConnection->write(res.toUtf8());
}
