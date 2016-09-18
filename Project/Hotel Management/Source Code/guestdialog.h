#ifndef GUESTDIALOG_H
#define GUESTDIALOG_H

#include <QDialog>

namespace Ui {
class guestDialog;
}

class guestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit guestDialog(QWidget *parent = 0);
    ~guestDialog();

private slots:
    void on_consumeButton_clicked();
    void on_reserveButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::guestDialog *ui;
};

#endif // GUESTDIALOG_H
