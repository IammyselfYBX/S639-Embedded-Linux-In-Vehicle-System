#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QProcess>


#define GET_TIME_ONLINE "http://api.k780.com/?app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void datetime_update();//获取网络时间
    void menupage_init();//主菜单页面初始化

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager * net_manager;//网络请求管理器

private:
    QTimer timeupdate_timer;
    QTimer datetime_show;
    QLabel * timeshow;//显示时间
    QLabel * dateshow;//显示日期
    QPushButton * weather;//天气功能
    QPushButton * music;//音乐功能
    QPushButton * video;//视频播放功能
    QPushButton * map;//地图功能
    QTimer * pro_timer;//进程管理定时器
    QProcess * pro;//创建进程

};
#endif // MAINWINDOW_H
