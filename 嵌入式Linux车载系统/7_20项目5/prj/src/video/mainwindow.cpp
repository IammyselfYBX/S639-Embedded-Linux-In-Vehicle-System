#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timetotal = 0;
    timecurrent = 0;
    play_state = false;
    pro = new QProcess(this);
    exit_button = new QPushButton(this);//退出按钮
    exit_button->setStyleSheet("QPushButton { color: white;background: transparent;border-image:transparent}");//
    exit_button->setGeometry(750,455,40,20);
    exit_button->setText("Exit");
    exit_button->setFont(QFont("Microsoft YaHei",10,20));
    exit_button->setFocusPolicy(Qt::NoFocus);
    connect(exit_button,&QPushButton::clicked,[&](){
        pro->close();
        exit(0);
    });
    videopage_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::videopage_init()
{
    this->setStyleSheet("MainWindow{background-image: url(:/pic/video_background.jpg);}");//背景

    //创建进度条
    //bar = new QProgressBar(this);
    //bar->setTextVisible(false);
    //bar->setGeometry(0,420,800,7);

    //创建进度控制条
    play_bar = new QSlider(this);
    play_bar->setGeometry(0,420,800,7);
    play_bar->setMinimum(0);
    play_bar->setMaximum(100);
    play_bar->setValue(0);
    play_bar->setOrientation(Qt::Horizontal);
    play_bar->setStyleSheet(MEDIASLIDER_STYLE);
    connect(play_bar,&QSlider::sliderMoved,[&](){
        //qDebug() << "Moved";

    });

    connect(play_bar,&QSlider::sliderPressed,[&](){
        //qDebug() << "Pressed";
        update_play_bar.stop();
    });

    connect(play_bar,&QSlider::sliderReleased,[&](){
        //qDebug() << "Released";
        QString tmp = QString("SEEK %1 2\n").arg(play_bar->value());
        pro->write(tmp.toUtf8());
        if(play_state == false)
        {
            pro->write("pause \n");
            timecurrent = play_bar->value();
        }
        else
        {
            update_play_bar.start(1000);
        }

    });


    //播放按钮
    play_button = new QPushButton(this);
    play_button->setFocusPolicy(Qt::NoFocus);
    play_button->setStyleSheet("border-image: url(:/pic/pause.png);");
    play_button->setGeometry(375,435,35,35);
    connect(play_button,&QPushButton::clicked,this,&MainWindow::play_button_action);



    //快退按钮
    slow_button = new QPushButton(this);
    slow_button->setFocusPolicy(Qt::NoFocus);
    slow_button->setStyleSheet("border-image: url(:/pic/slow.png);");
    slow_button->setGeometry(320,435,35,35);
    connect(slow_button,&QPushButton::clicked,[&](){
        pro->write("SEEK -5\n");
        send_cmd.start(1000);
        update_play_bar.start(1000);
        play_state = true;
        play_button->setStyleSheet("border-image: url(:/pic/play.png);");
    });

    //快进按钮
    fast_button = new QPushButton(this);
    fast_button->setFocusPolicy(Qt::NoFocus);
    fast_button->setStyleSheet("border-image: url(:/pic/fast.png);");
    fast_button->setGeometry(425,435,35,35);
    connect(fast_button,&QPushButton::clicked,[&](){
        pro->write("SEEK +5\n");
        send_cmd.start(1000);
        update_play_bar.start(1000);
        play_state = true;
        play_button->setStyleSheet("border-image: url(:/pic/play.png);");
    });

    //上一曲
    prev_button = new QPushButton(this);
    prev_button->setFocusPolicy(Qt::NoFocus);
    prev_button->setStyleSheet("border-image: url(:/pic/prev.png);");
    prev_button->setGeometry(270,435,35,35);
    connect(prev_button,&QPushButton::clicked,[&](){
        if(video_list_dispaly->currentRow() == 0)
        {
            video_list_dispaly->setCurrentRow(video_list_dispaly->count()-1);
        }
        else
        {
            video_list_dispaly->setCurrentRow(video_list_dispaly->currentRow()-1);
        }
        //双击开始播放
        pro->close();
        play_bar->setValue(0);
        timecurrent = 0;
        timetotal = 0;
        QString cmd = QString("mplayer -ac mad -geometry 0:0 -zoom -x 700 -y 420 -slave -quiet %1")
                       .arg(video_list[video_list_dispaly->currentRow()]);
        pro->start(cmd);
        pro->write(QString("volume %1 1\n").arg(volume_bar->value()).toUtf8());
        play_state = true;
        play_button->setStyleSheet("border-image: url(:/pic/play.png);");
        pro->write("get_time_length\n");
        send_cmd.start(1000);
    });

    //下一曲
    next_button = new QPushButton(this);
    next_button->setFocusPolicy(Qt::NoFocus);
    next_button->setStyleSheet("border-image: url(:/pic/next.png);");
    next_button->setGeometry(475,435,35,35);
    connect(next_button,&QPushButton::clicked,[&](){
        if(video_list_dispaly->currentRow() == video_list_dispaly->count()-1)
        {
            video_list_dispaly->setCurrentRow(0);
        }
        else
        {
            video_list_dispaly->setCurrentRow(video_list_dispaly->currentRow()+1);
        }
        //双击开始播放
        pro->close();
        play_bar->setValue(0);
        timecurrent = 0;
        timetotal = 0;
        QString cmd = QString("mplayer -ac mad -geometry 0:0 -zoom -x 700 -y 420 -slave -quiet %1")
                       .arg(video_list[video_list_dispaly->currentRow()]);
        pro->start(cmd);
        pro->write(QString("volume %1 1\n").arg(volume_bar->value()).toUtf8());
        play_state = true;
        play_button->setStyleSheet("border-image: url(:/pic/play.png);");
        pro->write("get_time_length\n");
        send_cmd.start(1000);
    });

    //音量
    volume_button = new QPushButton(this);
    volume_button->setFocusPolicy(Qt::NoFocus);
    volume_button->setStyleSheet("border-image: url(:/pic/volume.png);");
    volume_button->setGeometry(20,435,35,35);
    connect(volume_button,&QPushButton::clicked,[&](){
        volume_bar->setVisible(true);
        volume_timer.start(2000);
        connect(&volume_timer,&QTimer::timeout,[&](){
            volume_timer.stop();
            volume_bar->setVisible(false);
        });
    });


    //音量控制条
    volume_bar = new QSlider(this);
    volume_bar->setGeometry(55,450,100,7);
    volume_bar->setMinimum(0);
    volume_bar->setMaximum(100);
    volume_bar->setValue(10);
    volume_bar->setOrientation(Qt::Horizontal);
    volume_bar->setStyleSheet(MEDIASLIDER_STYLE);
    volume_bar->setVisible(false);
    connect(volume_bar,&QSlider::sliderMoved,[&](){
            volume_timer.start();
    });
    connect(volume_bar,&QSlider::sliderPressed,[&](){
            volume_timer.start();
    });
    connect(volume_bar,&QSlider::sliderReleased,[&](){
        QString tmp = QString("volume %1 1\n").arg(volume_bar->value());
        if(play_state == true)
        {
            pro->write(tmp.toUtf8());
        }
        else
        {
            pro->write(tmp.toUtf8());
            usleep(500);
            pro->write("pause \n");
        }
    });

    //打开文件按钮
    open_file_button = new QPushButton(this);
    open_file_button->setFocusPolicy(Qt::NoFocus);
    open_file_button->setStyleSheet("border-image: url(:/pic/open.png);");
    open_file_button->setGeometry(700,455,20,20);
    connect(open_file_button,&QPushButton::clicked,[&](){
        //初始化文件列表
        video_list_init();
    });

    //播放时间
    timelength = new QLabel(this);
    timelength->setFont(QFont("Microsoft YaHei",10,10));
    timelength->setStyleSheet("QLabel { color: white}");
    timelength->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    timelength->setText("0:00/0:00");
    timelength->setGeometry(710,428,80,20);
    connect(&send_cmd,&QTimer::timeout,[&](){
        pro->write("get_time_pos\n");
    });


    //视频列表
    video_list_dispaly = new QListWidget(this);
    video_list_dispaly->setFont(QFont("Microsoft YaHei",10,10));
    video_list_dispaly->setGeometry(700,0,100,420);
    video_list_dispaly->setStyleSheet("QListWidget { color: black;background: transparent;border-image:transparent}");
    connect(video_list_dispaly,&QListWidget::itemDoubleClicked,[&](){
        //双击开始播放
        pro->close();
        play_bar->setValue(0);
        QString cmd = QString("mplayer -ac mad -geometry 0:0 -zoom -x 700 -y 420 -slave -quiet %1")
                       .arg(video_list[video_list_dispaly->currentRow()]);
        pro->start(cmd);
        pro->write(QString("volume %1 1\n").arg(volume_bar->value()).toUtf8());
        play_state = true;
        play_button->setStyleSheet("border-image: url(:/pic/play.png);");
        pro->write("get_time_length\n");
        send_cmd.start(1000);


    });
    connect(pro,&QProcess::readyRead,this,&MainWindow::read);
    connect(pro,&QProcess::started,[&](){
        pro->write(QString("volume %1 1\n").arg(volume_bar->value()).toUtf8());
    });


    //进度条更新
    update_play_bar.start(1000);
    connect(&update_play_bar,&QTimer::timeout,[&](){
        play_bar->setMinimum(0);
        play_bar->setMaximum(timetotal);
        play_bar->setValue(timecurrent);
        QString tmp = QString("%01:%02/%03:%04").arg(timecurrent/60).arg(timecurrent%60).arg(timetotal/60).arg(timetotal%60);
        timelength->setText(tmp);
    });


}

