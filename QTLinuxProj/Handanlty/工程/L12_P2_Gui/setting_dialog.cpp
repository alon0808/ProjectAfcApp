#include "setting_dialog.h"

#include "./class/common_interface.h"
#include "LtyCommonStyle.h"

#include "protocol.h"
#include "common_data.h"


#include <QDebug>
#include <QIntValidator>

CSettingDialog::CSettingDialog(QWidget *parent) :
    QDialog(parent)
{
     qDebug() << "CSettingDialog::CSettingDialog(QWidget *parent) :>>>>>>>>>>>>>\n";
    this->setFixedSize(1024,768);
    this->setObjectName("CSettingDialog");
    this->setStyleSheet("QDialog#CSettingDialog{background-color:#000000;}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(570, 410);
    m_widget_bg->move(35, 38);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img_p2/res/start/content_background_dk.png);}");


    m_SettingLable = new QLabel(this);
    QString showText1= tr("设置");
    QString str_sheet1 = "QLabel{border:0px solid #ffffff;font:30px;color:#94c6ff;}";     ////////boder temp
    CCommonInterface::init_label_text(m_SettingLable,262, 67, 150,50, showText1, str_sheet1); ////hight, width temp
    m_SettingLable->setAlignment(Qt::AlignCenter);

     m_CityLable = new QLabel(this);
    QString showText2= tr("城市ID");
    QString str_sheet2 = "QLabel{border:0px solid #ffffff;font:14px;color:#8ea2c0;}";     ////////boder temp
    CCommonInterface::init_label_text(m_CityLable,170, 131, 150,50, showText2, str_sheet2); ////hight, width temp
    m_CityLable->setAlignment(Qt::AlignLeft);



    m_CompLable = new QLabel(this);
    QString showText3 = tr("公司ID");
    QString str_sheet3 = "QLabel{border:0px solid #ffffff;font:14px;color:#8ea2c0;}";     ////////boder temp
    CCommonInterface::init_label_text(m_CompLable,170, 196, 150,50, showText3, str_sheet3); ////hight, width temp
    m_CompLable->setAlignment(Qt::AlignLeft);

    m_LineLable = new QLabel(this);
    QString showText4 = tr("线路ID");
    QString str_sheet4 = "QLabel{border:0px solid #ffffff;font:14px;color:#8ea2c0;}";     ////////boder temp
    CCommonInterface::init_label_text(m_LineLable,170, 259, 150,50, showText4, str_sheet4); ////hight, width temp
    m_LineLable->setAlignment(Qt::AlignLeft);

      m_CarNumLable = new QLabel(this);
      QString showText5 = tr("车辆编号");
      QString str_sheet5 = "QLabel{border:0px solid #ffffff;font:14px;color:#8ea2c0;}";     ////////boder temp
      CCommonInterface::init_label_text(m_CarNumLable,163, 323, 150,50, showText5, str_sheet5); ////hight, width temp
      m_CarNumLable->setAlignment(Qt::AlignLeft);

       QIntValidator *validor1 = new QIntValidator (0, 999999, this);
       m_CityLine = new QLineEdit(this);
       m_CityLine->move(236, 119);
       m_CityLine->setFixedSize(240, 40);
       m_CityLine->setStyleSheet("QLineEdit {font:16px;color:#94c6ff;text-align:center;border:1px solid #275ba7;background:transparent; }");
       // CCommonInterface::set_lineedit_sheet(m_CityLine);
       m_CityLine->setValidator(validor1);
       m_CityLine->setContextMenuPolicy(Qt::NoContextMenu);

       QIntValidator *validor2 = new QIntValidator (0, 99, this);
       m_CompLine = new QLineEdit(this);
       m_CompLine->move(236, 183);
       m_CompLine->setFixedSize(240, 40);
       m_CompLine->setStyleSheet("QLineEdit {font:16px;color:#94c6ff;text-align:center;border:1px solid #275ba7;background:transparent; }");
       // CCommonInterface::set_lineedit_sheet(m_CompLine);
       m_CompLine->setValidator(validor2);
       m_CompLine->setContextMenuPolicy(Qt::NoContextMenu);

       QIntValidator *validor3 = new QIntValidator (0, 999999, this);
       m_LineLine = new QLineEdit(this);
       m_LineLine->move(236, 247);
       m_LineLine->setFixedSize(240, 40);
       m_LineLine->setStyleSheet("QLineEdit {font:16px;color:#94c6ff;text-align:center;border:1px solid #275ba7;background:transparent; }");
       m_LineLine->setValidator(validor3);
       m_LineLine->setContextMenuPolicy(Qt::NoContextMenu);


      QIntValidator *validor4 = new QIntValidator (0, 999999, this);
      m_CarLIne = new QLineEdit(this);
      m_CarLIne->move(236, 311);
      m_CarLIne->setFixedSize(240, 40);
      m_CarLIne->setStyleSheet("QLineEdit {font:16px;color:#94c6ff;text-align:center;border:1px solid #275ba7;background:transparent; }");
      // CCommonInterface::set_lineedit_sheet(m_CarLIne);
      m_CarLIne->setValidator(validor4);
      m_CarLIne->setContextMenuPolicy(Qt::NoContextMenu);


     m_SettingButton = new QPushButton (this);
