#include "state_dialog.h"

#include "./class/common_interface.h"
#include "LtyCommonStyle.h"
#include "protocol.h"

#include <QDebug>


CStateDialog::CStateDialog(QWidget *parent) :
    QDialog(parent)
{
     qDebug() << "CStateDialog::CStateDialog(QWidget *parent) :>>>>>>>>>>>";
    this->setFixedSize(1024,768);
    this->setObjectName("CStateDialog");
    this->setStyleSheet("QDialog#CStateDialog{background-color:#000000;}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(570, 410);
    m_widget_bg->move(35, 38);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img_p2/res/start/content_background_dk.png);}");

    m_StateLable = new QLabel(this);
    QString showText= tr("状态");
    QString str_sheet1 = "QLabel{border:0px solid #ffffff;font:30px;color:#94c6ff;}";     ////////boder temp
    CCommonInterface::init_label_text(m_StateLable, 262, 67, 150,50, showText, str_sheet1); ////hight, width temp
    m_StateLable->setAlignment(Qt::AlignCenter);


     m_StateLine = new QTextEdit(this);
     //67为右边距, 82为底部边距, 24为上边距
     m_StateLine->setFixedSize(470 + 67, 240 + 82+24);

     m_StateLine->move(75, 114-24);
     // m_StateLine->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
     m_StateLine->setStyleSheet("QTextEdit{font:19px; border:0px solid #ffffff;background:transparent;color:#94c6ff;padding-bottom:82px; padding-right:67px;padding-top:24px;}");/////two words? temp
     m_StateLine->setAlignment(Qt::AlignLeft);
     m_StateLine->setReadOnly(true);
     m_StateLine->setText(tr("正在搜索网络……"));
     m_StateLine->append(tr("正在查找SIM卡……"));
     m_StateLine->setContextMenuPolicy(Qt::NoContextMenu);

    m_ScrollController = new ScrollController(this);
    m_ScrollController->addMonitorWidget((QScrollArea *)(m_StateLine));
    m_ScrollController->setVerticalScrollBarStyleSheet(CCommonData::getInstance()->get_scroll_sheet(),0,50,50);
    m_ScrollController->set_scroll_right_length(20);

         m_StatePreButton = new QPushButton (this);
//     QString   str_sheet2 = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
//        .arg(":/img_p2/res/start/button_complete_default.png").arg(":/img_p2/res/start/button_complete_default.png").arg(":/img_p2/res/start/button_complete_selected.png");
     CCommonInterface::init_new_button(m_StatePreButton,191, 384, 120, 45,LTY_PUSHBUTTON_FINISH);
     m_StatePreButton->setText(tr("上一步"));
     connect(m_StatePreButton, SIGNAL(clicked()), this, SIGNAL(statePreButtonClicked()));

     m_StateFinishButton = new QPushButton (this);
//     QString   str_sheet3 = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
//        .arg(":/img_p2/res/start/button_complete_default.png").arg(":/img_p2/res/start/button_complete_default.png").arg(":/img_p2/res/start/button_complete_selected.png");
     CCommonInterface::init_new_button(m_StateFinishButton,335, 384, 120, 45, LTY_PUSHBUTTON_FINISH);
     m_StateFinishButton->setText(tr("完成"));
     m_StateFinishButton->setEnabled(false);
     connect(m_StateFinishButton, SIGNAL(clicked()), this, SLOT(slot_stateFinishButtonClicked()));


}
void CStateDialog::slot_g3_network_status(int type, int state)
{
   QString strState = tr("");
   if(kEnumNetworkG3 == type)
   {
        if(G3_NETWORK_STATUS_FORBIDDEN == state )
         {
            strState = tr("正在注册网络……");
         }
         else if(G3_NETWORK_STATUS_STOPPING ==state )
         {
            strState = tr("网络注册失败……");
         }
         else if( G3_NETWORK_STATUS_WAITTING == state || G3_NETWORK_STATUS_DIALING == state)
         {
             strState = tr("正在注册网络……");
         }
         else if(G3_NETWORK_STATUS_NORMAL == state)
         {
             strState = tr("网络注册成功……");

         }
   }
   m_StateLine->append(strState);

}

void CStateDialog::slot_tmp_status(int type, int state, int errorType)
{
   QString strState = tr("");
   if (kEnumTMPLogin == type)
   {
      if( kEnumTMP_DOING == state)
      {
          strState = tr("正在登录云平台……");
      }
      else if(kEnumTMP_SUCCESS == state)
      {  
         m_StateLine->append(tr("云平台登录成功……"));
		 strState = tr("系统注册完成，欢迎使用……");
         m_StateFinishButton->setEnabled(true);
      }
      else if(kEnumTMP_FAILURE == state)
      {
          m_StateLine->append(tr("云平台登录失败……"));
          if(1 == errorType)
          {
              strState = tr("城市ID错误……");
          }
          else if(2 == errorType)
          {
              strState = tr("公司ID错误……");
          }
          else if(3 == errorType)
          {
              strState = tr("线路ID错误……");
          }
          else if(4 == errorType)
          {
              strState = tr("车辆ID错误……");
          }
          else
          {
              strState = tr("其他未知错误……");
          }

      }
	  else if(kEnumTMP_SNDOING == state)
      {
          strState = tr("正在登录序列号……");
      }
	  else if(kEnumTMP_SNSUCCESS == state)
      {
          strState = tr("序列号登录成功……");
      }
      else if(kEnumTMP_SNFAILURE == state)
      {
          strState = tr("序列号登录失败……");
      }
      else
      {}
   }
   else if(kEnumTMPConnect == type)
   {
       if( kEnumTMP_DOING == state)
      {
          strState = tr("正在连接云平台……");
      }
      else if(kEnumTMP_SUCCESS == state)
      {
          strState = tr("云平台连接成功……");
      }
      else if(kEnumTMP_FAILURE == state)
      {
          strState = tr("云平台连接失败……");
      }
	  
      else
      {}
   }
   else
   {}
   if(QString("") != strState)
   {
       m_StateLine->append(strState);
   }
}

void CStateDialog::slot_network_status(int type, const CRunStatusInfo &_runStatusInfo)
{

      // sim card
     if(NETWORK_TYPE_G3 == type)
     {
          if ( E_G3_SIM_STATUS_VALID  != _runStatusInfo.m_CRunStatusMobileNetwork->m_sim_status)
          {
              m_StateLine->append(tr("未找到SIM卡……"));
              m_StateLine->append(tr("正在查找SIM卡……"));
          }
          else if(E_G3_SIM_STATUS_VALID  == _runStatusInfo.m_CRunStatusMobileNetwork->m_sim_status)
          {
             //m_StateLine->append(tr("正在注册网络……"));
          }
          else
          {
//              if(2 == _runStatusInfo.m_CRunStatusMobileNetwork->m_model_status)
//              {
//                  m_StateLine->append(tr("通讯模块异常"));
//              }
//              else
//              {}
          }
     }
}

void CStateDialog:: slot_stateFinishButtonClicked()
{
    qDebug() << "CStateDialog:: slot_stateFinishButtonClicked():>>>>>>>>>>>>>\n";
    CCommonData::getInstance()->m_naviFinish =  1; //用户选择的线路
    CProtocol::getInstance()->call_json_starting_screen_flag_set(CProtocol::kEnumSave);
    emit  stateFinishButtonClicked();

}


