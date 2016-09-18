#ifndef CHECKINDOUBLEDIALOG_H
#define CHECKINDOUBLEDIALOG_H

#include "ui_checkindoubledialog.h"

class checkindoubleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit checkindoubleDialog(int roomnum, int days, QWidget *parent = 0);



private:
    Ui::checkindoubleDialog *ui;

private slots:
    void setman();//确保性别选项只能选择一个
    void setwoman();//确保性别选项只能选择一个
    void checkinto1();//登记函数
    void checkinsucess();//提示信息


};

#endif // CHECKINDOUBLEDIALOG_H
