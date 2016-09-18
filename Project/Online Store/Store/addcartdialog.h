#ifndef ADDCARTDIALOG_H
#define ADDCARTDIALOG_H

#include <QDialog>

namespace Ui {
class addCartDialog;
}

class addCartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addCartDialog(QWidget *parent = 0, int index = 1, int type = 0);
    ~addCartDialog();
    int num;
    int type;

private slots:
    void on_buttonOk_clicked();

    void on_buttonCancel_clicked();

private:
    Ui::addCartDialog *ui;
};

#endif // ADDCARTDIALOG_H
