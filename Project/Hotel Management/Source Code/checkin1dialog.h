#ifndef CHECKIN1DIALOG_H
#define CHECKIN1DIALOG_H

#include "ui_checkin1dialog.h"
#include <QMessageBox>



namespace Ui {
class checkin1Dialog;
}

class checkin1Dialog : public QDialog
{
    Q_OBJECT

public:
    checkin1Dialog(QWidget *parent = 0);
//    ~checkinDialog();

private slots:

    void setman();//确保性别选项只能选择一个
    void setwoman();//确保性别选项只能选择一个
    void checkinto();//登记函数
    void checkinsucess(int consume);//提示信息


private:
    Ui::checkin1Dialog *ui;




public slots:

public:



};

#endif // CHECKIN1DIALOG_H
