#include "mainwindow.h"
#include "account.h"
#include "bankserver.h"

#include <QHostAddress>
#include <QApplication>
#include <QString>
#include <QTcpSocket>
#include <QTcpServer>

bool isLogged=false;
account * accountPtr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if(isLogged==true){
        w.show();

        //建立QTcpServer
        bankServer server;
        if(!server.listen(QHostAddress::Any,6665)){
            qDebug()<<"Wrong in server in main.";
        }
        return a.exec();
    }
    return 0;
}
