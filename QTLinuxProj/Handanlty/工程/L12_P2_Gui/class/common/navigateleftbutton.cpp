#include "navigateleftbutton.h"

#include "navigateleftbutton.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
#include "./class/common/iconLocation.h"

#include "./class/common/commonuiinit.h"

#define TEXT_FONT_SIZE  30


NavigateLeftButton::NavigateLeftButton(int _width,int _height,ButtonMode paramButtonMode, QWidget *parent, QString paramNormalIconStr, QString paramActiveIconStr, QString paramPressedIconStr, QString paramDisabledIconStr)
    : QToolButton(parent), m_buttonMode(paramButtonMode), m_normalIconStr(paramNormalIconStr), m_activeIconStr(paramActiveIconStr), m_pressedIconStr(paramPressedIconStr), m_disabledIconStr(paramDisabledIconStr)
{
    m_width = _width;
    m_height = _height;
    m_isSelected = false;
    m_addtionMarginRight = 0;
    m_dispersed = false;

    m_define_screen = new CMyDefineScreen();

    createWidgets();

    initSlotConnection();

    this->installEventFilter(this);

    if(NavigateLeftMode == m_buttonMode)
        m_define_screen->add_parent(this,true);
    else
        m_define_screen->add_parent(this,false);

    m_normal_first = true;
    m_pressed_first = true;

}

void NavigateLeftButton::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

