#ifndef DYNAMIC_TEXT_H
#define DYNAMIC_TEXT_H

#include <QWidget>
#include <QTimer>

class  CDynTextParam
{
public:
    CDynTextParam()
    {
        m_control_this = NULL;
        m_control_type = 0;
        m_control_id   = 0;
        m_control_show_text = "";
        m_control_font_size = 0;
        m_control_show_length = 0;
        m_action = 0;
    }
    void    *m_control_this;        //控件指针
    char    m_control_type;         //控件类型
    char    m_control_id;           //控件ID
    QString m_control_source_text;  //控件源文本
    QString m_control_show_text;    //控件显示文本
    int     m_control_font_size;    //文本字体大小
    int     m_control_show_length;  //文本显示长度
    char    m_action;               //动作
};


class CDynamicText : public QWidget
{
    Q_OBJECT
public:
    explicit CDynamicText(QWidget *parent = 0);
    ~CDynamicText();
    enum
    {
      kEnumAlignmentLeft = 1, //文字左对齐
      kEnumAlignmentRight,    //文字居中对齐
      kEnumUpdateText         //更新文本显示
    };
    enum
    {
      kEnumPushbutton,  //按钮类型
      kEnumLabel        //标签类型
    };
 public:
    void set_text(CDynTextParam &_param);
signals:
    void signal_dynamic_text_event(const CDynTextParam &_param);
public slots:
    void slot_text_roll_timer();
private:
    CDynTextParam m_CDynTextParam;

    QTimer *m_timer_text_roll;//文字滚动
    int m_text_pos;

    
};

#endif // DYNAMIC_TEXT_H
