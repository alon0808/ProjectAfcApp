#ifndef COMMON_INTERFACE_H
#define COMMON_INTERFACE_H

#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFont>
#include "LtyCommonDefine.h"

class CCommonInterface : public QObject
{
    Q_OBJECT
public:
    explicit CCommonInterface(QObject *parent = 0);
    
signals:
    
public slots:

public:
    static int get_string_length(const QString _strOriginal,int _fontSize,bool _bold = false);
    static bool get_string_limit(const QString _strOriginal,int strMaxPixel,QString &_strOutput,int _fontSize,bool _bold = false);
    static bool get_string_limitEx(const QString _strOriginal,int strMaxPixel,QString &_strOutput,int _fontSize,bool _bold = false,QString _str_dot = "..");
    static void init_new_button(QPushButton *_button,int _x,int _y,int _w,int _h,QString _typeSheet = "");
    static void modify_button_sheet(QPushButton *_button,QString _typeSheet);
    static int init_label_text(QLabel *_label,int _x,int _y,int _w,int _h, QString _text,QString _sheet = "",Qt::Alignment _align = Qt::AlignCenter,int fontSize = TEXT_FONT);
    static void set_lineedit_sheet(QLineEdit * _edit,bool _use_number = false,int _number1 = 0,int _number2 = 999999);
    static void set_lineedit_double(QLineEdit * _edit);
    static void *get_method_widget();
    static bool input_panel_control(bool _show);
    static void printf_debug(QString _debug);
    static void text_dispersed_align(const QString &srcText,const QString &dstText,int _fontSize,QFont &_font);
    static QString convert_string_file_size(const qint64 size);
    static int read_dir_file(QString _dir,QStringList &_file_list,QString _filter_name);
    static bool get_config_info(QString _name,QString &_data);
    static bool notify_ui_printf();
    static bool p2_is_demo_mainpage();

};

#endif // COMMON_INTERFACE_H
