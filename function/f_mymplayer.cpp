#include "f_mymplayer.h"

F_mymplayer::F_mymplayer(QObject *parent) : QObject(parent)
{

}

int F_mymplayer::getDayTime()
{
    //1）获取系统时间
    QDateTime dt = QDateTime::currentDateTime();
    //2）把当前时间转换为字符型
    QString strHours = dt.toString("HH");
    bool ok;
    int timer = strHours.toInt(&ok, 10);
    if(timer >= 0 && timer < 6)
    {
        return MIDNIGHT;
    }else if(timer >= 6 && timer < 10)
    {
        return MORNING;
    }else if(timer >= 10 && timer < 13)
    {
        return NOON;
    }else if(timer >= 13 && timer < 19)
    {
        return AFTERNOON;
    }else
    {
        return EVENING;
    }

}

//函数:判断是否为.mp3文件
QStringList F_mymplayer::is_Mp3File(QStringList list)
{
    int i = 0;
    QFileInfo fileinfo;
    QStringList return_list;
    while(i < list.size())
    {
        fileinfo = QFileInfo(list[i]);
        QString temp = fileinfo.suffix();
        if(temp == "mp3") //获取文件后缀名
        {
            return_list << list[i];
        }
        i++;
    }
    return return_list;
}
