#ifndef CHECKOUT_H
#define CHECKOUT_H

#include <QDialog>
#include <QMessageBox>
#include "ui_checkoutDialog.h"
#include "datastruct.h"

namespace Ui {
class checkoutDialog;
}
class checkoutDialog : public QDialog
{
    Q_OBJECT

public:
    checkoutDialog(QWidget *parent = 0);
//    ~checkoutDialog();

private slots:
    void enablepushButtoncheckout(const QString&);//控制离店按钮可编辑
    void checkoutto();//离店函数
    void checkoutsuccess();//提示信息

private:
    Ui::checkoutDialog *ui;

public slots:

public:


};
#endif // CHECKOUT_H















