#ifndef CKECKIN_H
#define CKECKIN_H

#include <QDialog>
#include <QMessageBox>
#include "ui_checkinDialog.h"
#include "datastruct.h"


namespace Ui {
class checkinDialog;
}

class checkinDialog : public QDialog
{
    Q_OBJECT

public:
    checkinDialog(QWidget *parent = 0);
//    ~checkinDialog();

private slots:

    void setman();//确保性别选项只能选择一个
    void setwoman();//确保性别选项只能选择一个
    void setno();//确保是否预约选项只能选择一个
    void setyes();//确保是否预约选项只能选择一个
    void checkinto();//登记与预约转登记
    void deletereverse(int consume);//提示信息
    void undeletereverse(int consume);//提示信息


private:
    Ui::checkinDialog *ui;




public slots:

public:
    void checkin(struct roomNode*,struct guestNode*,struct guestNode*,struct roomNode*);


};



#endif // CKECKIN_H
