#include "declaration_dialog.h"
#include "./class/common_interface.h"
#include "LtyCommonStyle.h"
#include "class/common/scrollcontroller.h"

#include <QDebug>
#include <QTextCodec>
#include <QFile>
#include <QScrollArea>

#include "common_data.h"


#define MESSAGEBOX_BUTTON_WIDTH   160
#define MESSAGEBOX_BUTTON_HEIGHT   65

CDeclarationDialog::CDeclarationDialog(QWidget *parent) :
    QDialog(parent)
{
    qDebug() << "CDeclarationDialog::CDeclarationDialog(QWidget *parent) :>>>>>>>>>>>";
    this->setFixedSize(1024,768);
    this->setObjectName("CDeclarationDialog");
    this->setStyleSheet("QDialog#CDeclarationDialog{background-color:#000000;}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(570, 410);
    m_widget_bg->move(35, 38);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img_p2/res/start/content_background_dk.png);}");

    m_DecLable = new QLabel(this);
    QString showText = tr("免责声明");
    QString str_sheet1 = "QLabel{border:0px solid #ffffff;font:30px;color:#94c6ff;}";      ////////boder temp
    CCommonInterface::init_label_text(m_DecLable,262, 67, 150,50, showText, str_sheet1); ////hight, width temp
    m_DecLable->setAlignment(Qt::AlignCenter);


     m_DecText = new QTextEdit(this);
     //67为右边距, 82为底部边距, 24为上边距
     m_DecText->setFixedSize(470 + 67, 240 + 82+24);
     m_DecText->move(73, 114-24);
    //m_DecText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
     m_DecText->setStyleSheet("QTextEdit{font:19px; border:0px solid #ffffff;background:transparent;color:#8ea2c0;padding-bottom:82px;padding-right:67px;padding-top:24px;}");/////two words? temp
     m_DecLable->setAlignment(Qt::AlignLeft);
     m_DecText->setReadOnly(true);
     m_DecText->setContextMenuPolicy(Qt::NoContextMenu);

     QFile fp(":/img_p2/res/p2/txt/declaration.txt");
     if(fp.open(QFile::ReadOnly))
     {
        qDebug()<< "if(fp.open(QFile::ReadOnly))\n";
        QTextStream toText(&fp);
        //设置文件流编码方式
        toText.setCodec("GBK");
        m_DecText->setPlainText(toText.readAll());
        fp.close();
     }

    m_ScrollController = new ScrollController(this);
    m_ScrollController->addMonitorWidget((QScrollArea *)(m_DecText));
    m_ScrollController->setVerticalScrollBarStyleSheet(CCommonData::getInstance()->get_scroll_sheet(),0,50,50);
    m_ScrollController->set_scroll_right_length(20);

     m_DecButton = new QPushButton (this);
     // QString str_sheet2 = "QPushButton {border:0px;background:transparent;border-image:url(:/Img_p2/res/start/button_confirm_default.png); }\QPushButton:hover:pressed {border-image:url(:/Img_p2/res/start/button_confirm_default.png);}\QPushButton:hover:!pressed {border-image:url(:/Img_p2/res/start/button_confirm_selected.png);}";
     CCommonInterface::init_new_button(m_DecButton,259, 384, 120, 45, LTY_PUSHBUTTON_CONFIRM);
     m_DecButton->setText(tr("同意"));
     connect(m_DecButton, SIGNAL(clicked()), this, SIGNAL(decButtonClicked()));

     // 获取序列编号
     if(NULL !=CProtocol::getInstance())
     {
        qDebug() << "CDeclarationDialog::CDeclarationDialog(QWidget *parent) :if(NULL !=CProtocol::getInstance())\n";
         CProtocol::getInstance()->call_json_about_machine_set(1);
     }
}
