#ifndef RESERVEDIALOG_H
#define RESERVEDIALOG_H

#include <QDialog>

namespace Ui {
class reserveDialog;
}

class reserveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit reserveDialog(QWidget *parent = 0);
    ~reserveDialog();

private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();

private:
    Ui::reserveDialog *ui;
};

#endif // RESERVEDIALOG_H
