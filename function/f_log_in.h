#ifndef F_LOG_IN_H
#define F_LOG_IN_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "common_use.h"


class F_log_in: public QObject
{
    Q_OBJECT
public:

    explicit F_log_in(QObject *parent = nullptr);

    void init_dataBase();                                             //初始化数据库
    int is_DataExist(User_Data *userInf);                             //判断数据是否存在
    bool Fun_Register(User_Data *userInf);                            //注册函数
    int Fun_Login(User_Data *userInf);                                //登录函数
    bool setNickName(User_Data *userInf);                             //设置昵称
    int isNickName(User_Data *userInf);                               //判断是否有昵称

private:
    QSqlDatabase db;                //数据库函数

};

#endif // F_LOG_IN_H
