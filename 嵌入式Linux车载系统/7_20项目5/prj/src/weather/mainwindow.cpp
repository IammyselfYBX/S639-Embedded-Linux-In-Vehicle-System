#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    net_manager = new QNetworkAccessManager(this);//创建网络管理对象
    net_request = QNetworkRequest(QUrl(IP_QUERY_API_URL));
    exit_button = new QPushButton(this);//退出按钮
    exit_button->setStyleSheet("QPushButton { color: white;background: transparent;border-image:transparent}");//
    exit_button->setGeometry(700,430,150,50);
    exit_button->setText("Exit");
    exit_button->setFont(QFont("Microsoft YaHei",10,20));
    exit_button->setFocusPolicy(Qt::NoFocus);
    connect(exit_button,&QPushButton::clicked,[=](){
        net_manager->deleteResource(net_request);
        exit(0);
    });


    //更新时间
    timeshow = new QLabel(this);//显示当前时间
    timeshow->setGeometry(500,20,250,100);
    timeshow->setStyleSheet("QLabel { color: white}");
    timeshow->setFont(QFont("Microsoft YaHei",20,20));
    timeshow->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    datetime_timer = new QTimer();
    datetime_timer->start(1000);
    connect(datetime_timer,&QTimer::timeout,[=](){
        QDateTime current_time = QDateTime::currentDateTime();
        timeshow->setText(current_time.toString("yyyy-MM-dd \n hh:mm:ss ddd"));
    });


    weatherPage_init();//初始化背景


}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化背景
void MainWindow::weatherPage_init()
{
    this->setStyleSheet("MainWindow{background-image: url(:/pic/weather_background.jpg);}");//设置背景图片qrc:/pic/weather_background.jpg

    //显示城市按钮
    button_city = new QPushButton(this);
    QFont font_city("Microsoft YaHei",26,30);
    font_city.setBold(true);
    button_city->setFont(font_city);
    button_city->setText("正在定位...");
    button_city->setGeometry(150,20,250,60);
    button_city->setStyleSheet("QPushButton { color: white;background: transparent;border-image:transparent}");
    button_city->setFocusPolicy(Qt::NoFocus);
    connect(button_city,&QPushButton::clicked,this,&MainWindow::weather_request);

    //城市下方显示温度
    Temp = new QLabel(this);
    QFont font_Temp("Microsoft YaHei",40,30);
    font_Temp.setBold(true);
    Temp->setFont(font_Temp);
    Temp->setText("--℃");
    Temp->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    Temp->setGeometry(150,60,200,100);
    Temp->setStyleSheet("QLabel { color: white}");//;background: transparent;border-image:transparent

    //今天天气图标
    today_weather_icon = new QLabel(this);
    today_weather_icon->setGeometry(80,40,120,60);
    today_weather_text = new QLabel(this);
    QFont today_w("Microsoft YaHei",20,30);
    today_w.setBold(true);
    today_weather_text->setFont(today_w);
    today_weather_text->setGeometry(80,100,120,30);
    today_weather_text->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    today_weather_text->setStyleSheet("QLabel { color: white}");

    //昨天到未来5天内天气显示
    //定义 一组字体
    QFont font_weather_6days("Microsoft YaHei",10,30);
    font_weather_6days.setBold(true);
    for(int i =0;i<6;i++)
    {
        //
        //显示当前日期
        date[i] = new QLabel(this);
        date[i]->setFont(font_weather_6days);
        date[i]->setGeometry(i*120+45,180,120,60);
        date[i]->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        date[i]->setStyleSheet("QLabel { color: white}");
        date[i]->setText("星期日\n7月18日");
        //显示最高最低气温
        minMaxTempLabelArr[i] = new QLabel(this);
        minMaxTempLabelArr[i]->setFont(font_weather_6days);
        minMaxTempLabelArr[i]->setGeometry(i*120+45,220,120,60);
        minMaxTempLabelArr[i]->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        minMaxTempLabelArr[i]->setStyleSheet("QLabel { color: white}");
        minMaxTempLabelArr[i]->setText("--℃~--℃");
        //显示天气图标
        weather_icon[i] = new QLabel(this);
        weather_icon[i]->setGeometry(i*120+45,280,120,60);

        //显示天气文本
        weather_text[i] = new QLabel(this);
        weather_text[i]->setFont(font_weather_6days);
        weather_text[i]->setGeometry(i*120+45,360,120,60);
        weather_text[i]->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        weather_text[i]->setStyleSheet("QLabel { color: white}");
        weather_text[i]->setText("");
    }


    //获取城市
    weather_request();


}


