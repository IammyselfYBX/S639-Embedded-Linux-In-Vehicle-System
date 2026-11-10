#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QLabel>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDateTime>



//IP定位的API
/*var lo="广东省", lc="广州市"; var localAddress={city:"广州市", province:"广东省"}*/
#define   IP_QUERY_API_URL    "http://ip.ws.126.net/ipquery"
#define   WEATHER_GET_API_URL  "http://wthrcdn.etouch.cn/weather_mini?city="
//#define   WEATHER_GET_API_URL "http://v2.alapi.cn/api/weather/forecast?token=MoxBwFHqrHnHUDb1%20&location="
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
    void weatherPage_init();//初始化背景
    void weather_request();//开始获取天气
    void update_Page();//刷新页面



private:
    QNetworkAccessManager * net_manager;//网络请求管理器
    QNetworkRequest net_request;
    QPushButton * exit_button;//退出
    QPushButton * button_city;//显示城市按钮
    QLabel * Temp;//显示温度
    QLabel * timeshow;//显示系统时间
    QLabel * today_weather_icon;//今天天气
    QLabel * today_weather_text;//今天天气
    QLabel * date[6];//日期
    QLabel * minMaxTempLabelArr[6];//显示最高最低气温
    QLabel * weather_icon[6];//显示天气图标
    QLabel * weather_text[6];//显示天气文本

    QTimer * datetime_timer;//显示时间定时器
    QString city;
    QJsonObject yesterday_obj;
    QString forecast_city;
    QJsonArray forecast_arry;
    QString wendu;

public slots:
    void city_get(QNetworkReply *reply);//获取城市位置
    void request_data(QNetworkReply *reply);//获取天气数据
};





#endif // MAINWINDOW_H
