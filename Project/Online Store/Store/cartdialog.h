#ifndef CARTDIALOG_H
#define CARTDIALOG_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class cartDialog;
}

class cartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cartDialog(QWidget *parent = 0);
    QSqlTableModel *model;
    float total=0;
    float sale=0;
    void updateCart();
    ~cartDialog();

private slots:
    void on_buttonCancel_clicked();
    void on_buttonClear_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_buttonComfirm_clicked();

private:
    Ui::cartDialog *ui;
};

#endif // CARTDIALOG_H
