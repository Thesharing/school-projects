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
    ~configDialog();
    bool isCold = true;
    bool isFIFS = true;
    void closeEvent(QCloseEvent *);
    void accept();
private slots:
    void on_buttonOk_clicked();
    void on_buttonCancel_clicked();
    void on_buttonCold_clicked();
    void on_buttonHot_clicked();
    void on_buttonFIFS_clicked();
    void on_buttonHP_clicked();
signals:
    void configChanged();
private:
    Ui::configDialog *ui;
};

#endif // CONFIGDIALOG_H
