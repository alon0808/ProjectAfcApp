#include "voice_dialog.h"

#include "./class/common_interface.h"
#include "class/ltyuicallinterface.h"

#include "class/cJSON.h"
#include "main_page.h"

#include "common_data.h"

#define SLIDER_CSS  "\
QSlider::groove:horizontal {\
background: #0e68a1;\
height: 2px;\
border-radius: 1px;\
}\
\
QSlider::sub-page:horizontal {\
background-color:#80e3ff;\
border: 1px solid #80e3ff;\
margin-top: -1px;\
margin-bottom: -1px;\
border-radius: 2px;\
}\
\
QSlider::add-page:horizontal {\
background: #0e68a1;\
height: 2px;\
border-radius: 1px;\
}\
\
QSlider::handle:horizontal \
{\
    border-image:url(:/img/res/main_page/popup_dialog/volume.png);\
    width: 62px;\
    margin-top: -40px;\
    margin-bottom: -40px;\
    margin-left: -19px;\
    margin-right: -19px;\
}\
\
QSlider::handle:horizontal:hover {\
    border-image:url(:/img/res/main_page/popup_dialog/volume.png);\
    width: 62px;\
    margin-top: -40px;\
    margin-bottom: -40px;\
    margin-left: -19px;\
    margin-right: -19px;\
}"



CVoiceDialog *CVoiceDialog::m_single = NULL;

CVoiceDialog *CVoiceDialog::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CVoiceDialog(CMainPage::getInstance());
    }
    return m_single;
}
bool CVoiceDialog::getIsEmpty()
{
     if(NULL == m_single)
     {
        return true;
     }
     return false;
}

