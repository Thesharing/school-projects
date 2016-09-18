#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

/* “管理”界面，可以查看目前已经有注册了那些账户 */

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class configDialog;
}

class configDialog : public QDialog
{
    Q_OBJECT

public:
    explicit configDialog(QWidget *parent = 0);
    QSqlTableModel *model;
    ~configDialog();

private:
    Ui::configDialog *ui;
};

#endif // CONFIGDIALOG_H
