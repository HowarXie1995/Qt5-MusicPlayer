#ifndef COMMON_USE_H
#define COMMON_USE_H

#include <QMainWindow>
#include <QDebug>

enum return_value //返回值表
{
    EXIST_YES = 1000,        //存在
    EXIST_NO,                //不存在
    CHECK_PASWD_ERROR,       //密码错误
    RETURN_ERROR,             //出错
    RETURN_SUCCESS           //成功
};

//表示时间
enum day_time
{
    MORNING,
    NOON,
    AFTERNOON,
    EVENING,
    MIDNIGHT
};

//临时用户信息
typedef struct
{
    QString name;      //用户名
    QString passward;  //密码
    QString nickname;  //昵称
}User_Data;


#endif // COMMON_USE_H