CVoiceDialog::CVoiceDialog(QWidget *parent) :
    CParentWidget(parent)
{
    this->setFixedSize(723,551);
    this->setObjectName("CVoiceDialog");
    this->setStyleSheet("QDialog#CVoiceDialog{background:transparent;}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);
    //this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    m_widget_background = new QWidget(this);
    m_widget_background->setFixedSize(712,542);
    m_widget_background->move((723 - 712)/2,(551 - 542)/2);
    m_widget_background->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/popup_dialog/popup_bg.png);}");

    QString str_sheet = "";
    m_pushbutton_close = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/popup_dialog/close.png");
    CCommonInterface ::init_new_button(m_pushbutton_close,this->width() - 62,0,62,66,str_sheet);
    connect(m_pushbutton_close,SIGNAL(clicked()),SLOT(slot_pushbutton_close()));

    m_widget_voice_background1 = new QWidget(this);
    m_widget_voice_background1->setFixedSize(580,126);
    m_widget_voice_background1->move(60,80);
    m_widget_voice_background1->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/popup_dialog/volume_bg.png);border-radius:8px;}");

    m_widget_voice_background2 = new QWidget(this);
    m_widget_voice_background2->setFixedSize(580,126);
    m_widget_voice_background2->move(60,80 + (126 + 10) * 1);
    m_widget_voice_background2->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/popup_dialog/volume_bg.png);;border-radius:8px;}");

    m_widget_voice_background3 = new QWidget(this);
    m_widget_voice_background3->setFixedSize(580,126);
    m_widget_voice_background3->move(60,80 + (126 + 10) * 2);
    m_widget_voice_background3->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/popup_dialog/volume_bg.png);;border-radius:8px;}");


    QString align_text = tr("TTS音:");
    QString align_text1 = tr("内音:");
    QString align_text2 = tr("外音:");
    QFont nameFont1;
    QFont nameFont2;
    QFont nameFont3;

    m_label_voice_icon1 = new QLabel(this);
    m_label_voice_icon1->setFixedSize(100,31);
    m_label_voice_icon1->move(60 + 40,80 + (126 - 31)/2);
    m_label_voice_icon1->setStyleSheet("QLabel{border:0px;font:31px;color:#80e3ff;}");
    m_label_voice_icon1->setText(align_text1);
    CCommonInterface::text_dispersed_align(align_text,align_text1,31,nameFont1);
    m_label_voice_icon1->setFont(nameFont1);

    m_label_voice_icon2 = new QLabel(this);
    m_label_voice_icon2->setFixedSize(100,31);
    m_label_voice_icon2->move(60 + 40,80 + (126 + 10) * 1 + (126 - 31)/2);
    m_label_voice_icon2->setStyleSheet("QLabel{border:0px;font:31px;color:#80e3ff;}");
    m_label_voice_icon2->setText(align_text2);
    CCommonInterface::text_dispersed_align(align_text,align_text2,31,nameFont2);
    m_label_voice_icon2->setFont(nameFont2);

    m_label_voice_icon3 = new QLabel(this);
    m_label_voice_icon3->setFixedSize(100,31);
    m_label_voice_icon3->move(60 + 40,80 + (126 + 10) * 2 + (126 - 31)/2);
    m_label_voice_icon3->setStyleSheet("QLabel{border:0px;font:31px;color:#80e3ff;}");
    m_label_voice_icon3->setText(align_text);
    CCommonInterface::text_dispersed_align(align_text,align_text,31,nameFont3);
    m_label_voice_icon3->setFont(nameFont3);


    m_slider1 = new QSlider(this);
    m_slider1->setFixedSize(230,80);
    m_slider1->move(60 + 40 + 100,80 + (126 - 80)/2);
    m_slider1->setOrientation(Qt::Horizontal);
    m_slider1->setStyleSheet(SLIDER_CSS);
    m_slider1->setMinimum(0);  // 最小值
    m_slider1->setMaximum(VOICE_MAX);  // 最大值


    m_slider2 = new QSlider(this);
    m_slider2->setFixedSize(230,80);
    m_slider2->move(60 + 40 + 100,80 + (126 + 10) * 1 + (126 - 80)/2);
    m_slider2->setOrientation(Qt::Horizontal);
    m_slider2->setStyleSheet(SLIDER_CSS);
    m_slider2->setMinimum(0);  // 最小值
    m_slider2->setMaximum(VOICE_MAX);  // 最大值


    m_slider3 = new QSlider(this);
    m_slider3->setFixedSize(230,80);
    m_slider3->move(60 + 40 + 100,80 + (126 + 10) * 2 + (126 - 80)/2);
    m_slider3->setOrientation(Qt::Horizontal);
    m_slider3->setStyleSheet(SLIDER_CSS);
    m_slider3->setMinimum(0);  // 最小值
    m_slider3->setMaximum(VOICE_MAX);  // 最大值


    m_label_voice_icon4 = new QLabel(this);
    m_label_voice_icon4->setFixedSize(40,30);
    m_label_voice_icon4->move(60 + 40 + 100 + 230 + 20,80 + (126 + 10) * 0 + (126 - 30)/2);
    m_label_voice_icon4->setStyleSheet("QLabel{border:0px;font:31px;color:#80e3ff;}");
    m_label_voice_icon4->setText(tr("0"));

    m_label_voice_icon5 = new QLabel(this);
    m_label_voice_icon5->setFixedSize(40,30);
    m_label_voice_icon5->move(60 + 40 + 100 + 230 + 20,80 + (126 + 10) * 1 + (126 - 30)/2);
    m_label_voice_icon5->setStyleSheet("QLabel{border:0px;font:31px;color:#80e3ff;}");
    m_label_voice_icon5->setText(tr("0"));

    m_label_voice_icon6 = new QLabel(this);
    m_label_voice_icon6->setFixedSize(40,30);
    m_label_voice_icon6->move(60 + 40 + 100 + 230 + 20,80 + (126 + 10) * 2 + (126 - 30)/2);
    m_label_voice_icon6->setStyleSheet("QLabel{border:0px;font:31px;color:#80e3ff;}");
    m_label_voice_icon6->setText(tr("0"));