NavigateLeftButton::~NavigateLeftButton()
{
    if (m_pHMainLayout){
        delete m_pHMainLayout;
        m_pHMainLayout = NULL;
    }
    if (m_pVMainLayout){
        delete m_pVMainLayout;
        m_pVMainLayout = NULL;
    }
}
#include "class/common_interface.h"
void NavigateLeftButton::createWidgets()
{
    this->setObjectName("NavigateLeftButton");
    this->setStyleSheet("QToolButton#NavigateLeftButton{border:0px solid #ffffff;padding:0px;background-color:transparent;}");

    m_QLabelBackground = new QLabel(this);
    m_QLabelIcon = new QLabel(this);
    m_QLabelText = new QLabel(this);
    m_pHMainLayout = new QHBoxLayout();
    m_pVMainLayout = new QVBoxLayout();
    m_label_text1 = NULL;
    m_label_text2 = NULL;

    switch (m_buttonMode)
    {
    case NavigateLeftMode:
        {
            this->setFixedSize(m_width, m_height);

            //m_QLabelBackground->setFixedSize(m_width, m_define_screen->get_change_factor_y(78)/*m_height*/);
            //m_QLabelBackground->move(0, (m_height - m_define_screen->get_change_factor_y(78))/2);
            m_QLabelBackground->setFixedSize(m_width, 78/*m_height*/);
            m_QLabelBackground->move(0, 18/2);
            //m_QLabelBackground->setScaledContents(true);
            m_QLabelIcon->setFixedSize(23, 23);


            m_QLabelText->setFixedSize(m_width, m_height);
            m_QLabelText->move(0,0);

            m_QLabelText->setAlignment(Qt::AlignVCenter);
            //m_QLabelText->setFixedHeight(42);
            #if 0
            m_pHMainLayout->setSpacing(0);
            m_pHMainLayout->setContentsMargins(0, 0, 0, 0);
            m_pHMainLayout->setAlignment(Qt::AlignLeft);
            //m_pHMainLayout->addWidget(m_QLabelIcon);
            //m_pHMainLayout->setSpacing();
            m_pHMainLayout->addWidget(m_QLabelText);
            m_pHMainLayout->addStretch(0);

            this->setLayout(m_pHMainLayout);
            #endif
        }
        break;
    case NavigateTopMode:
        {
            this->setFixedSize(90, 84);

            m_QLabelBackground->setFixedSize(90, 84);
            m_QLabelBackground->move(0, 0);

            m_QLabelIcon->setFixedSize(44, 44);

            m_QLabelText->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
            m_QLabelText->setFixedSize(44, 15);

            m_pVMainLayout->setSpacing(0);
            m_pVMainLayout->setContentsMargins(23, 11, 23, 0);
            m_pVMainLayout->setAlignment(Qt::AlignTop);
            m_pVMainLayout->addWidget(m_QLabelIcon);
            m_pVMainLayout->addWidget(m_QLabelText);
            m_pVMainLayout->addStretch(0);

            this->setLayout(m_pVMainLayout);
        }
        break;
    case PhoneInfoMode:
        {
            this->setFixedSize(132, 154);

            m_QLabelBackground->setFixedSize(132, 154);
            m_QLabelBackground->move(0, 0);

            m_QLabelIcon->setFixedSize(88, 88);

            m_QLabelText->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
            m_QLabelText->setFixedHeight(20);

            m_pVMainLayout->setSpacing(0);
            m_pVMainLayout->setContentsMargins(0, 28, 0, 0);
            m_pVMainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
            m_pVMainLayout->addWidget(m_QLabelIcon);
            m_pVMainLayout->addWidget(m_QLabelText);
            m_pVMainLayout->addStretch(0);

            this->setLayout(m_pVMainLayout);
        }
        break;
    case PhoneToolBoxMode:
        break;
    case NavigateMiddleMode:
        {
            this->setFixedHeight(NAVIGATE_MIDDLE_HEIGHT);

            m_QLabelBackground->setFixedSize(90, NAVIGATE_MIDDLE_HEIGHT);
            m_QLabelBackground->move(0, 0);

            m_QLabelIcon->setFixedSize(NAVIGATE_MIDDLE_BUTTON_ICON_WIDTH, NAVIGATE_MIDDLE_BUTTON_ICON_HEIGHT);
            //m_QLabelIcon->setStyleSheet("background-color:red;");

            m_QLabelText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            m_QLabelText->setFixedHeight(NAVIGATE_MIDDLE_HEIGHT);
            m_QLabelText->setStyleSheet("background-color:green;");

            m_pHMainLayout->setSpacing(0);
            m_pHMainLayout->setContentsMargins(0, 0, 0, 0);
            //m_pHMainLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            m_pHMainLayout->addWidget(m_QLabelIcon);
            m_pHMainLayout->addWidget(m_QLabelText);
            m_pHMainLayout->addStretch(0);

            this->setLayout(m_pHMainLayout);
        }
        break;
    case BackupRestoreEntryMode:
        {
            this->setFixedSize(88, 108);

            m_QLabelBackground->setFixedSize(88, 108);
            m_QLabelBackground->move(0, 0);

            m_QLabelIcon->setFixedSize(88, 88);

            m_QLabelText->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
            m_QLabelText->setFixedSize(88, 20);

            m_pVMainLayout->setSpacing(0);
            m_pVMainLayout->setContentsMargins(0, 0, 0, 0);
            m_pVMainLayout->setAlignment(Qt::AlignTop);
            m_pVMainLayout->addWidget(m_QLabelIcon);
            m_pVMainLayout->addWidget(m_QLabelText);
            m_pVMainLayout->addStretch(0);

            this->setLayout(m_pVMainLayout);
        }
        break;
    case BackupMode:
        {
            this->setFixedSize(240, 136);

            m_QLabelBackground->setFixedSize(240, 136);
            m_QLabelBackground->move(0, 0);

            m_QLabelIcon->setFixedSize(88, 88);

            m_QLabelText->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
            m_QLabelText->setFixedSize(88, 20);

            m_pVMainLayout->setSpacing(0);
            m_pVMainLayout->setContentsMargins(76, 9, 76, 0);
            m_pVMainLayout->setAlignment(Qt::AlignTop);
            m_pVMainLayout->addWidget(m_QLabelIcon);
            m_pVMainLayout->addWidget(m_QLabelText);
            m_pVMainLayout->addStretch(0);

            this->setLayout(m_pVMainLayout);
        }
        break;
     case PullMode:
    {
        this->setFixedSize(220, 768 - (64 + 76 + 9 * 64));

        m_QLabelBackground->setFixedSize(220, 768 - (64 + 76 + 9 * 64));
        //m_QLabelBackground->move(0, 0);

        m_QLabelIcon->setFixedSize(18, 18);

        m_QLabelText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_QLabelText->setFixedHeight(38);

        m_pHMainLayout->setSpacing(0);
        m_pHMainLayout->setContentsMargins(30, 0, 0, 0);
        m_pHMainLayout->setAlignment(Qt::AlignLeft);
        m_pHMainLayout->addWidget(m_QLabelIcon);
        m_pHMainLayout->setSpacing(6);
        m_pHMainLayout->addWidget(m_QLabelText);
        m_pHMainLayout->addStretch(0);

        this->setLayout(m_pHMainLayout);

    };break;
    case BottomMode:
    {
        this->setFixedSize(146,109);

        m_QLabelIcon->setFixedSize(48,39);

        m_pHMainLayout->setSpacing(0);
        m_pHMainLayout->setContentsMargins(0, 0, 0, 0);
        m_pHMainLayout->setAlignment(Qt::AlignCenter);
        m_pHMainLayout->addWidget(m_QLabelIcon);

        this->setLayout(m_pHMainLayout);

    }break;
    case StorageMode:
    {
        this->setFixedSize(m_width, m_height);

        m_QLabelBackground->setFixedSize(m_width, 77);
        m_QLabelBackground->move(0, 0);

        m_QLabelIcon->setFixedSize(30, 38);
        //m_QLabelIcon->setStyleSheet("background-color:red;");

        m_QLabelText->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        m_QLabelText->setFixedSize(300,40);
        //m_QLabelText->setFixedHeight(NAVIGATE_MIDDLE_HEIGHT);
        //m_QLabelText->setStyleSheet("background-color:green;");

        m_QLabelIcon->move(30,(m_height- 38)/2);
        m_QLabelText->move(30 + 30 + 5,(m_height- 40)/2);

    }break;
    case NextMode:
    {
        this->setFixedSize(m_width, m_height);

        m_QLabelBackground->setFixedSize(m_width, m_height);
        m_QLabelBackground->move(0, 0);

        m_QLabelText->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        m_QLabelText->setFixedSize(300,40);
        m_QLabelText->move(30,(m_height- 40)/2);

        CommonUiInit::setTitleTextStyleSheet(m_QLabelText, 26, "#8ea2c0");

        m_QLabelIcon->setFixedSize(20, 46);
        m_QLabelIcon->move(m_width - 25 - 20,(m_height- 46)/2);

        m_label_text1 = new QLabel(this);
        m_label_text1->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        m_label_text2 = new QLabel(this);
        m_label_text2->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        CommonUiInit::setTitleTextStyleSheet(m_label_text1, 28, "#94c6ff",false,1);
        CommonUiInit::setTitleTextStyleSheet(m_label_text2, 28, "#3ea2ff",false,1);

    }break;
    default:
        break;
    }
}
void NavigateLeftButton::setText1Text2(const QString &_text1,const QString &_text2)
{
    m_label_text1->setText(_text1);
    m_label_text2->setText(_text2);

    int text1_length = CCommonInterface::get_string_length(_text1,28);
    int text2_length = CCommonInterface::get_string_length(_text2,28);



    //m_label_text1->move(m_width - 25 - 20 - text1_length - text2_length - 15,(m_define_screen->get_change_factor_y(m_height) - 40)/2);
    //m_label_text2->move(m_width - 25 - 20 - text2_length - 5,(m_define_screen->get_change_factor_y(m_height) - 40)/2);
    if(m_label_text1->isVisible())
    {
        m_label_text1->setFixedSize(text1_length,m_define_screen->get_change_factor_y(40));
        m_label_text2->setFixedSize(text2_length,m_define_screen->get_change_factor_y(40));
        m_label_text1->move(m_define_screen->get_change_factor_x(m_width - 25 - 20 - text1_length - text2_length - 15),m_define_screen->get_change_factor_y((m_height - 40)/2));
        m_label_text2->move(m_define_screen->get_change_factor_x(m_width - 25 - 20 - text2_length - 5),m_define_screen->get_change_factor_y((m_height - 40)/2));
    }
    else
    {
        m_label_text1->setFixedSize(text1_length,40);
        m_label_text2->setFixedSize(text2_length,40);
        m_label_text1->move(m_width - 25 - 20 - text1_length - text2_length - 15,(m_height - 40)/2);
        m_label_text2->move(m_width - 25 - 20 - text2_length - 5,(m_height - 40)/2);
    }


    if(NavigateLeftMode == m_buttonMode)
        m_define_screen->add_parent(this,true);
    else
        m_define_screen->add_parent(this,false);

}
void NavigateLeftButton::initSlotConnection()
{

}

