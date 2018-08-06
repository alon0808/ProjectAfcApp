#ifndef DEFINE_SCREEN_H
#define DEFINE_SCREEN_H

#include <QWidget>
#include <QMap>

struct AutoResizeOriginalData
{
    QRect data_rect;
    QFont data_font;
    QString data_sheet;
};

class CDefineScreen : public QWidget
{
    Q_OBJECT
public:
    explicit CDefineScreen(QWidget *parent = 0);
    
signals:
    
public slots:
    
public:
    void add_parent(QWidget *_parent,bool _add = true);
    void reset_widget();

    int get_change_factor_x(int _x);
    int get_change_factor_y(int _y);
    void ignore_widget(QWidget *_widget);
    //void update_widget_data();
private:
    double get_factor_x();
    double get_factor_y();
    void add_widget(QWidget *widget);
    void reset_size(QWidget *widget,const QRect &_rect,const QFont &_font,const QString &_sheet);
    bool is_ignore(QWidget *_widget);
private:
    double m_factor_x;
    double m_factor_y;

    QMap<QWidget*,AutoResizeOriginalData> m_resize_map;

    QList<QWidget *> m_ignore_list;

};

#endif // DEFINE_SCREEN_H
