#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

/* “登录”界面 */

#include <QDialog>

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = 0);
    ~loginDialog();

private slots:
    void on_buttonLogin_clicked();
    void on_buttonReg_clicked();
    void on_buttonClear_clicked();
    void on_buttonSetting_clicked();

private:
    Ui::loginDialog *ui;
};

#endif // LOGINDIALOG_H
