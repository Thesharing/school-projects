#ifndef REGACCOUNTDIALOG_H
#define REGACCOUNTDIALOG_H

#include <QDialog>

namespace Ui {
class regAccountDialog;
}

class regAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit regAccountDialog(QWidget *parent = 0);
    ~regAccountDialog();

private slots:
    void on_buttonOk_clicked();

    void on_buttonCancel_clicked();

private:
    Ui::regAccountDialog *ui;
};

#endif // REGACCOUNTDIALOG_H
