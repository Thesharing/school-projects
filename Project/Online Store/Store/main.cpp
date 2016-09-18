#include "mainwindow.h"

#include <QApplication>

#include "datastruct.h"

bool isLogged = false;
bool isEvent = false;
int eventReq = 0;
int eventFav = 0;
QString account;
struct Store store;
int cartnum;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if(isLogged==true){
        w.show();
        return a.exec();
    }
    return 0;
}
