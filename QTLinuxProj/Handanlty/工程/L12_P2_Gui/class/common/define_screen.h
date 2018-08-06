#ifndef DEFINE_SCREEN_H
#define DEFINE_SCREEN_H

#include <QWidget>
#include <QMap>

class CAutoResizeOriginalData
{
public:
    QRect data_rect;
    QFont data_font;
    QString data_sheet;
};

class CMyDefineScreen : public QWidget
{
    Q_OBJECT
public:
    explicit CMyDefineScreen(QWidget *parent = 0);
    static CMyDefineScreen *getInstance();
signals:
    
public slots:
    
public:
    void add_parent(QWidget *_parent,bool _add = true);
    void reset_widget();

    int get_change_factor_x(int _x);
    int get_change_factor_y(int _y);
    void ignore_widget(QWidget *_widget);
    //void update_widget_data();
    void debug_mode_start();
    void debug_mode_close();
private:
    double get_factor_x();
    double get_factor_y();
    void add_widget(QWidget *widget);
    void reset_size(QWidget *widget,const QRect &_rect,const QFont &_font,const QString &_sheet);
    bool is_ignore(QWidget *_widget);
    QString get_font_size(const QString &_font_text);
private:
    double m_factor_x;
    double m_factor_y;

    QMap<QWidget*,CAutoResizeOriginalData> m_resize_map;

    QList<QWidget *> m_ignore_list;

    static CMyDefineScreen *m_single;
    bool m_start_debug;
};

#endif // DEFINE_SCREEN_H