void NavigateLeftButton::updatePressStyleSheet(bool _first)
{
    m_button_status = kEnumPressed;
    switch (m_buttonMode)
    {
    case NavigateLeftMode:
        {
            //m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_pressedIconStr));
            //CommonUiInit::setTitleTextStyleSheet(m_QLabelText, 29, "#80e3ff"/*TEXT_COLOR_WHITE*/,false,1);

            int left_spacing = m_define_screen->get_change_factor_x(50);
            int font_size = 29;
            int background_width = 230;
            int background_y = 18/2;
#ifdef APPLICATION_TYPE_D2
            left_spacing = 70;
#endif
            if(!_first)
            {
                font_size = m_define_screen->get_change_factor_y(29);
                background_width = m_define_screen->get_change_factor_x(230);
                background_y = (m_define_screen->get_change_factor_y(18))/2;
            }

            m_QLabelText->setStyleSheet(QString("\
                   QLabel{\
                   padding: 0px 0px 0px %1px;\
                   border:0px solid #ffffff;\
                   font:%2px;\
                   color:#80e3ff;\
                   background:transparent;\
                   }").arg(left_spacing).arg(font_size));


    //border-image  background-image
            m_QLabelBackground->setStyleSheet(tr("border-image:url(%1)").arg(m_pressedIconStr));//setStyleSheet("background-color:#ffffff;");
            m_QLabelBackground->setFixedWidth(background_width);
            m_QLabelBackground->move(0, background_y);
        }
        break;
    case NavigateTopMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_pressedIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_S, TEXT_COLOR_WHITE, true);
            m_QLabelBackground->setStyleSheet("background-color:#22b567;");
        }
        break;
    case PhoneInfoMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_pressedIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_M, TEXT_COLOR_BLACK);
        }
        break;
    case PhoneToolBoxMode:
        break;
    case NavigateMiddleMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_pressedIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_S, "#22b567");
        }
        break;
    case BackupRestoreEntryMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_pressedIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_M, TEXT_COLOR_BLACK);
        }
        break;
    case BackupMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_pressedIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_M, TEXT_COLOR_BLACK);
        }
        break;
    case PullMode:
    {
        m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_pressedIconStr));
        CommonUiInit::setTitleTextStyleSheet(m_QLabelText, 26, TEXT_COLOR_WHITE);
        m_QLabelBackground->setStyleSheet("background-color:#1f2024;");
    }
    break;
    case BottomMode:
    {
         m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_pressedIconStr));
    }break;
    case StorageMode:
    {
        //m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_pressedIconStr));
         m_QLabelIcon->setStyleSheet(tr("QLabel{border:0px;background:transparent;border-image:url(%1);}").arg(m_pressedIconStr));
        CommonUiInit::setTitleTextStyleSheet(m_QLabelText, m_define_screen->get_change_factor_y(33), "#0f1d31");
        m_QLabelBackground->setStyleSheet("border:1px solid #24508F;background-color:#3ea2ff;");
    }break;
    case NextMode:
    {
        m_QLabelIcon->setStyleSheet(tr("QLabel{border:0px;background:transparent;border-image:url(%1);}").arg(m_pressedIconStr));
        CommonUiInit::setTitleTextStyleSheet(m_QLabelText, m_define_screen->get_change_factor_y(26), "#8ea2c0");
        m_QLabelBackground->setStyleSheet("border:1px solid #24508F;background-color:#0e2748;");
    }break;
    default:
        break;
    }
}

