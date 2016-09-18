#ifndef REGDIALOG_H
#define REGDIALOG_H

/* 注册界面 */

#include <QDialog>

namespace Ui {
class regDialog;
}

class regDialog : public QDialog
{
    Q_OBJECT

public:
    explicit regDialog(QWidget *parent = 0);
    ~regDialog();

private slots:
    void on_buttonSubmit_clicked();
    void on_buttonClear_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::regDialog *ui;
};

#endif // REGDIALOG_H