void MainWindow::video_list_init()
{
    QStringList tmp = QFileDialog::getOpenFileNames(
                this,
                "Select one or more files to open",
                "/home",
                "Videos (*.avi)");

    //QStringList去除重复
    bool r = true;
    auto it = tmp.begin();
    for (;it!=tmp.end() ;it++ ){
        r = true;
        for (auto ita = video_list.begin();ita!=video_list.end();ita++)
        {
            if(*it == *ita)
            {
                r = false;
            }
        }
        if(r == true)
        {
            video_list.push_back(*it);
        }
    }

    for (it = video_list.begin();it!=video_list.end() ;it++ ) {
        QString filename = QFileInfo(*it).fileName();
        r = true;
        for(int i =0;i<video_list_dispaly->count();i++)
        {
            if(filename == video_list_dispaly->item(i)->text())
            {
                r = false;
            }
        }
        if(r == true)
        {
            video_list_dispaly->addItem(QFileInfo(*it).fileName());
        }

    }
    video_list_dispaly->setCurrentRow(0);

}

void MainWindow::read()
{
    QString tmp = pro->readAll();
    //qDebug() << tmp;
    //总时间ANS_LENGTH get_time_length
    QString timelength = tmp;
    if(tmp.indexOf("ANS_LENGTH=") != -1)
    {
        timelength = timelength.mid(timelength.indexOf("ANS_LENGTH=")+11);
        //qDebug() << timelength;
        timelength = timelength.left(timelength.indexOf('.'));
        //qDebug() << timelength;
        timetotal = timelength.toInt();
        //qDebug() << timetotal;
    }
    //当前播放时间ANS_TIME_POSITION get_time_pos
    QString timepos = tmp;
    if(tmp.indexOf("ANS_TIME_POSITION=") != -1)
    {
        timepos = timepos.mid(timepos.indexOf("ANS_TIME_POSITION=")+18);
        //qDebug() << timepos;
        timepos = timepos.left(timepos.indexOf('.'));
        //qDebug() << timepos;
        timecurrent = timepos.toInt();
        //qDebug() << timecurrent;
    }
}

void MainWindow::play_button_action()
{
    if(pro->state() == QProcess::Running)//
    {
        if(play_state == true)
        {
            send_cmd.stop();
            pro->write("pause \n");
            play_state = false;
            play_button->setStyleSheet("border-image: url(:/pic/pause.png);");
            return;
        }
        if(play_state == false)
        {
            send_cmd.start(1000);
            update_play_bar.start(1000);
            pro->write("pause \n");
            play_state = true;
            play_button->setStyleSheet("border-image: url(:/pic/play.png);");
            return;
        }

    }
    else
    {
        if(video_list_dispaly->count() > 0 && video_list_dispaly->currentRow() >= 0)
        {
            pro->close();
            play_bar->setValue(0);
            QString cmd = QString("mplayer -ac mad -geometry 0:0 -zoom -x 700 -y 420 -slave -quiet %1")
                           .arg(video_list[video_list_dispaly->currentRow()]);

            pro->start(cmd);
            pro->write("volume 10 1\n");
            play_state = true;
            play_button->setStyleSheet("border-image: url(:/pic/play.png);");
            pro->write("get_time_length\n");
            send_cmd.start(1000);
        }

    }
}