//     QString   str_sheet6 = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
//        .arg(":/img_p2/res/start/button_confirm_default.png").arg(":/img_p2/res/start/button_confirm_default.png").arg(":/img_p2/res/start/button_confirm_selected.png");
     CCommonInterface::init_new_button(m_SettingButton,259, 384, 120, 45,LTY_PUSHBUTTON_CONFIRM);
     m_SettingButton->setText(tr("连接"));
     connect(m_SettingButton, SIGNAL(clicked()), this, SLOT(slot_settingButtonClicked()));

     m_SNLable = new QLabel(this);
     QString showText6 = tr("SN:");
     QString str_sheet6 = "QLabel{border:0px solid #ffffff;font:14px;color:#8ea2c0;}";     ////////boder temp
     CCommonInterface::init_label_text(m_SNLable,50, 400, 30, 50, showText6, str_sheet6); ////hight, width temp
     m_SNLable->setAlignment(Qt::AlignLeft);

    m_SNNumLable = new QLabel(this);
    QString showText7 = tr("");
    if(NULL != CCommonData::getInstance())
    {
        showText7 = CCommonData::getInstance()->m_snNum;
    }
    QString str_sheet7 = "QLabel{border:0px solid #ffffff;font:14px;color:#8ea2c0;}";     ////////boder temp
    CCommonInterface::init_label_text(m_SNNumLable, 81, 400, 120,50, showText7, str_sheet7); ////hight, width temp
    m_SNNumLable->setAlignment(Qt::AlignLeft);


}


void CSettingDialog::slot_settingButtonClicked()
{
    qDebug() << "CSettingDialog::slot_settingButtonClicked():>>>>>>>>>>>>>\n";
    if( CCommonData::getInstance()->m_CSystemStartingInfo.m_CityLine !=  m_CityLine->text()
        ||  CCommonData::getInstance()->m_CSystemStartingInfo.m_CompLine != m_CompLine->text()
        || CCommonData::getInstance()->m_CSystemStartingInfo.m_CarLIne != m_CarLIne->text()
        || CCommonData::getInstance()->m_CSystemStartingInfo.m_LineLine != m_LineLine->text())
     {
        qDebug() << "CSettingDialog::slot_settingButtonClicked(),valid data, request \n";
        CCommonData::getInstance()->m_CSystemStartingInfo.m_CityLine =  m_CityLine->text(); //用户选择的线路
        CCommonData::getInstance()->m_CSystemStartingInfo.m_CompLine = m_CompLine->text();
        CCommonData::getInstance()->m_CSystemStartingInfo.m_CarLIne = m_CarLIne->text();//车辆编号
        CCommonData::getInstance()->m_CSystemStartingInfo.m_LineLine  = m_LineLine->text();//线路编号
        CProtocol::getInstance()->call_json_system_starting_info_set(CProtocol::kEnumSave);
     }
     else
     {
         qDebug() << "CSettingDialog::slot_settingButtonClicked(),same data, do nothing \n";
     }
     emit  settingButtonClicked();
}

