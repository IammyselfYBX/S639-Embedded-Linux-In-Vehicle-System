#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>
#include <QTimer>
#include <QListWidget>
#include <QStringList>
#include <QFileDialog>
#include <QProcess>
#include <unistd.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void videopage_init();
    void video_list_init();

private:
    Ui::MainWindow *ui;
    QProgressBar * bar;//进度条
    QSlider * play_bar;//播放进度条
    QSlider * volume_bar;//音量控制条
    QPushButton * play_button;//播放按钮
    QPushButton * slow_button;//快退按钮
    QPushButton * fast_button;//快进按钮
    QPushButton * prev_button;//上一曲
    QPushButton * next_button;//下一曲
    QPushButton * volume_button;//音量
    QPushButton * open_file_button;//打开文件对话框
    QLabel * timelength;//播放时间/总时间
    QListWidget * video_list_dispaly;//视频列表显示
    QStringList video_list; //视频文件表
    QPushButton * exit_button;//退出

private:
    int timetotal;
    int timecurrent;
    bool play_state;

    //定时器
    QTimer volume_timer;//音量条自动消失
    QTimer send_cmd;//1S发送一次获取当前时间的命令
    QTimer update_play_bar;//1S 更新一次进度条

    //进程
    QProcess * pro;
public slots:
    void read();
    void play_button_action();

};

//歌曲播放进度条样式
#define MEDIASLIDER_STYLE                       "QSlider{\
                                                border-color: #bcbcbc;\
                                                }\
                                                QSlider::groove:horizontal {\
                                                border: 1px solid #999999;\
                                                height: 5px;\
                                                margin: 0px 0; \
                                                left: 5px; right: 5px;\
                                                }\
                                                QSlider::handle:horizontal {\
                                                border: 0px ;\
                                                border-image:  url(:icon/Resource/icon/handle.png);\
                                                width: 20px; \
                                                margin: -10px -7px -10px -7px; \
                                                } \
                                                QSlider::add-page:horizontal{\
                                                background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #bcbcbc, stop:0.25 #bcbcbc, stop:0.5 #bcbcbc, stop:1 #bcbcbc); \
                                                }\
                                                QSlider::sub-page:horizontal{ \
                                                background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #439cf3, stop:0.25 #439cf3, stop:0.5 #439cf3, stop:1 #439cf3); \
                                                }"
//音量条样式
#define VOLUMESLIDER_STYLE                 "QSlider{\
                                                border-color: #cbcbcb;\
                                            }\
                                            QSlider::groove:vertical {\
                                            background: #cbcbcb;\
                                            width: 6px;\
                                            border-radius: 1px;\
                                            padding-left:-1px;\
                                            padding-right:-1px;\
                                            padding-top:-1px;\
                                            padding-bottom:-1px;\
                                            } \
                                            QSlider::sub-page:vertical {\
                                            background: #cbcbcb;\
                                            border-radius: 2px;\
                                            } \
                                            QSlider::add-page:vertical {\
                                            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
                                                stop:0 #439cf4, stop:1 #439cf4);\
                                            background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1, \
                                                stop: 0 #439cf4, stop: 1 #439cf4);\
                                            width: 10px;\
                                            border-radius: 2px; \
                                            }\
                                            QSlider::handle:vertical\
                                            {   \
                                                border-image: url(:icon/Resource/icon/handle.png);\
                                                 margin: -2px -15px -2px -15px;\
                                                height: 35px;\
                                            }"
#endif // MAINWINDOW_H
