#include "common_interface.h"

#include <QFont>
#include <QFontMetrics>
#include <QDir>

#include "LtyCommonDefine.h"

CCommonInterface::CCommonInterface(QObject *parent) :
    QObject(parent)
{
}

int CCommonInterface::get_string_length(const QString _strOriginal,int _fontSize,bool _bold)
{
    QFont nameFont(tr("msyh"));
    nameFont.setPixelSize(_fontSize);
    QFontMetrics fm(nameFont);

    nameFont.setBold(_bold);

    return fm.width(_strOriginal);
}

bool CCommonInterface::get_string_limit(const QString _strOriginal,int strMaxPixel,QString &_strOutput,int _fontSize,bool _bold)
{
    bool retVal = false;
    do
    {
        QFont nameFont("msyh");
        nameFont.setPixelSize(_fontSize);
        QFontMetrics fm(nameFont);
        nameFont.setBold(_bold);

        int iTextRealPixel = fm.width(_strOriginal);

        if (iTextRealPixel > strMaxPixel)
        {
            for (int index = 0;index < _strOriginal.length();index++)
            {
                if (fm.width(_strOriginal.left(index + 1)) > strMaxPixel)
                {
                    _strOutput = _strOriginal.left(index);
                    retVal = true;
                    break;
                }
            }
        }
        else
        {
            _strOutput = _strOriginal;
        }

    } while (0);

    return retVal;
}
bool CCommonInterface::get_string_limitEx(const QString _strOriginal,int strMaxPixel,QString &_strOutput,int _fontSize,bool _bold,QString _str_dot)
{
    bool retVal = false;
    do
    {
        QFont nameFont("msyh");
        nameFont.setPixelSize(_fontSize);
        QFontMetrics fm(nameFont);
        nameFont.setBold(_bold);

        int iTextRealPixel = fm.width(_strOriginal);

        if (iTextRealPixel > strMaxPixel)
        {
            for (int index = 0;index < _strOriginal.length();index++)
            {
                if (fm.width(_strOriginal.left(index + 1) + _str_dot) > strMaxPixel)
                {
                    _strOutput = _strOriginal.left(index) + _str_dot;
                    retVal = true;
                    break;
                }
            }
        }
        else
        {
            _strOutput = _strOriginal;
        }

    } while (0);

    return retVal;
}
void CCommonInterface::init_new_button(QPushButton *_button,int _x,int _y,int _w,int _h,QString _typeSheet)
{
    _button->setFixedSize(_w,_h);
    _button->move(_x,_y);
    if(!_typeSheet.isEmpty())
        _button->setStyleSheet(_typeSheet);

    _button->setFocusPolicy(Qt::NoFocus);
    _button->setFlat(true);
}

void CCommonInterface::modify_button_sheet(QPushButton *_button,QString _typeSheet)
{
    _button->setStyleSheet(_typeSheet);
}

int CCommonInterface::init_label_text(QLabel *_label,int _x,int _y,int _w,int _h, QString _text,QString _sheet,Qt::Alignment _align,int fontSize)
{
    int str_length = 0;
    if(_w > 0)
        str_length = _w;
    else
        str_length = CCommonInterface::get_string_length(_text,fontSize);

    _label->setFixedSize(str_length,_h);
    _label->move(_x,_y);
    if(_sheet.isEmpty())
        _label->setStyleSheet(QString("QLabel{border:0px solid #8ea2c0;background:transparent;font:%1px;color:#8ea2c0;}").arg(fontSize));
    else
        _label->setStyleSheet(_sheet);

    if(0 == _w)
        _label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    else
        _label->setAlignment(_align);

    _label->setText(_text);

    return str_length;
}
#include <QIntValidator>
void CCommonInterface::set_lineedit_sheet(QLineEdit * _edit,bool _use_number,int _number1,int _number2)
{//font:16px;//color:#333333;
    _edit->setStyleSheet("QLineEdit {font:24px;color:#94c6ff;text-align:center;border:1px solid #275ba7;background:transparent; }");
    _edit->setAlignment(Qt::AlignCenter);
    _edit->setContextMenuPolicy (Qt::NoContextMenu);
    if(_use_number)
        _edit->setValidator(new QIntValidator(_number1, _number2, _edit->parent()));
}

void CCommonInterface::set_lineedit_double(QLineEdit * _edit)
{
    QRegExp rx("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,4})?$|(^\\t?$)");
    QRegExpValidator *pReg = new QRegExpValidator(rx, _edit->parent());

    _edit->setValidator(pReg);
}

#include "LtyCommonDefine.h"

#ifdef CURRENT_ARM_RUN
//#include "./include/syszuxim.h"
//#include "./include/syszuxpinyin.h"
#include "./include/myinputpanelcontext.h"
#endif

void *CCommonInterface::get_method_widget()
{
#ifdef CURRENT_ARM_RUN
      #if 0
      static QWSInputMethod* im = NULL;

      if(NULL == im)
      {
          im = new SyszuxIM;
          QWSServer::setCurrentInputMethod(im);
      }

    return im;
    #endif

   static MyInputPanelContext *ic = NULL;

   if(NULL == ic)
   {
        ic = new MyInputPanelContext;
   }
   return ic;

#else
    return NULL;
#endif
}

