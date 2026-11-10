#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QFile>
#include<QEvent>
#include<QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager * net_manager;//网络连接管理器
    QNetworkRequest net_request;//网络请求
    QNetworkReply * reply;
    QLabel * pic; //显示图片
    QPushButton * exit_button;//退出按钮
    QPushButton * zoom_p;//放大按钮
    QPushButton * zoom_m;//缩小按钮
    QFile file;

    QString city;
    int zoom;
    float lng;//经度 longitude
    float lat;//纬度 latitude
private:
    void page_init();//页面初始化
    void request_map(float & lng,float & lat,int &zoom);
    bool eventFilter(QObject *watched, QEvent *event);
public slots:
    void read_netdata();
    void download_done();
};
#endif // MAINWINDOW_H
