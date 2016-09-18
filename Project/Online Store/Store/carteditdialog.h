#ifndef CARTEDITDIALOG_H
#define CARTEDITDIALOG_H

#include <QDialog>

namespace Ui {
class cartEditDialog;
}

class cartEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cartEditDialog(QWidget *parent = 0, int num = 1);
    QString name;

    ~cartEditDialog();

private slots:
    void on_buttonDelete_clicked();
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();

private:
    Ui::cartEditDialog *ui;
};

#endif // CARTEDITDIALOG_H
