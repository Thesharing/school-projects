#ifndef WITHDRAWDIALOG_H
#define WITHDRAWDIALOG_H

/* 取款界面 */

#include <QDialog>

namespace Ui {
class withdrawDialog;
}

class withdrawDialog : public QDialog
{
    Q_OBJECT

public:
    explicit withdrawDialog(QWidget *parent = 0);
    ~withdrawDialog();

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::withdrawDialog *ui;
};

#endif // WITHDRAWDIALOG_H
