#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QFileInfo>
#include <QSqlTableModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QSqlDatabase db;
    QSqlTableModel *model;

    void createConnection(bool isExist);
    void initialize();
    void updateUI();
    static int askBank(int function,QString str);

    ~MainWindow();

private slots:
    void on_tableView_clicked(const QModelIndex &index);
    void on_buttonCart_clicked();
    void on_buttonExit_clicked();
    void displayType();
    void on_buttonUserConfig_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