bool CCommonInterface::input_panel_control(bool _show)
{
    bool ret_value = false;
#ifdef CURRENT_ARM_RUN
    MyInputPanelContext *pInputPanel = (MyInputPanelContext *)CCommonInterface::get_method_widget();
    QWidget *pMethodWidget = NULL;

    if(pInputPanel != NULL)
    {
         pMethodWidget  = pInputPanel->current_window();

         if(NULL != pMethodWidget)
         {
            if(_show)
            {
                if(!pMethodWidget->isVisible())
                    pMethodWidget->show();
            }
            else
            {
                 if(pMethodWidget->isVisible())
                 {
                    ret_value = true;
                    pMethodWidget->hide();
                 }
            }
         }

    }
#endif
    return ret_value;
}
#include <QDateTime>
#include <QDebug>
void CCommonInterface::printf_debug(QString _debug)
{
    qDebug() << QDateTime::currentDateTime().toString("[hh:mm:ss:zzz] >>>>>>>gui_info:") << _debug;
}

#include "class/common/define_screen.h"
void CCommonInterface::text_dispersed_align(const QString &srcText,const QString &dstText,int _fontSize,QFont &_font)
{
    int length = CCommonInterface::get_string_length(srcText,CMyDefineScreen::getInstance()->get_change_factor_y(_fontSize));
    int text_lehgth = CCommonInterface::get_string_length(dstText,CMyDefineScreen::getInstance()->get_change_factor_y(_fontSize));

    _font.setFamily(tr("msyh"));
    _font.setPixelSize(_fontSize);
    qreal qreal_size = 0;
    if(text_lehgth < length)
    {

        qreal_size = (length - text_lehgth)/((qreal)(dstText.length() - 1));

        _font.setLetterSpacing(QFont::AbsoluteSpacing,qreal_size);
    }
}


QString CCommonInterface::convert_string_file_size(const qint64 size)
{
    float num = size;
    QStringList list;
    QString result = "";
    qint64 tempNum = num;
    list << " KB" << " MB" << " GB" << " TB";

    QStringListIterator i(list);
    QString unit = " 字节";
    QString strUnit = unit;
    while(num >= 999.9 && i.hasNext())
    {
        strUnit = i.next();
        num /= 1024.00;
    }
    tempNum =  num;

    //return QString().setNum(num,'f',1)+unit;
    if (unit == strUnit || ((float)tempNum == num))
    {
        result = QString().number(num) + strUnit;
    }
    else
    {
        result = QString("%1").arg(num,3,'f',2) + strUnit;
    }

    return result;
}


int CCommonInterface::read_dir_file(QString _dir,QStringList &_file_list,QString _filter_name)
{
    QDir d(_dir);
    if(!d.exists())
    {
        printf("disk not exist.\n");
        return -1;
    }

    QStringList list;
    list << _filter_name;//"*.sw";
    d.setNameFilters(list);
    d.setFilter(QDir::Files);
    d.setSorting(QDir::Time|QDir::Reversed);
    QStringList imgs=d.entryList();
    if(!imgs.empty())
    {
        for(QStringList::Iterator i=imgs.begin();i!=imgs.end();++i)
        {
            _file_list.append(_dir+"/"+*i);
        }
    }


#if 0
    list.clear();
    list<<"*";
    d.setNameFilters(list);
    d.setFilter(QDir::Dirs);
    QStringList dirs=d.entryList();
    for(QStringList::Iterator dir_it=dirs.begin();dir_it!=dirs.end();++dir_it)
    {
        if(*dir_it!="."&&*dir_it!="..")
        {
            this->readDir(dir+"/"+*dir_it);
        }
    }
#endif

    return 0;
}
#include <QSettings>
bool CCommonInterface::get_config_info(QString _name,QString &_data)
{
    bool retVal = true;

    do
    {
#ifdef CURRENT_ARM_RUN
        QSettings user_set("/p1app/t3_log.ini", QSettings::IniFormat);
#else
        QSettings user_set("ltyd2.ini", QSettings::IniFormat);
#endif
        user_set.beginGroup("config");
        _data = user_set.value(_name,"").toString();
        user_set.endGroup();

    } while (0);

    return retVal;
}
#define CONFIG_FILE_PATH    "/mnt/app/log.ini"
#define SECTION_NAME        "log"
bool CCommonInterface::notify_ui_printf()
{
    bool retVal = false;

    do
    {
        QSettings user_set(CONFIG_FILE_PATH, QSettings::IniFormat);

        user_set.beginGroup(SECTION_NAME);
        retVal = user_set.value("ui_printf",false).toBool();
        user_set.endGroup();

    } while (0);

    return retVal;
}

bool CCommonInterface::p2_is_demo_mainpage()
{
    bool retVal = false;

    do
    {
        QSettings user_set(CONFIG_FILE_PATH, QSettings::IniFormat);

        user_set.beginGroup(SECTION_NAME);
        retVal = user_set.value("p2_demo",false).toBool();
        user_set.endGroup();

    } while (0);

    return retVal;
}



