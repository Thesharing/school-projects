#ifndef EDITDIALOG_H
#define EDITDIALOG_H

/* “修改密码”界面 */

#include <QDialog>

namespace Ui {
class editDialog;
}

class editDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editDialog(QWidget *parent = 0);
    ~editDialog();

private slots:
    void on_buttonOk_clicked();

    void on_buttonCancel_clicked();

private:
    Ui::editDialog *ui;
};

#endif // EDITDIALOG_H
