#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QTimer>
#include <QtNetwork>
#include <QUdpSocket>
#include <QVector>

#include "slavedisplay.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QTimer * timer;
    int i = 0;
    ~MainWindow();
    QUdpSocket * socket;
    QVector<SlaveDisplay> slaveDisplayVector;
    void closeEvent(QCloseEvent *event);
    bool isBinded = false;
    void saveSetting();
    void changeColor(int color);
private slots:
    void updateFunction();
    void on_buttonPower_clicked();
    void processPendingDatagrams();
    void on_buttonReport_clicked();
    void on_buttonSetting_clicked();
    void on_buttonChekOut_clicked();
    void configChanged();
private:
    Ui::MainWindow *ui;
    void initLabels();
    void updateLabel();
    void sendSlaves();
};

#endif // MAINWINDOW_H
