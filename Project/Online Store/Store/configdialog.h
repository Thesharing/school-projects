#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class configDialog;
}

class configDialog : public QDialog
{
    Q_OBJECT

public:
    explicit configDialog(QWidget *parent = 0);
    float rateBook;
    float rateCloth;
    float rateFood;
    float rateElectro;
    ~configDialog();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::configDialog *ui;
};

#endif // CONFIGDIALOG_H
