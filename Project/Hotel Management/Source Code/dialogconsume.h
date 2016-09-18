#ifndef DIALOGCONSUME_H
#define DIALOGCONSUME_H

#include <QDialog>

namespace Ui {
class dialogConsume;
}

class dialogConsume : public QDialog
{
    Q_OBJECT

public:
    explicit dialogConsume(QWidget *parent = 0);
    ~dialogConsume();

private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();

private:
    Ui::dialogConsume *ui;
};

#endif // DIALOGCONSUME_H