/*
    QPushButton *m_pushbutton_voice1;
    QPushButton *m_pushbutton_voice2;
    QPushButton *m_pushbutton_voice3;
*/
    m_pushbutton_voice1 = new QPushButton(this);
    m_pushbutton_voice1->setFixedSize(62,80);
    m_pushbutton_voice1->move(60 + 40 + 100 + 230 + 20 + 40 + 20,80 + (126 + 10) * 0 + (126 - 80)/2);
    //m_pushbutton_voice1->setStyleSheet("QLabel{border:0px;border-image:url(:/img/res/main_page/popup_dialog/listen.png);}");
  // str_sheet = QString("QPushButton {border:0px;border-image:url(:/img/res/main_page/popup_dialog/listen.png); }");
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
            .arg(":/img/res/main_page/popup_dialog/listen.png").arg(":/img/res/main_page/popup_dialog/listen_hover.png").arg(":/img/res/main_page/popup_dialog/listen_hover.png");

   CCommonInterface::modify_button_sheet(m_pushbutton_voice1,str_sheet);

    m_pushbutton_voice2 = new QPushButton(this);
    m_pushbutton_voice2->setFixedSize(62,80);
    m_pushbutton_voice2->move(60 + 40 + 100 + 230 + 20 + 40 + 20,80 + (126 + 10) * 1 + (126 - 80)/2);
    //m_pushbutton_voice2->setStyleSheet("QLabel{border:0px;border-image:url(:/img/res/main_page/popup_dialog/listen.png);}");
    CCommonInterface::modify_button_sheet(m_pushbutton_voice2,str_sheet);

    m_pushbutton_voice3 = new QPushButton(this);
    m_pushbutton_voice3->setFixedSize(62,80);
    m_pushbutton_voice3->move(60 + 40 + 100 + 230 + 20 + 40 + 20,80 + (126 + 10) * 2 + (126 - 80)/2);
    //m_pushbutton_voice3->setStyleSheet("QLabel{border:0px;border-image:url(:/img/res/main_page/popup_dialog/listen.png);}");
    CCommonInterface::modify_button_sheet(m_pushbutton_voice3,str_sheet);

    connect(m_pushbutton_voice1,SIGNAL(clicked()),this,SLOT(slot_pushbutton_voice1()));
    connect(m_pushbutton_voice2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_voice2()));
    connect(m_pushbutton_voice3,SIGNAL(clicked()),this,SLOT(slot_pushbutton_voice3()));

    connect(m_slider1,SIGNAL(valueChanged(int)),this,SLOT(slot_value_change_slider1(int)));
    connect(m_slider2,SIGNAL(valueChanged(int)),this,SLOT(slot_value_change_slider2(int)));
    connect(m_slider3,SIGNAL(valueChanged(int)),this,SLOT(slot_value_change_slider3(int)));


}

void CVoiceDialog::slot_pushbutton_close()
{

    if(CCommonData::getInstance()->m_CGuiVoice.m_volume_within != m_slider1->value()
    || CCommonData::getInstance()->m_CGuiVoice.m_volume_out != m_slider2->value()
    || CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS != m_slider3->value())
    {

        if(CCommonData::getInstance()->messagebox_data_change())
        {
            CCommonData::getInstance()->m_CGuiVoice.m_volume_within = m_slider1->value();
            CCommonData::getInstance()->m_CGuiVoice.m_volume_out = m_slider2->value();
            CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS = m_slider3->value();
            call_json_interface(kEnumVolumeSet,0);
        }
        else
        {
            m_slider1->setValue(CCommonData::getInstance()->m_CGuiVoice.m_volume_within);
            m_slider2->setValue(CCommonData::getInstance()->m_CGuiVoice.m_volume_out);
            m_slider3->setValue(CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS);
        }
    }

    close();
}

void CVoiceDialog::slot_pushbutton_voice1()
{
    call_json_interface(kEnumVolumePlay,kEnumPlayWithin);
}
void CVoiceDialog::slot_pushbutton_voice2()
{
    call_json_interface(kEnumVolumePlay,kEnumPlayOut);
}
void CVoiceDialog::slot_pushbutton_voice3()
{
    call_json_interface(kEnumVolumePlay,kEnumPlayTTS);
}
void CVoiceDialog::call_json_interface(char _action,char _play)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }
        cJSON_AddStringToObject(root, "msg_type", "gui_voice");/*声音*/

        cJSON_AddNumberToObject(root, "type", _action);
        if(kEnumVolumeGet != _action)
        {
            cJSON_AddNumberToObject(root, "volume_within", m_slider1->value());/*内音量*/
            cJSON_AddNumberToObject(root, "volume_out", m_slider2->value());/*外音量*/
            cJSON_AddNumberToObject(root, "volume_TTS", m_slider3->value());/*TTS音*/ /*司机喇叭*/

        }

        if(kEnumVolumePlay == _action)
            cJSON_AddNumberToObject(root, "volume_play", _play);/*播放声音*/ /*1:内音量 2:外音量 3:TTS音*/

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