//获取天气信息
void MainWindow::weather_request()
{
    button_city->setText("正在定位...");
    Temp->setText("--℃");
    today_weather_icon->clear();
    today_weather_text->clear();

    //清除信息
    for(int i=0; i<6;i++)
    {
        date[i]->clear();
        minMaxTempLabelArr[i]->setText("--℃~--℃");
        weather_icon[i]->clear();
        weather_text[i]->clear();
    }

    //获取城市 关联
    connect(net_manager,&QNetworkAccessManager::finished,this,&MainWindow::city_get);
    net_manager->get(net_request);

}

void MainWindow::update_Page()
{
    button_city->setText(forecast_city);//单当前城市
    Temp->setText(wendu+QString("℃"));//温度

    //今天天气
    QString pic_path;
    pic_path = QString(":/pic/%1.png").arg(forecast_arry.at(1).toObject().value("type").toString());
    today_weather_icon->setPixmap(QPixmap(pic_path).scaled(today_weather_icon->width(),today_weather_icon->height()));//当天天气状况
    today_weather_text->setText(forecast_arry.at(1).toObject().value("type").toString());

    //昨天到未来5天内天气数据更新
    for(int i=0;i<6;i++)
    {
        //日期
        QString date_tmp = forecast_arry.at(i).toObject().value("date").toString();
        date_tmp = date_tmp.insert(date_tmp.indexOf("日")+1,'\n');
        if(i == 1)
        {
            date_tmp = date_tmp + "(今天)";
            date[i]->setStyleSheet("QLabel { color: yellow}");
        }
        date[i]->setText(date_tmp);

        //最高最低气温
        QString Temp_tmp = forecast_arry.at(i).toObject().value("low").toString().mid(3) + "~" +
                           forecast_arry.at(i).toObject().value("high").toString().mid(3);
        minMaxTempLabelArr[i]->setText(Temp_tmp);

        //天气图片
        pic_path = QString(":/pic/%1.png").arg(forecast_arry.at(i).toObject().value("type").toString());
        weather_icon[i]->setPixmap(QPixmap(pic_path).scaled(today_weather_icon->width(),today_weather_icon->height()));//当天天气状况
        weather_text[i]->setText(forecast_arry.at(i).toObject().value("type").toString());
    }

}

void MainWindow::request_data(QNetworkReply *reply)
{

    disconnect(net_manager,&QNetworkAccessManager::finished,this,&MainWindow::request_data);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString temp = codec->toUnicode(reply->readAll());
    //qDebug() << temp;

    //解析JSON数据
    QJsonDocument JSON = QJsonDocument::fromJson(temp.toUtf8());
    QJsonObject obj ;
    if(JSON.isObject())
    {
        obj = JSON.object();
    }
    QJsonObject data = obj.value("data").toObject();
    yesterday_obj = data.value("yesterday").toObject();
    forecast_city = data.value("city").toString();
    forecast_arry = data.value("forecast").toArray();
    wendu = data.value("wendu").toString();

    //把昨天的天气信息插入到forecast_arry
    forecast_arry.insert(0,yesterday_obj);

    update_Page();

}

void MainWindow::city_get(QNetworkReply *reply)
{
    disconnect(net_manager,&QNetworkAccessManager::finished,this,&MainWindow::city_get);
    QTextCodec *codec = QTextCodec::codecForName("gbk");
    QString temp = codec->toUnicode(reply->readAll());

    //"var lo=\"广东省\", lc=\"广州市\";\r\nvar localAddress={city:\"广州市\", province:\"广东省\"}\r\n"
    //提取关键字
    temp = temp.mid(temp.indexOf("city:\"")+6);
    city = temp.left(temp.indexOf('"'));
    reply->deleteLater();
    if(city == "") //如果没有获取到城市,说明IP定位失败，默认城市 北京
    {
        city = "北京";
    }

    //请求天气数据
    //拼接天气请求URL
    QString request_url = WEATHER_GET_API_URL + city;
    connect(net_manager,&QNetworkAccessManager::finished,this,&MainWindow::request_data);
    net_manager->get(QNetworkRequest(QUrl(request_url)));

}



