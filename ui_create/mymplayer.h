#ifndef MYMPLAYER_H
#define MYMPLAYER_H

#include <QMainWindow>
#include <QDebug>
#include <QProcess>


#include "f_mymplayer.h"
#include "common_use.h"

namespace Ui {
class Mymplayer;
}

class Mymplayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mymplayer(QWidget *parent = 0);
    void showDayTime();                     //显示晚上，早上好，之类的东西
    void getUserInf(User_Data infor);       //用户数据传递
    void init_local_widget();               //初始化本地歌曲列表
    void init_local_music();                //默认目录
    ~Mymplayer();                           //析构函数

signals: //信号:显示登录
    void showMainWindow();

private slots:
    void initProcess();

    void on_actionlogout_triggered();
    void lo_timeout();

    void on_bt_play_music_clicked();

    void on_bt_pause_music_clicked();

    void on_bt_home_clicked();

    void on_bt_localmusic_clicked();

    void on_bt_add_music_clicked();

    void on_bt_start_clicked();

    void onStarted();
    void onError();
    void onReadData();

    void on_bt_pre_music_clicked();

    void on_bt_next_music_clicked();

private:
    Ui::Mymplayer *ui;          //UI
    User_Data userInf;          //用户数据
    F_mymplayer *mym;           //功能函数接口
    QTimer *timer;              //时间
    QStringList list;           //保存歌曲数据
    QString path;               //保存路径
    QProcess *Mprocess;         //要执行的进程
    QStringList order_music;    //播放命令
    int music_num;              //歌曲下标
};

#endif // MYMPLAYER_H
