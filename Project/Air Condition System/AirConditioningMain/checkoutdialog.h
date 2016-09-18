#ifndef CHECKOUTDIALOG_H
#define CHECKOUTDIALOG_H

#include <QDialog>

namespace Ui {
class checkOutDialog;
}

class checkOutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit checkOutDialog(QWidget *parent = 0);
    ~checkOutDialog();

private slots:
    void on_buttonOk_clicked();

    void on_buttoCancel_clicked();

private:
    Ui::checkOutDialog *ui;
};

#endif // CHECKOUTDIALOG_H
