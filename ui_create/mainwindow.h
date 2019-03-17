#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <cstdio>
#include <QTime>

#include "common_use.h"
#include "f_log_in.h"
#include "mymplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:


    explicit MainWindow(QWidget *parent = 0);
    void clear_all_text();              //清空
    void initMymplayer();               //初始化
    ~MainWindow();

private slots:
    void on_bt_Register_clicked();      //按钮_跳转：注册页面

    void on_bt_R_giveup_clicked();      //按钮_跳转：转登录页面

    void on_bt_Login_clicked();         //按钮_输入:登录

    void on_bt_NowRegister_clicked();   //按钮_输入:注册

    void LogOut();                      //注销


    void on_labe_back_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    F_log_in *mydb;
    Mymplayer *myMplayerShow;
    User_Data userInf;
};

#endif // MAINWINDOW_H
