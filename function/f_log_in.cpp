#include "f_log_in.h"

F_log_in::F_log_in(QObject *parent) : QObject(parent)
{
    init_dataBase();
}

/*
 * 公有:打开创建数据库
 */
void F_log_in::init_dataBase()
{
    bool ok;                //判断错误
    //1）连接或创建数据库
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Z:/Share_Code/Qt_Program/dataBase/User_data.db");
    //2）打开数据库
    ok = db.open();
    if(!ok)
    {
        qDebug() << "open db error: " << db.lastError().text();
    }else
    {
        QSqlQuery query;
        QString sql = "create table if not exists "             \
                      "User_Data(user_name text primary key, "  \
                      "user_passward text not null, user_nickname text);";
        ok = query.exec(sql);
        if(!ok)
        {
            qDebug() << "open db error: " << db.lastError().text();
            db.close();
            exit(1);
        }
    }
    db.close();
}

/*
 * 函数功能：判断数据是否存在
 * 返回值：枚举return_value
 *        EXIST_YES                //存在
 *        EXIST_NO                 //不存在
 *        RETURN_ERROR              //出错
 *
 */
int F_log_in::is_DataExist(User_Data *userInf)
{
    QString select_sql;         //查询命令字符串
    bool ok = 0;                //判断对错
    QSqlQuery query;            //数据库返回变量

    //1）打开数据库
    if(!db.open())
    {
        qDebug() << "open db error: " << db.lastError().text();
        return RETURN_ERROR;
    }
    //2）构造查询命令并执行
    select_sql = QString("select user_name from User_Data where user_name='%1';").arg(userInf->name);
    ok = query.exec(select_sql);
    if(ok)
    {
        if(query.next())
        {
            db.close();
            return EXIST_YES;
        }else
        {
            db.close();
            return EXIST_NO;
        }
    }else
    {
        qDebug() << "exec error: " << db.lastError().text();
        db.close();
        return RETURN_ERROR;
    }
}
/*
 *  函数功能:把数据写入数据库
 *  返回值:0 写入失败
 *        1  写入成功
 */
bool F_log_in::Fun_Register(User_Data *userInf)
{
    bool ok = 0;            //判断对错
    QString insert_sql;     //加入命令字符串
    QSqlQuery query;

    //1）打开数据库
    if(!db.open())
    {
        qDebug() << "open db error: " << db.lastError().text();
        return false;
    }

    //2）构造加入命令并执行
    insert_sql = QString("insert into User_Data(user_name,use" \
                         "r_passward)values('%1','%2');").arg(userInf->name).arg(userInf->passward);
    ok = query.exec(insert_sql);
    if(!ok)         //执行失败
    {
        qDebug() << query.lastError();
        db.close();
        return false;
    }else           //执行成功
    {
        db.close();
        return true;
    }
}

/*
 *  函数功能:把数据写入数据库
 *  返回值:0 写入失败
 *        1  写入成功
 */
int F_log_in::Fun_Login(User_Data *userInf)
{
    QString check_sql;     //加入命令字符串
    QSqlQuery query;
    bool ok = 0;           //判断对错

    //1）打开数据库
    if(!db.open())
    {
        qDebug() << "open db error: " << db.lastError().text();
        return RETURN_ERROR;
    }

    //2）构造加入命令并执行
    check_sql = QString("select user_passward from User_Data " \
                         "where user_name='%1';").arg(userInf->name);
    ok = query.exec(check_sql);
    if(!ok)
    {
        qDebug() << query.lastError();
        db.close();
        return RETURN_ERROR;
    }else if(!query.next())            //错误：未找到用户名
    {
        db.close();
        return EXIST_NO;
    }else                               //找到用户名
    {
        QString passward = query.value(0).toString();
        if( passward != userInf->passward )
        {
            db.close();
            return CHECK_PASWD_ERROR;
        }
    }
    db.close();
    return RETURN_SUCCESS;
}

/*
 * 函数功能：设置昵称
 * 返回值：
 *      0  失败
 *      1  成功
 */
bool F_log_in::setNickName(User_Data *userInf)
{
    bool ok;
    QString check_sql;     //加入命令字符串
    QSqlQuery query;

    //1）打开数据库
    if(!db.open())
    {
        qDebug() << "open db error: " << db.lastError().text();
        return false;
    }
    //2）构造加入命令并执行
    check_sql = QString("UPDATE User_Data SET user_nickname='%1'" \
                        "where user_name='%2';").arg(userInf->nickname).arg(userInf->name);
    ok = query.exec(check_sql);
    if(!ok)         //执行失败
    {
        qDebug() << query.lastError();
        db.close();
        return false;
    }else           //执行成功
    {
        db.close();
        return true;
    }
}

/*
 * 函数功能：判断是否有昵称
 * 返回值：
 *   EXIST_YES                //存在
 *   EXIST_NO                 //不存在
 *   RETURN_ERROR             //出错
 */
int F_log_in::isNickName(User_Data *userInf)
{
    QString select_sql;         //查询命令字符串
    bool ok;                //判断对错
    QSqlQuery query;            //数据库返回变量

    //1）打开数据库
    if(!db.open())
    {
        qDebug() << "open db error: " << db.lastError().text();
        return RETURN_ERROR;
    }
    //2）构造查询命令并执行
    select_sql = QString("select user_nickname from User_Data where"      \
                         " user_name='%1';").arg(userInf->name);
    ok = query.exec(select_sql);
    if(ok)
    {
      query.next();
      userInf->nickname = query.value(0).toString();
      if(userInf->nickname == "")
      {
          db.close();
          return EXIST_NO;
      }
      return EXIST_YES;
    }else
    {
        qDebug() << "exec error: " << db.lastError().text();
        db.close();
        return RETURN_ERROR;
    }
}




















