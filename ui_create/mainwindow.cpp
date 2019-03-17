#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mydb = new F_log_in(this);
    //1）设置窗口固定大小
    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinimizeButtonHint & Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setFixedSize(527, 347);
    //2)初始化
    initMymplayer();
    //3)设置当前页面
    ui->stackedWidget->setCurrentWidget(ui->page_login);
    //4)设置名字图标
    this->setWindowTitle("云播放器");
    QIcon icon("../image/UserInterface.png");
    this->setWindowIcon(icon);
}

//公有函数：清空所有text
void MainWindow::clear_all_text()
{
    ui->line_L_Pasw->clear();
    ui->line_L_User->clear();
    ui->line_R_Pasw->clear();
    ui->line_R_RPasw->clear();
    ui->line_R_User->clear();
}

void MainWindow::initMymplayer()
{
    myMplayerShow = new Mymplayer;
    connect(myMplayerShow, SIGNAL(showMainWindow()), this, SLOT(LogOut()));
}


void MainWindow::LogOut()
{
    ui->stackedWidget->setCurrentWidget(ui->page_login);
    clear_all_text();
    show();
}

MainWindow::~MainWindow()
{
    delete myMplayerShow;
    delete ui;
}

//按钮_跳转：注册页面
void MainWindow::on_bt_Register_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_register);
}

//按钮_跳转：转登录页面
void MainWindow::on_bt_R_giveup_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_login);
}

//按钮_跳转：转登录页面
void MainWindow::on_labe_back_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_login);
}

//按钮_输入:登录
void MainWindow::on_bt_Login_clicked()
{
    int login;             //判断返回值

    //1）从两个登录用户名密码中，获取值
    //判断是否为空
    if(ui->line_L_Pasw->text().isEmpty() || ui->line_L_User->text().isEmpty())
    {
        QMessageBox::critical(0 , "温馨提醒" , "输入非法",   \
                              QMessageBox::Ok | QMessageBox::Default ,  0 );
        clear_all_text();
        return ;
    }
    userInf.name = ui->line_L_User->text();
    userInf.passward = ui->line_L_Pasw->text();

    //2）把用户名，密码进入数据库核对
    login = mydb->Fun_Login(&userInf);
    switch(login)
    {
    case EXIST_NO:         //返回1:用户名不存在
        QMessageBox::critical(0 , "温馨提醒" , "用户名未注册或输入错误",   \
                              QMessageBox::Ok | QMessageBox::Default ,  0 );
        clear_all_text();
        return ;
    case CHECK_PASWD_ERROR://返回2:密码错误
        QMessageBox::critical(0 , "温馨提醒" , "密码输入错误,请重新输入",   \
                              QMessageBox::Ok | QMessageBox::Default ,  0 );
        //清空密码框
        ui->line_L_Pasw->clear();
        return ;
    case RETURN_ERROR:         //返回3:异常退出
        QMessageBox::critical(0 , "温馨提醒" , "异常错误", \
                              QMessageBox::Ok | QMessageBox::Default ,  0 );;
        return ;
    case RETURN_SUCCESS:       //登录成功
        //判断是否取昵称
        switch(mydb->isNickName(&userInf))
        {
        case EXIST_NO:      //不存在
            ui->line_L_Pasw->clear();
            ui->stackedWidget->setCurrentWidget(ui->page_nickname);
            break;
        case RETURN_ERROR:  //异常退出
            QMessageBox::critical(0 , "温馨提醒" , "异常错误", \
                                  QMessageBox::Ok | QMessageBox::Default ,  0 );;
            break;
        case EXIST_YES:     //成功

            myMplayerShow->getUserInf(userInf);
            //显示用户界面
            this->hide();
            myMplayerShow->show();
        }

    }

}




//按钮_输入:注册
void MainWindow::on_bt_NowRegister_clicked()
{
    bool ok = 0;                    //查错
    int exist;                      //判断是否存在

    //1）判断是否为空
    if(ui->line_R_Pasw->text().isEmpty() || ui->line_R_User->text().isEmpty() \
            || ui->line_R_RPasw->text().isEmpty())
    {
        QMessageBox::critical(0 , "温馨提醒" , "输入非法",   \
                              QMessageBox::Ok | QMessageBox::Default ,  0 );
        clear_all_text();
        return ;
    }
    //2）接收注册三个框内的字符串
    userInf.name = ui->line_R_User->text();
    userInf.passward = ui->line_R_Pasw->text();
    QString user_repassward = ui->line_R_RPasw->text();
    //3）查看用户名是否存在
    exist = mydb->is_DataExist(&userInf);
    switch(exist)
    {
    case EXIST_YES:
        QMessageBox::critical(0 , "温馨提醒" , "用户名已存在", \
                              QMessageBox::Ok | QMessageBox::Default ,  0 );;
        clear_all_text();
        return ;
    case RETURN_ERROR:
        QMessageBox::critical(0 , "温馨提醒" , "异常错误", \
                              QMessageBox::Ok | QMessageBox::Default ,  0 );;
        return ;
    }
    //4）比较密码与再次输入密码是否一致
    if(userInf.passward != user_repassward)
    {
        QMessageBox::critical(0 , "温馨提醒" , "前后密码输入不一致",      \
                              QMessageBox::Ok | QMessageBox::Default ,  0 );
        //清空密码框
        ui->line_R_Pasw->clear();
        ui->line_R_RPasw->clear();
        return ;
    }

    //5）存入数据库
    ok = mydb->Fun_Register(&userInf);
    if(!ok)
    {
        QMessageBox::critical(0 , "温馨提醒" , "异常错误", \
                              QMessageBox::Ok | QMessageBox::Default ,  0 );;
        return ;
    }else
    {
        //注册成功自动跳转登录
        QMessageBox::information(0, "恭喜", "注册成功",   \
                                 QMessageBox::Ok | QMessageBox::Default, 0);
        ui->stackedWidget->setCurrentWidget(ui->page_login);

    }

}


//按钮_输入:昵称
void MainWindow::on_pushButton_clicked()
{
    //1）判断是否为空
    if(ui->line_nickname->text().isEmpty())
    {
        QMessageBox::critical(0 , "温馨提醒" , "输入合理的昵称",   \
                              QMessageBox::Ok | QMessageBox::Default ,  0 );
        return ;
    }

    //2）接收昵称
    userInf.nickname = ui->line_nickname->text();

    //3）写入数据库
    bool ok = mydb->setNickName(&userInf);
    if(!ok)
    {
        QMessageBox::critical(0 , "温馨提醒" , "异常错误", \
                              QMessageBox::Ok | QMessageBox::Default ,  0 );;
        return ;
    }else
    {
        ui->label_nickname->setText(QString("%1").arg(userInf.nickname));
        ui->stackedWidget->setCurrentWidget(ui->page_show);
        //延时2s
        QTime dieTime = QTime::currentTime().addMSecs(2000);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        myMplayerShow->getUserInf(userInf);
        //显示用户界面
        this->hide();
        myMplayerShow->show();
    }
}