void CVoiceDialog::data_init()
{
    call_json_interface(kEnumVolumeGet,0);

    if(CCommonData::getInstance()->m_CGuiVoice.m_volume_within != 0)
    {
        m_slider1->setValue(CCommonData::getInstance()->m_CGuiVoice.m_volume_within);
        m_label_voice_icon4->setText(QString::number(CCommonData::getInstance()->m_CGuiVoice.m_volume_within));
    }

    if(CCommonData::getInstance()->m_CGuiVoice.m_volume_out != 0)
    {
        m_slider2->setValue(CCommonData::getInstance()->m_CGuiVoice.m_volume_out);
        m_label_voice_icon5->setText(QString::number(CCommonData::getInstance()->m_CGuiVoice.m_volume_out));
    }

    if(CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS != 0)
    {
        m_slider3->setValue(CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS);
        m_label_voice_icon6->setText(QString::number(CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS));
    }

}

void CVoiceDialog::slot_voice_dialog_event(int _value1,int _value2,int _value3)
{
    CCommonData::getInstance()->m_CGuiVoice.m_volume_within = _value1;
    m_slider1->setValue(CCommonData::getInstance()->m_CGuiVoice.m_volume_within);
    m_label_voice_icon4->setText(QString::number(CCommonData::getInstance()->m_CGuiVoice.m_volume_within));

    CCommonData::getInstance()->m_CGuiVoice.m_volume_out = _value2;
    m_slider2->setValue(CCommonData::getInstance()->m_CGuiVoice.m_volume_out);
    m_label_voice_icon5->setText(QString::number(CCommonData::getInstance()->m_CGuiVoice.m_volume_out));

    CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS = _value3;
    m_slider3->setValue(CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS);
    m_label_voice_icon6->setText(QString::number(CCommonData::getInstance()->m_CGuiVoice.m_volume_TTS));

}

void CVoiceDialog::slot_value_change_slider1(int _value)
{
    m_label_voice_icon4->setText(QString::number(_value));
}
void CVoiceDialog::slot_value_change_slider2(int _value)
{
    m_label_voice_icon5->setText(QString::number(_value));
}
void CVoiceDialog::slot_value_change_slider3(int _value)
{
    m_label_voice_icon6->setText(QString::number(_value));
}

bool CVoiceDialog::keyboard_value(int _value)
{
    bool ret_value = false;
    switch(_value)
    {
        case KEYPAD_KEY7_F0:
        {

        }break;
        case KEYPAD_KEY7_F1:
        {

        }break;
        case KEYPAD_KEY7_F2:
        {

        }break;
        case KEYPAD_KEY7_F3:
        {

        }break;
        case KEYPAD_KEY7_F4:
        {

        }break;
        case KEYPAD_KEY7_F5:
        {

        }break;
        case KEYPAD_KEY7_F6:
        {

        }break;
        case KEYPAD_KEY7_F7:
        {

        }break;
        case KEYPAD_KEY7_F8:
        {

        }break;
        case KEYPAD_KEY7_F9:
        {

        }break;
        case KEYPAD_KEY7_PLAY:
        {

        }break;
        case KEYPAD_KEY7_REPLAY:
        {

        }break;
        case KEYPAD_KEY7_MENU:
        {

        }break;
        case KEYPAD_KEY7_EXIT:
        {
            slot_pushbutton_close();
            ret_value = true;
        }break;
        case KEYPAD_KEY7_FUN:
        {

        }break;
        case KEYPAD_KEY7_UP:
        {

        }break;
        case KEYPAD_KEY7_LEFT:
        {

        }break;
        case KEYPAD_KEY7_RIGHT:
        {

        }break;
        case KEYPAD_KEY7_DOWN:
        {

        }break;
    }
    return ret_value;
}