void NavigateLeftButton::updateNormalStyleSheet(bool _first)
{
    m_button_status = kEnumNormal;
    switch (m_buttonMode)
    {
    case NavigateLeftMode:
        {
            //m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_normalIconStr));
            //CommonUiInit::setTitleTextStyleSheet(m_QLabelText, m_define_screen->get_change_factor_x(29), "#94c6ff",false,1);
            int left_spacing = m_define_screen->get_change_factor_x(50);
            int font_size = 29;
            int background_width = 184;
            int background_y = 18/2;
#ifdef APPLICATION_TYPE_D2
            left_spacing = 70;
#endif
            if(!_first)
            {
                font_size = m_define_screen->get_change_factor_y(29);
                background_width = m_define_screen->get_change_factor_x(184);
                background_y = (m_define_screen->get_change_factor_y(18))/2;
            }
            m_QLabelText->setStyleSheet(QString("\
                       QLabel{\
                       padding: 0px 0px 0px %1px;\
                       border:0px solid #ffffff;\
                       font:%2px;\
                       color:#94c6ff;\
                       background:transparent;\
                       }").arg(left_spacing).arg(font_size));


            m_QLabelBackground->setStyleSheet(tr("border-image:url(%1)").arg(m_normalIconStr));//setStyleSheet("background-color:#1f2024;");
            m_QLabelBackground->setFixedWidth(background_width);
            m_QLabelBackground->move((this->width() - background_width)/2,background_y);
        }
        break;
    case NavigateTopMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_normalIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_S, "#d3f0e1", true);
            m_QLabelBackground->setStyleSheet("background-color:#189854;");
        }
        break;
    case PhoneInfoMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_normalIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_M, TEXT_COLOR_BLACK);
        }
        break;
    case PhoneToolBoxMode:
        break;
    case NavigateMiddleMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_normalIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_S, TEXT_COLOR_BLACK);
        }
        break;
    case BackupRestoreEntryMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_normalIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_M, TEXT_COLOR_BLACK);
        }
        break;
    case BackupMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_normalIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_M, TEXT_COLOR_BLACK);
        }
        break;
        case PullMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_normalIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, 26, TEXT_COLOR_WHITE);
            m_QLabelBackground->setStyleSheet("background-color:#1f2024;");
        }
        break;
     case BottomMode:
     {
         m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_normalIconStr));
     }break;
    case StorageMode:
    {
        //m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_normalIconStr));
        m_QLabelIcon->setStyleSheet(tr("QLabel{border:0px;background:transparent;border-image:url(%1);}").arg(m_normalIconStr));
        //if(_first)
       //     CommonUiInit::setTitleTextStyleSheet(m_QLabelText, 33, "#94c6ff");
       // else
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, m_define_screen->get_change_factor_y(33), "#94c6ff");
        m_QLabelBackground->setStyleSheet("border:1px solid #24508F;background-color:#1d304e;");
    }break;
    case NextMode:
    {
        m_QLabelIcon->setStyleSheet(tr("QLabel{border:0px;background:transparent;border-image:url(%1);}").arg(m_normalIconStr));
        if(_first)
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText,26, "#8ea2c0");
        else
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, m_define_screen->get_change_factor_y(26), "#8ea2c0");
        m_QLabelBackground->setStyleSheet("border:1px solid #24508F;background-color:#03111F;");
    }break;
    default:
        break;
    }
}

