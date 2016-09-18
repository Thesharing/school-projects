#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = 0);
    ~loginDialog();

    void closeEvent(QCloseEvent *);

private slots:
    void on_buttonReg_clicked();

    void on_buttonOk_clicked();

    void on_buttonCancel_clicked();

private:
    Ui::loginDialog *ui;
};

#endif // LOGINDIALOG_H
