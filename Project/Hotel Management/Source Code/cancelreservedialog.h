#ifndef CANCELRESERVEDIALOG_H
#define CANCELRESERVEDIALOG_H

#include <QDialog>

namespace Ui {
class cancelreservedialog;
}

class cancelreservedialog : public QDialog
{
    Q_OBJECT

public:
    explicit cancelreservedialog(QWidget *parent = 0);
    ~cancelreservedialog();

private slots:
    void on_cancelButton_clicked();//当按下“取消”按钮时，关闭对话框
    void on_okButton_clicked();//当按下“确认”按钮时，对输入的信息进行处理，取消预约

private:
    Ui::cancelreservedialog *ui;
};

#endif // CANCELRESERVEDIALOG_H
