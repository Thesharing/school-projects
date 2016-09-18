#ifndef ROOMINFODIALOG_H
#define ROOMINFODIALOG_H

#include <QDialog>

namespace Ui {
class roominfodialog;
}

class roominfodialog : public QDialog
{
    Q_OBJECT

public:
    explicit roominfodialog(QWidget *parent = 0);
    ~roominfodialog();
    void updateRoomInfoDialog(int num);

private slots:
    void on_okButton_clicked();//当按下ok时关闭对话框
    void checkin1();//当按下“入住”时进行登记
    void checkout1(); //当按下“退房”时进行退房

private:
    Ui::roominfodialog *ui;
};

#endif // ROOMINFODIALOG_H
