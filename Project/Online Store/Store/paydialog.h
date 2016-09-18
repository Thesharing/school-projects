#ifndef PAYDIALOG_H
#define PAYDIALOG_H

#include <QDialog>

namespace Ui {
class payDialog;
}

class payDialog : public QDialog
{
    Q_OBJECT

public:
    explicit payDialog(QWidget *parent = 0, int num=0, float total=0);
    int num;
    float total;
    ~payDialog();

private slots:
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::payDialog *ui;
};

#endif // PAYDIALOG_H
