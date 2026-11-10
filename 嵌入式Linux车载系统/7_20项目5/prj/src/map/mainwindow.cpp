#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


//http://api.map.baidu.com/staticimage?width=800&height=480&center=113.27,23.134&zoom=11

//移动
int m_x,m_y;
double move_x,move_y;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("MainWindow{background-image: url(:/pic/map_background.jpg);}");
    net_manager = new QNetworkAccessManager(this);//创建一个网络管理器对象

    //设置触摸事件
    setAttribute(Qt::WA_AcceptTouchEvents);

    //安装事件过滤器
    this->installEventFilter(this);

    //显示图片
    pic = new QLabel(this);
    pic->setGeometry(0,0,800,480);

    zoom = 11;
    lng = 113.27;//广州坐标
    lat = 23.134;//广州坐标
    request_map(lng,lat,zoom);
    page_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::page_init()
{


    //退出按钮
    exit_button = new QPushButton(this);//退出按钮
    exit_button->setStyleSheet("QPushButton { color: black;background: transparent;border-image:transparent}");//
    exit_button->setGeometry(700,450,150,30);
    exit_button->setText("Exit");
    exit_button->setFont(QFont("Microsoft YaHei",10,20));
    exit_button->setFocusPolicy(Qt::NoFocus);
    connect(exit_button,&QPushButton::clicked,[=](){
            net_manager->deleteResource(net_request);
            exit(0);
    });

    //放大按钮
    zoom_p = new QPushButton(this);
    zoom_p->setFocusPolicy(Qt::NoFocus);
    zoom_p->setStyleSheet("border-image: url(:/pic/zoom_p.png);");
    zoom_p->setGeometry(760,360,35,35);
    connect(zoom_p,&QPushButton::clicked,[&](){
        if(zoom < 19)
        {
            request_map(lng,lat,++zoom);
        }

    });

    //缩小按钮
    zoom_m = new QPushButton(this);
    zoom_m->setFocusPolicy(Qt::NoFocus);
    zoom_m->setStyleSheet("border-image: url(:/pic/zoom_m.png);");
    zoom_m->setGeometry(760,400,35,35);
    connect(zoom_m,&QPushButton::clicked,[&](){
        if(zoom > 1)
        {
           request_map(lng,lat,--zoom);
        }

    });
}

void MainWindow::request_map(float & lng,float & lat,int &zoom)
{
    net_manager->disconnect();
    file.close();
    file.remove();
    //disconnect(reply,&QNetworkReply::readyRead,this,&MainWindow::read_netdata);
    QString request_addr = QString("http://api.map.baidu.com/staticimage/v2?ak=EOyx5aQ2ZDtgnA90R7yvaEP8kb04WGs1&mcode=666666&width=800&height=480&markers=%1,%2&markerStyles=l&zoom=%3")
                            .arg(lng).arg(lat).arg(zoom);
    qDebug() << request_addr;
    net_request.setUrl(QUrl(request_addr));
    reply = net_manager->get(net_request);
    //关联信号
    connect(reply,&QNetworkReply::readyRead,this,&MainWindow::read_netdata);
    //关联下载完成信号
    connect(reply,&QNetworkReply::finished,this,&MainWindow::download_done);

    file.setFileName("./temp.png");
    //打开文件
    file.open(QIODevice::WriteOnly);
}

void MainWindow::read_netdata()
{
    file.write(reply->readAll());
}

void MainWindow::download_done()
{
    file.close();
    pic->setPixmap(QPixmap("./temp.png"));
}

//事件过滤器
QPoint point,last_point;//按下坐标
bool pres_flag,rele_flag;//按下松开标志
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{

    if(watched == this ){
        QTouchEvent* touch = static_cast<QTouchEvent*>(event);

        switch (event->type()) {
        case QEvent::MouseButtonPress://鼠标按下事件

            pres_flag = 1;
            last_point.setY(cursor().pos().y());     // 记录按下点的y坐标
            last_point.setX(cursor().pos().x());     // 记录按下点的x坐标
            break;
        case QEvent::MouseButtonRelease://鼠标松开事件
            rele_flag = 1;
            point.setY(cursor().pos().y());     // 记录按下点的y坐标
            point.setX(cursor().pos().x());     // 记录按下点的x坐标
            break;

        case QEvent::TouchBegin://触摸事件

            point.setY(touch->touchPoints().at(0).pos().y());     // 记录按下点的y坐标
            point.setX(touch->touchPoints().at(0).pos().x());     // 记录按下点的x坐标
            if(point.x()<580 || point.y()<350)
            {
                rele_flag = 1;
                pres_flag = 1;
            }
            event->accept();
            break;

        default:
            break;
        }
    }
    if(pres_flag==1 && rele_flag==1)
    {
        pres_flag = 0;
        rele_flag = 0;
        if(event->type() == QEvent::TouchBegin)//触摸
        {
            m_x = 400-point.x();
            m_y = 240-point.y();
        }
        else {    //鼠标
            m_x = point.x()-last_point.x();
            m_y = point.y()-last_point.y();
        }
        if(m_x>10 || m_x<-10 || m_y>10 || m_y<-10)
        {
            //计算需要偏移多少东经北纬
            move_x = m_x*0.0000003*(19-zoom)*(19-zoom)*(19-zoom)*(19-zoom);
            move_y = m_y*0.0000003*(19-zoom)*(19-zoom)*(19-zoom)*(19-zoom);
            lng -= move_x;
            lat += move_y;

            request_map(lng,lat,zoom);//发送图片请求
        }

    }

    return QWidget::eventFilter(watched,event);//将事件传递给父类
}

