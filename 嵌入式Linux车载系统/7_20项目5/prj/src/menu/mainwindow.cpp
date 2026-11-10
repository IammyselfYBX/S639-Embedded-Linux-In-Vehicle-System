#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    net_manager = new QNetworkAccessManager(this);//创建网络管理对象

    //开机后首先更新网络时间
    timeupdate_timer.start(2000);
    connect(&timeupdate_timer,&QTimer::timeout,[=](){
        datetime_update();
    });
    menupage_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::datetime_update()
{

    //在获取网络时间成功后，停止继续获取
    timeupdate_timer.stop();

    net_manager->get(QNetworkRequest(QUrl(GET_TIME_ONLINE)));
    connect(net_manager,&QNetworkAccessManager::finished,[=](QNetworkReply *reply){



        //处理数据
        QString msg = reply->readAll();

        //解析JSON数据
        QJsonDocument json = QJsonDocument::fromJson(msg.toUtf8());
        QJsonObject obj;
        //判断是否包含一个对象
        if(json.isObject())
        {
            obj = json.object();
        }

        //取出result对象
        obj = obj.value("result").toObject();

        //取出时间
        QString datatime = obj.value("datetime_1").toString();
        QString set_datetime  = "date -s ";
        set_datetime = set_datetime + '\"' + datatime + '\"';
        //qDebug() << set_datetime;
        system(set_datetime.toUtf8());
        system("hwclock -w");
        net_manager->deleteLater();
    });
}

void MainWindow::menupage_init()
{
    //设置背景图片
    this->setStyleSheet("MainWindow{background-color: rgb(85, 85, 127);}");

    /*显示时间和日期*/
    timeshow = new QLabel(this);
    QFont font_time("Microsoft YaHei",30,30);
    font_time.setBold(true);
    timeshow->setFont(font_time);
    timeshow->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    timeshow->setGeometry(50,0,300,100);
    timeshow->setStyleSheet("QLabel { color: white}");


    dateshow = new QLabel(this);
    QFont font_deta("Microsoft YaHei",15,10);
    font_deta.setBold(true);
    dateshow->setFont(font_deta);
    dateshow->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    dateshow->setGeometry(50,100,300,100);
    dateshow->setStyleSheet("QLabel { color: white}");


    datetime_show.start(1000);
    connect(&datetime_show,&QTimer::timeout,[=](){
        timeshow->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
        dateshow->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd ddd"));
    });

    //创建一个进程管理对象
    pro = new QProcess(this);
    //定时器，扫描进程状态
    pro_timer = new QTimer(this);

    //显示按钮
    weather = new QPushButton(this);
    weather->setGeometry(30,270,200,180);
    weather->setStyleSheet("border-image: url(:/pic/weather.png);");
    weather->setFocusPolicy(Qt::NoFocus);
    connect(weather,&QPushButton::clicked,[=](){
        pro->close();
        pro->start("./weather");
        pro_timer->start(200);
    });

    music = new QPushButton(this);
    music->setGeometry(280,260,200,200);
    music->setStyleSheet("border-image: url(:/pic/music.png);");
    music->setFocusPolicy(Qt::NoFocus);
    connect(music,&QPushButton::clicked,[=](){
        pro->close();
        pro->start("./music");
        pro_timer->start(200);
    });

    video = new QPushButton(this);
    video->setGeometry(560,260,200,200);
    video->setStyleSheet("border-image: url(:/pic/video.png);");
    video->setFocusPolicy(Qt::NoFocus);
    connect(video,&QPushButton::clicked,[=](){
        pro->close();
        pro->start("./video");
        pro_timer->start(200);
    });

    map = new QPushButton(this);
    map->setGeometry(560,10,200,200);
    map->setStyleSheet("border-image: url(:/pic/map.png);");
    map->setFocusPolicy(Qt::NoFocus);
    connect(map,&QPushButton::clicked,[=](){
        pro->close();
        pro->start("./map");
        pro_timer->start(200);
    });

    //pro_timer->start(200);
    connect(pro_timer,&QTimer::timeout,[=](){
        if(pro->state() == QProcess::NotRunning)
        {
            this->show();
            pro_timer->stop();

        }
        else
        {
            this->hide();

        }
    });
}

