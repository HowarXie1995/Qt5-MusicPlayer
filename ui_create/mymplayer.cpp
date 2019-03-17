#include "mymplayer.h"
#include "ui_mymplayer.h"

Mymplayer::Mymplayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mymplayer)
{
    ui->setupUi(this);
    //1)设置名字图标
    this->setWindowTitle("云播放器");
    QIcon icon("../image/UserInterface.png");
    this->setWindowIcon(icon);
    showDayTime();
    //2)创建定时器
    timer = new QTimer(this);
    //3)关联信号和槽
    connect(timer, SIGNAL(timeout()), this, SLOT(lo_timeout()));
    timer->start(1000);
    //4)
    ui->stackedWidget->setCurrentWidget(ui->page_basic);
    ui->bt_start->setVisible(true);
    ui->bt_play_music->setVisible(false);
    ui->bt_pause_music->setVisible(false);
    //5)设置本地音乐格式
    init_local_widget();
    //6)设置本地音乐默认目录
    init_local_music();
    //7)初始化
    order_music << "-slave" << "-quiet";
    music_num = 0;
    //8）
    initProcess();
}

void Mymplayer::showDayTime()
{
    int daytime = -1;
    daytime = mym->getDayTime();
    switch (daytime)
    {
        case MORNING:
                    ui->label_showtime->setText("早上好!");break;
        case NOON:
                    ui->label_showtime->setText("中午好!");break;
        case AFTERNOON:
                    ui->label_showtime->setText("下午好!");break;
        case EVENING:
                    ui->label_showtime->setText("晚上好!");break;
        case MIDNIGHT:
                    ui->label_showtime->setText("夜深了!");break;
    }
}

void Mymplayer::getUserInf(User_Data infor)
{
    userInf = infor;
    ui->label_name->setText(userInf.nickname);
}

//函数:设置本地音乐表格格式
void Mymplayer::init_local_widget()
{
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setShowGrid(false);            //取消线
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);   //选中一行
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);  //只能选一行
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);    //内容不能修改
    ui->tableWidget->setAlternatingRowColors(true);                         //黑白相间
}

//函数：设置本地文件夹默认目录
void Mymplayer::init_local_music()
{
    path = "R:/CloudMusic";
    QDir dir(path);

    //1)获取目录下所有文件名字,装入list内
    list = dir.entryList();
    //2)去除list内的不是.mp3
    list = mym->is_Mp3File(list);
    int i = 0;
    while(i < list.size())
    {
        int RowCout;
        RowCout = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(RowCout);
        ui->tableWidget->setItem(RowCout, 0, new QTableWidgetItem(list[i]));
        i++;
    }
}

void Mymplayer::lo_timeout()
{
    QDateTime dt = QDateTime::currentDateTime();
    QString strTime = dt.toString("hh:mm:ss");
    ui->label_time->setText(strTime);
}

Mymplayer::~Mymplayer()
{
    Mprocess->close();
    delete ui;
}

//函数：初始化进程
void Mymplayer::initProcess()
{
    //1）创建一个新进程
    Mprocess = new QProcess(this);
    Mprocess->setProcessChannelMode(QProcess::MergedChannels);
    //2）成功发出成功信号
    connect(Mprocess, SIGNAL(started()), this, SLOT(onStarted()));
    //3）出错发出出错信号
    connect(Mprocess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onError()));
    //4）有数据输出
    connect(Mprocess, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadData()));
#ifdef Q_OS_WIN
    Mprocess->setProgram("Z:/MPlayer_Windows/MPlayer_core.exe");
#else
    Mprocess->setProgram("/usr/bin/mplayer");
#endif
    if(list.isEmpty())
    {
        return ;
    }
    //准备参数列表
    int num = music_num;
    while(num < list.size())
    {
        QString mp3 = path + "/" +list[num++];
        order_music << mp3;
    }

    qDebug() << order_music;
    Mprocess->setArguments(order_music);



}

//返回到登录界面
void Mymplayer::on_actionlogout_triggered()
{
    this->hide();
    emit showMainWindow();
}

//按钮开始播放 从QList里面读取
void Mymplayer::on_bt_start_clicked()
{
    //1）设置初始按钮状态
    ui->bt_start->setVisible(false);
    ui->bt_play_music->setVisible(false);
    ui->bt_pause_music->setVisible(true);
    Mprocess->start();
}

void Mymplayer::onStarted()
{
    qDebug() << "started ok!";
}
void Mymplayer::onError()
{
    qDebug() << "process error: " << Mprocess->errorString();
}
void Mymplayer::onReadData()
{
    while(Mprocess->canReadLine())
        {
            QString line = Mprocess->readLine();
            if(line.contains("ANS_LENGTH"))
                qDebug() << "read line : " << line;
        }
}


//按钮：播放
void Mymplayer::on_bt_play_music_clicked()
{
    ui->bt_play_music->setVisible(false);
    ui->bt_pause_music->setVisible(true);
}

//按钮：暂停
void Mymplayer::on_bt_pause_music_clicked()
{
    ui->bt_play_music->setVisible(true);
    ui->bt_pause_music->setVisible(false);
}


//按钮：返回主页
void Mymplayer::on_bt_home_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_basic);
}

//按钮：本地音乐
void Mymplayer::on_bt_localmusic_clicked()
{
    //1）跳转到本地音乐页面
    ui->stackedWidget->setCurrentWidget(ui->page_local);
}

//按钮：添加歌曲
void Mymplayer::on_bt_add_music_clicked()
{
    //1)获取一个存在的 目录名字
    path = QFileDialog::getExistingDirectory(this);
    if(path.isEmpty())
    {
          return ;
    }
    //2)清除行数据
    int iLen = ui->tableWidget->rowCount();
    for(int i = 0; i < iLen; i++)
    {
        ui->tableWidget->removeRow(0);
    }
    //3)构建目录对象
    QDir dir(path);

    //4)获取目录下所有文件名字,装入list内
    list = dir.entryList();
    //5)去除list内的不是.mp3
    list = mym->is_Mp3File(list);
    int i = 0;
    while(i < list.size())
    {
        int RowCout;
        RowCout = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(RowCout);
        ui->tableWidget->setItem(RowCout, 0, new QTableWidgetItem(list[i]));
        i++;
    }


}

//函数：上一曲
void Mymplayer::on_bt_pre_music_clicked()
{
    if(list.size() == 0)
        return;
    music_num--;
    if(music_num < 0)
    {
        music_num = list.size() - 1;
    }
    order_music.clear();
    order_music << "-slave" << "-quiet";
    delete Mprocess;
    initProcess();
    on_bt_start_clicked();
}

//函数：下一曲
void Mymplayer::on_bt_next_music_clicked()
{
    if(list.size() == 0)
        return;
    qDebug() << music_num;
    music_num++;
    if(music_num >= list.size())
    {
        music_num = 0;
    }
    qDebug() << music_num;
    order_music.clear();
    order_music << "-slave" << "-quiet";
    delete Mprocess;
    initProcess();
    on_bt_start_clicked();
}
