#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* 主界面 */

#include <QMainWindow>
#include <QSqlDatabase>
#include <QFileInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QSqlDatabase db;

    void createConnection(bool isExist);
    void updateInfo();

    ~MainWindow();

private slots:
    void on_buttonDeposit_clicked();
    void on_buttonEdit_clicked();
    void on_buttonWithdraw_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
