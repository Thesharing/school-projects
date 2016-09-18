#ifndef DEPOSITDIALOG_H
#define DEPOSITDIALOG_H

/* “取款”界面 */

#include <QDialog>

namespace Ui {
class depositDialog;
}

class depositDialog : public QDialog
{
    Q_OBJECT

public:
    explicit depositDialog(QWidget *parent = 0);
    ~depositDialog();

private slots:
    void on_okButton_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::depositDialog *ui;
};

#endif // DEPOSITDIALOG_H
