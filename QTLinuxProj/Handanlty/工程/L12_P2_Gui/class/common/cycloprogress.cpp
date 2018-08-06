#include "cycloprogress.h"
#include <QPainter>
#include <QTimerEvent>

#include <QDebug>

CycloProgress::CycloProgress(QWidget *parent) :
    QLabel(parent), minLoops(3)
{
    m_rotation = 0.0;
    timerID = 0;//UI_03  cs000
    m_pixmap = QPixmap(":/img/res/cs000.png");//QPixmap(":/img/res/test_route.png");
    //216,276 - 40


    setStyleSheet("QLabel{border:1px solid #ffffff;font:13px;color:#ffffff;}");
}

void CycloProgress::set_pixmap_size(int _width,int _height)
{
    if ((m_pixmap.height() != _height) || (m_pixmap.width() != _width))
    {
            m_pixmap = m_pixmap.scaled(_width,_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);// 缩放图片
    }
}

void CycloProgress::setMinLoops(int loops)
{
    minLoops = loops;
}

void CycloProgress::startAnimation(int interval)
{
    if(timerID == 0){
        timerID = startTimer(interval);
    }
}

void CycloProgress::stopAnimation()
{
    killTimer(timerID);
    timerID = 0;
}

void CycloProgress::timerEvent(QTimerEvent * event)
{
    if(event->timerId() == timerID){
        m_rotation += 5;
        update();

        if(m_rotation > minLoops*360){
            emit runMinLoopsEnough();
        }
    }
}

void CycloProgress::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);

    //QPixmap leadon_text("://qrc/leadon_e_char.png");

    //int width = m_pixmap.width();
    //int height = m_pixmap.height();

    QPointF center(m_pixmap.width() / qreal(2), m_pixmap.height() / qreal(2));
    //painter.drawPixmap(m_pixmap.width() / qreal(2) -leadon_text.width() / 2 , m_pixmap.height() / qreal(2), leadon_text);

    painter.translate(center);
    painter.rotate(m_rotation);
    painter.translate(-center);

    painter.drawPixmap(QPointF(0, 0), m_pixmap);
}







