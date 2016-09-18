#ifndef BACKUPDIALOG_H
#define BACKUPDIALOG_H

#include <QDialog>

namespace Ui {
class backupDialog;
}

class backupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit backupDialog(QWidget *parent = 0);
    ~backupDialog();

public slots:
    void backupStatus(void);

private:
    Ui::backupDialog *ui;

private slots:
    void backupStatusUpdate(void); //绘制备份时的进度窗口
    void on_okButton_clicked(); //控制备份进度条的前进
};

#endif // BACKUPDIALOG_H