void NavigateLeftButton::updateActiveStyleSheet()
{
    m_button_status = kEnumActive;
    switch (m_buttonMode)
    {
    case NavigateLeftMode:
        {
            int left_spacing = m_define_screen->get_change_factor_x(50);
            int font_size = 29;
            int background_width = 184;
            int background_y = 18/2;

                font_size = m_define_screen->get_change_factor_y(29);
                background_width = m_define_screen->get_change_factor_x(184);
                background_y = (m_define_screen->get_change_factor_y(18))/2;
#ifdef APPLICATION_TYPE_D2
            left_spacing = 70;
#endif
            m_QLabelText->setStyleSheet(QString("\
                       QLabel{\
                       padding: 0px 0px 0px %1px;\
                       border:0px solid #ffffff;\
                       font:%2px;\
                       color:#94c6ff;\
                       background:transparent;\
                       }").arg(left_spacing).arg(font_size));


            m_QLabelBackground->setStyleSheet(tr("border-image:url(%1)").arg(m_activeIconStr));
            m_QLabelBackground->setFixedWidth(background_width);
            m_QLabelBackground->move((this->width() - background_width)/2,background_y);
        }
        break;
    case NavigateTopMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_activeIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_S, TEXT_COLOR_WHITE, true);
            m_QLabelBackground->setStyleSheet("background-color:#189854;");
        }
        break;
    case PhoneInfoMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_activeIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_M, TEXT_COLOR_BLACK);
        }
        break;
    case PhoneToolBoxMode:
        break;
    case NavigateMiddleMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_activeIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_S, "#22b567");
        }
        break;
    case BackupRestoreEntryMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_activeIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_M, TEXT_COLOR_BLACK);
        }
        break;
    case BackupMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_activeIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_M, TEXT_COLOR_BLACK);
        }
        break;
    case PullMode:
    {
        m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_activeIconStr));
        CommonUiInit::setTitleTextStyleSheet(m_QLabelText, 26, TEXT_COLOR_WHITE);
        m_QLabelBackground->setStyleSheet("background-color:transparent;");
    }
    break;
     case BottomMode:
     {
         m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_activeIconStr));
     }break;
    default:
        break;
    }
}

