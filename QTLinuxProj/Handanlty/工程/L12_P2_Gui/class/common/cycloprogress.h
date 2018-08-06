
#ifndef CYCLOPROGRESS_H
#define CYCLOPROGRESS_H

#include <QLabel>
#include <QPixmap>

class CycloProgress : public QLabel
{
    Q_OBJECT
public:
    explicit CycloProgress(QWidget *parent = 0);
    void setMinLoops(int loops = 3);
    /**
     * @brief startAnimation start the Animation
     * @param interval   unit is  milliseconds
     */
    void startAnimation(int interval = 25);
    void stopAnimation();
    void set_pixmap_size(int _width,int _height);
signals:
    void runMinLoopsEnough();
protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *event);
private:
    int timerID;
    int minLoops;
    QPixmap m_pixmap;
    qreal m_rotation;
};

#endif // CYCLOPROGRESS_H

