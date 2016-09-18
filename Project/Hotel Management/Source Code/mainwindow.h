#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void roomButtonChange(); //用于更新主窗口的”客房信息“里的按钮状态
    void createConnection(bool isExist); //创建数据库
    void sqlInitialize(); //将数据库中的“房间信息”进行初始化，将房间信息写入
    QSqlTableModel *model;//顾客信息表的模型，用于显示顾客信息，和数据库中的“顾客信息guestInfo”相关联
    QSqlTableModel *modelLog;//日志信息表的模型，用于显示日志内容，和数据库中的“日志信息log”相关联
    QSqlTableModel *modelReserve;//预约信息表的模型，用于显示预约信息，和数据库中的“预约信息bookInfo”相关联
    QSqlDatabase db; //数据库
    void closeEvent(QCloseEvent *event); //当主程序被关闭时，进行备份操作

private slots:
    void on_layerComboBox_currentIndexChanged(int index);//当楼层数被更改时更改layerShow（当前显示楼层数）并更新一下图标状态
    void progressUpdate();//通过进度条来更新时间，并在中午12点提示退房和午夜0点将过期预约删除
    void guestFunction(); //显示顾客界面

    void login();//登记函数
    void logout();//离店函数
    void search();//顾客信息查找函数，searchmodel为1时为模糊搜索，0时为精确搜索
    void searchclear();//重置顾客查找信息
    void searchReserve();//预约信息查找函数，searchmodel为1时为模糊搜索，0时为精确搜索
    void searchclearReserve();//重置预约查找信息
    void searchLog();//日志信息查找函数，searchmodel为1时为模糊搜索，0时为精确搜索
    void searchclearLog();//重置日志查找信息
    void count();//统计函数

    void backupFunction();//备份函数
    void restoreFunction();//还原函数
    int startRestoreFunction(); //在程序开始运行进行恢复
    void initialFunction(); //在程序开始时，如果不从备份中恢复则进行初始化
    void helpFunction(); //绘制帮助界面
    void aboutFunction(); //绘制“关于”界面

    void settingFunction(); //设置系统参数

    //当“客房信息”中各房间的按钮被按下时，显示房间信息
    void on_roomButton1_clicked();
    void on_roomButton2_clicked();
    void on_roomButton3_clicked();
    void on_roomButton4_clicked();
    void on_roomButton5_clicked();
    void on_roomButton6_clicked();
    void on_roomButton7_clicked();
    void on_roomButton8_clicked();
    void on_roomButton9_clicked();
    void on_roomButton10_clicked();
    void on_roomButton11_clicked();
    void on_roomButton12_clicked();
    void on_roomButton13_clicked();
    void on_roomButton14_clicked();
    void on_roomButton15_clicked();
    void on_roomButton16_clicked();
    void on_roomButton17_clicked();
    void on_roomButton18_clicked();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QMessageBox *messagebox1;
    QMessageBox *messagebox2;
    QMessageBox *messagebox3;
    QMessageBox *messagebox4;
    QMessageBox *messagebox5;
    QMessageBox *messagebox6;
    QMessageBox *messagebox7;

    //当鼠标、键盘移动/按下时停止时间
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void keyPressEvent(QMouseEvent * event);
};

#endif // MAINWINDOW_H