void NavigateLeftButton::updateDisabledStyleSheet()
{
    switch (m_buttonMode)
    {
    case NavigateLeftMode:
        break;
    case NavigateTopMode:
        break;
    case PhoneInfoMode:
        break;
    case PhoneToolBoxMode:
        break;
    case NavigateMiddleMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_disabledIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_S, "#cccccc");
        }
        break;
    case BackupRestoreEntryMode:
        break;
    case BackupMode:
        {
            m_QLabelIcon->setStyleSheet(tr("background-image:url(%1)").arg(m_disabledIconStr));
            CommonUiInit::setTitleTextStyleSheet(m_QLabelText, FONT_SIZE_M, TEXT_COLOR_BLACK);
        }
        break;
    default:
        break;
    }
}

void NavigateLeftButton::updateWidth()
{
    if (m_buttonMode == NavigateMiddleMode)	//更新长度
    {
        QFont fontTmp;
        fontTmp.setPixelSize(12);
        QFontMetrics fm(fontTmp);
        int pixelsWide = fm.width(m_QLabelText->text());
        this->setFixedWidth(NAVIGATE_MIDDLE_BUTTON_ICON_WIDTH + pixelsWide + m_addtionMarginRight);
    }
}

void NavigateLeftButton::setSelected(bool paramFlag, bool paramIsForce)
{
#if 0
    if (m_isSelected != paramFlag)
    {
        m_isSelected = paramFlag;
        paramIsForce = true;
    }

    if (paramIsForce)
    {
        if (m_isSelected)
        {
            updatePressStyleSheet();
        }
        else
        {
            updateNormalStyleSheet();
        }
    }
#else

    m_isSelected = paramFlag;
    if(paramFlag)
    {
        if(this->isVisible() && m_pressed_first)
            m_pressed_first = false;
        updatePressStyleSheet(m_pressed_first);

    }
    else
    {
        if(this->isVisible() && m_normal_first)
            m_normal_first = false;
        updateNormalStyleSheet(m_normal_first);

    }
#endif

#if 1
    if(NavigateLeftMode == m_buttonMode)
        m_define_screen->add_parent(this,true);
    else
        m_define_screen->add_parent(this,false);
#endif
    updateWidth();
}

void NavigateLeftButton::setNormalIconStr(const QString &paramText)
{
    m_normalIconStr = paramText;
    updateNormalStyleSheet(true);
#if 0
    if(NavigateLeftMode == m_buttonMode)
        m_define_screen->add_parent(this,true);
    else
        m_define_screen->add_parent(this,false);
#endif
}

void NavigateLeftButton::setActiveIconStr(const QString &paramText)
{
    m_activeIconStr = paramText;
}

void NavigateLeftButton::setPressedIconStr(const QString &paramText)
{
    m_pressedIconStr = paramText;
}

void NavigateLeftButton::setDisabledIconStr(const QString &paramText)
{
    m_disabledIconStr = paramText;
}

void NavigateLeftButton::set_dispersed_align(bool _flag)
{
    m_dispersed = _flag;
}


void NavigateLeftButton::setDisplayText(const QString &paramText, int paramAddtionMarginRight)
{
    if (m_QLabelText)
    {
        m_addtionMarginRight = paramAddtionMarginRight;

        if(m_dispersed)
        {
            QFont nameFont;
            int font_size = 29;/*m_define_screen->get_change_factor_x(29)*/;
            CCommonInterface::text_dispersed_align(tr("客流统计"),paramText,font_size,nameFont);
            m_QLabelText->setFont(nameFont);
        }


        m_QLabelText->setText(paramText);

        updateWidth();
    }
}

