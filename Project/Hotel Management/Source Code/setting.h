#ifndef SETTING_H
#define SETTING_H

#include <QDialog>

namespace Ui {
class setting;
}

class setting : public QDialog
{
    Q_OBJECT

public:
    explicit setting(QWidget *parent = 0);
    ~setting();

private slots:
    void on_cancelButton_clicked();
    void pricechange();//改变系统设置信息
    void pricereset();//设置系统设置为默认值

private:
    Ui::setting *ui;
};

#endif // SETTING_H
