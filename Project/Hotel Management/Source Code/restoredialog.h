#ifndef RESTOREDIALOG_H
#define RESTOREDIALOG_H

#include <QDialog>

namespace Ui {
class restoreDialog;
}

class restoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit restoreDialog(QWidget *parent = 0);
    ~restoreDialog();

public slots:
    void restoreStatus(void);//绘制恢复时的进度窗口

private:
    Ui::restoreDialog *ui;

private slots:
    void restoreStatusUpdate(void);//绘制恢复时的进度窗口
    void on_okButton_clicked();
};

#endif // RESTOREDIALOG_H