void NavigateLeftButton::setButtonMode(ButtonMode paramButtonMode)
{
    m_buttonMode = paramButtonMode;
}

NavigateLeftButton::ButtonMode NavigateLeftButton::buttonMode()
{
    return m_buttonMode;
}

void NavigateLeftButton::setButtonEnabled(bool paramFlag)
{
    if (this->isEnabled() != paramFlag)
    {
        this->setEnabled(paramFlag);
        if (paramFlag)
        {
            if (m_isSelected)
            {
                updatePressStyleSheet();
            }
            else
            {
                updateNormalStyleSheet();
            }
        }
        else
        {
            updateDisabledStyleSheet();
        }
    }
}

void NavigateLeftButton::setNavigateMiddleHeight(int paramHeight)
{
    this->setFixedHeight(paramHeight);
    this->m_QLabelText->setFixedHeight(paramHeight);
}

void NavigateLeftButton::setIconSize(int _w,int _h)
{
    this->m_QLabelIcon->setFixedSize(_w,_h);
}

void NavigateLeftButton::setAllButtonEnable(bool paramFlag, QList<NavigateLeftButton *> paramAllBtnList, QList<NavigateLeftButton *> paramIgnoreList)
{
    QList<NavigateLeftButton *> btnListTmp;

    for (int i = 0; i < paramAllBtnList.count(); i++)
    {
        btnListTmp.append(paramAllBtnList.at(i));
    }

    for (int i = 0; i < paramIgnoreList.count(); i++)
    {
        btnListTmp.removeOne(paramIgnoreList.at(i));
    }

    for (int i = 0; i < btnListTmp.count(); i++)
    {
        if (btnListTmp.at(i)->isEnabled() != paramFlag)
        {
            btnListTmp.at(i)->setButtonEnabled(paramFlag);
        }
    }
}

bool NavigateLeftButton::eventFilter(QObject *obj, QEvent *e)
{
    if (m_isSelected){
        if (e->type() == QEvent::HoverEnter){
            if (this->isEnabled()){
                updatePressStyleSheet();
            }
        }
        else if (e->type() == QEvent::HoverLeave){
            if (this->isEnabled()){
                updatePressStyleSheet();
            }
        }
        else if (e->type() == QEvent::MouseButtonPress){
            if (this->isEnabled()){
                updatePressStyleSheet();
            }
        }
        else if (e->type() == QEvent::MouseButtonRelease){
            if (this->isEnabled()){
                updatePressStyleSheet();
            }
        }
    }
    else{
        if (e->type() == QEvent::HoverEnter){
            if (this->isEnabled()){
                updateActiveStyleSheet();
            }
        }
        else if (e->type() == QEvent::HoverLeave){
            if (this->isEnabled()){
                updateNormalStyleSheet();
            }
        }
        else if (e->type() == QEvent::MouseButtonPress){
            if (this->isEnabled()){
                updatePressStyleSheet();
            }
        }
        else if (e->type() == QEvent::MouseButtonRelease){
            if (this->isEnabled()){
                updateNormalStyleSheet();
            }
        }
    }
    return QToolButton::eventFilter(obj, e);
}

QString NavigateLeftButton::getButtonText()
{
    return m_QLabelText->text();
}

char NavigateLeftButton::get_button_status()
{
    return m_button_status;
}

void NavigateLeftButton::focusInEvent(QFocusEvent *e)
{
    m_save_status = m_button_status;
    if(StorageMode == m_buttonMode)
    {
        m_QLabelIcon->setStyleSheet(tr("QLabel{border:0px;background:transparent;border-image:url(%1);}").arg(m_normalIconStr));

        CommonUiInit::setTitleTextStyleSheet(m_QLabelText, m_define_screen->get_change_factor_y(33), "#94c6ff");
        m_QLabelBackground->setStyleSheet("border:1px solid #24508F;background-color:#ffffff;");
    }
}

void NavigateLeftButton::focusOutEvent(QFocusEvent *e)
{
    if(StorageMode == m_buttonMode)
    {
        if(kEnumNormal == m_save_status)
            updateNormalStyleSheet();
        else if(kEnumPressed == m_save_status)
            updatePressStyleSheet();
    }
}
















