#include "navigate_left.h"
#include <QToolButton>

#define NAVIGATE_BUTTON_WIDTH   220
#define NAVIGATE_BUTTON_HEIGTH   80

CNavigateLeft::CNavigateLeft(int _buttonWidth,int _buttonHeight,int _height,QWidget *parent) :
    QWidget(parent)
{
    //m_define_screen = new CMyDefineScreen();

    m_button_width = _buttonWidth;
    m_button_height = _buttonHeight;

    //this->setObjectName("CNavigateLeft");
    //this->setStyleSheet("QWidget#CNavigateLeft{background-color:#ffffff;}");
    //this->setFixedSize(220,400);
    //this->setFixedSize(220,9 * 64);
    this->move(0,0);

#if 1
    m_pMainLayout = new QVBoxLayout();
    m_pMainLayout->setContentsMargins(0,0,0,0);
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setAlignment(Qt::AlignLeft/* | Qt::AlignTop*/);
    m_pMainLayout->addStretch(0);
    this->setLayout(m_pMainLayout);
#endif

//    this->setStyleSheet("QWidget{border:1px solid #ffffff; background-color: #ffffff;}");

#if 1
#define LTY_QSCROLLBAR_VERTICAL \
"QScrollBar:vertical{width:40px;background:#ffffff;margin:0px,0px,0px,0px;padding-top:0px;padding-bottom:0px;}"\
"QScrollBar::handle:vertical{width:40px;background:lightgray ;border-radius:0px;min-height:100px;}"\
"QScrollBar::handle:vertical:hover{width:40px;background:gray;border-radius:0px;min-height:100px;}"\
"QScrollBar::add-line:vertical{height:0px;width:40px;background-color:#FF83FA;subcontrol-position:bottom;}"\
"QScrollBar::sub-line:vertical{height:0px;width:40px;background-color:#FF83FA;subcontrol-position:top;}"\
"QScrollBar::add-line:vertical:hover{height:0px;width:40px;background-color:#FF83FA;subcontrol-position:bottom;}"\
"QScrollBar::sub-line:vertical:hover{height:0px;width:40px;background-color:#FF83FA;subcontrol-position:top;}"\
"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:#ffffff;}"

    m_scrollarea_1=new QScrollArea(this);  //新建了一条滚动条
    //m_scrollarea_1->setFocusPolicy(Qt::NoFocus);
    m_scrollarea_1->setFixedHeight(CMyDefineScreen::getInstance()->get_change_factor_y(_height));
    m_scrollarea_1->setStyleSheet("\
                                 QScrollArea{\
                                     border: 0px ;\
                                     background: transparent;\
                                 }\
                                 QScrollBar:vertical {\
                                     border: 0px ;\
                                     background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,\
                                     stop: 0 transparent, stop: 0.8 transparent,stop: 0.81 rgb(73,237,142) ,stop: 1 rgb(73,237,142));\
                                     width: 4px;\
                                     margin: 0px 0px 0px 0px;\
                                 }\
                                 QScrollBar::handle:vertical {\
                                     background-color: rgb(73,237,142);\
                                     width:4px;\
                                 }\
                                 QScrollBar::add-line:vertical {\
                                     width: 0px;\
                                 }\
                                 QScrollBar::sub-line:vertical {\
                                     width: 0px;\
                                 }\
                                 QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\
                                     background: none;\
                                 }"
                                 );
    m_scrollarea_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollarea_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pMainLayout->addWidget(m_scrollarea_1);


    QWidget * pwidget=new QWidget(this);

    pwidget->setAttribute(Qt::WA_StyledBackground);
    pwidget->setObjectName("pwidget");
    pwidget->setStyleSheet("QWidget#pwidget { background: transparent;}");

    m_vboxlayout_widget=new QVBoxLayout(pwidget);
    m_vboxlayout_widget->setContentsMargins(0,0,0,0);
       m_vboxlayout_widget->setAlignment(Qt::AlignTop|Qt::AlignLeft);
       m_vboxlayout_widget->setSpacing(0);


       pwidget->setLayout(m_vboxlayout_widget);
       m_scrollarea_1->setWidget(pwidget);
       m_scrollarea_1->setWidgetResizable(true);

#endif

    //m_table_widget->insertAction();

    //m_define_screen->add_parent(this);


}

bool CNavigateLeft::set_focus_first()
{
    m_scrollarea_1->setFocus();
    return true;
}

void CNavigateLeft::navigate_left_init()
{

    add_button(kEnumRegisterInfo,tr("注册信息"));
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
#else
    //add_button(kEnumCoordinateCollection,tr("坐标采集"));
    add_button(kEnumVideotapeQuery,tr("录像查询"));
    add_button(kEnumCameraConfig,tr("CAM配置"));
    add_button(kEnumPassengerFlow,tr("客流统计"));

#endif

    add_button(kEnumStorageManage,tr("存储管理"));
    add_button(kEnumNetwork,tr("网络"));
    add_button(kEnumSwitchConfig,tr("I/O配置"));
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
#else
    add_button(kEnumSeniorSetUp,tr("高级设置"));
#endif
#if(defined(APPLICATION_TYPE_P2))
    add_button(kEnumPosConfig,tr("POS配置"));
#endif

    add_button(kEnumSystemSetUp,tr("系统设置"));
    add_button(kEnumSystemMaintain,tr("系统维护"));
    //add_button(kEnumHistoryInfo,tr("历史信息"));

}

void CNavigateLeft::add_button(const int _id,const QString& _name)
{
    //int row = 1;
    if(!m_pButtonMap.contains(_id))
    {

        NavigateLeftButton* b=new NavigateLeftButton(m_button_width,m_button_height,NavigateLeftButton::NavigateLeftMode);
        //NavigateLeftButton* b=new NavigateLeftButton(m_button_width,m_button_height,NavigateLeftButton::NavigateLeftMode);
                switch (_id)
        {
            case kEnumRegisterInfo:          //注册信息
            case kEnumCoordinateCollection:  //坐标采集
            case kEnumVideotapeQuery:        //录像查询
            case kEnumCameraConfig:          //摄像机配置
            case kEnumPassengerFlow:         //客流统计
            case kEnumStorageManage:         //存储管理
            case kEnumNetwork:               //网络
            case kEnumSwitchConfig:          //开关量配置
            case kEnumSeniorSetUp:           //高级设置
            case kEnumPosConfig:
            case kEnumSystemSetUp:           //系统设置
            case kEnumSystemMaintain:        //系统维护
            case kEnumHistoryInfo:            //历史信息
            case kEnumAbountMachine:         //关于本机
            case kEnumManufactorInfo:        //厂家信息
            case kEnumMobileNetwork:         //移动网络
            case kEnumSwitchStatus:          //开关量
            case kEnumLocationModular:       //定位模块
            case kEnumLocalNetwork:          //本地网络
            case kEnumDeviceStatus:          //设备
            case kEnumStationInfo:           //站点信息
            case kEnumPeripheral:
            case kEnumBaseSet:               //基本设置
            case kEnumSeniorSet:             //高级设置
            case kEnumPresetEvent:           //预设事件
            case kEnumImportantMessage:      //重要短信
            case kEnumNormalMessage:         //普通短信
            case kEnumVehiclePlan:           //行车计划
            {
                b->set_dispersed_align(true);
                b->setNormalIconStr(":/img/res/main_page/menu/system_manage/nav_left_button_default.png");
                b->setPressedIconStr(":/img/res/main_page/menu/system_manage/nav_left_button_selected.png");
                b->setActiveIconStr(":/img/res/main_page/menu/system_manage/nav_left_button_selected.png");
            }break;
            default:
                break;
        }
        b->setDisplayText(_name);

        b->setSelected(false, true);

        connect(b,SIGNAL(clicked()),SLOT(slotButtonClicked()));


        //b->setFocusPolicy(Qt::NoFocus);

        //m_vboxlayout_widget->insertWidget(m_vboxlayout_widget->count() - 1, b);
        m_vboxlayout_widget->addWidget( b);

        m_pButtonMap.insert(_id,b);


    }

}
void CNavigateLeft::slotButtonClicked()
{
    int index= m_pButtonMap.key((NavigateLeftButton*)sender(),-2);
    this->setCurrentType(index);
}
void CNavigateLeft::setCurrentType(const int index)
{
    QString text;
    if(m_CurrentId!=index)
    {
        QMapIterator<int, NavigateLeftButton*> i(m_pButtonMap);
        while (i.hasNext()) {
            i.next();
            if(i.key()==index)
            {
                text = i.value()->getButtonText();
                i.value()->setSelected(true);

            }else
            {
                i.value()->setSelected(false);
            }
        }
        m_CurrentId=index;
        emit signalCurrentClassificationChanged(index,text);
    }
}
//QScrollArea::verticalScrollBar()->setValue(QScrollArea::verticalScrollBar()->maximum());
#include <QScrollBar>
#include <QAbstractScrollArea>

void CNavigateLeft::set_scroll_value(bool _max)
{

    if(_max)
    {
                    int page_size = m_scrollarea_1->verticalScrollBar()->pageStep();
            int current_value = m_scrollarea_1->verticalScrollBar()->value();
            //int maximum = m_scrollarea_1->verticalScrollBar()->maximum();
            //int minimum = m_scrollarea_1->verticalScrollBar()->minimum();

            m_scrollarea_1->verticalScrollBar()->setValue(current_value + page_size);




            //m_scrollarea_1->verticalScrollBar()->setValue(m_scrollarea_1->verticalScrollBar()->maximum());
    }
    else
    {

        int page_size = m_scrollarea_1->verticalScrollBar()->pageStep();
        int current_value = m_scrollarea_1->verticalScrollBar()->value();

        m_scrollarea_1->verticalScrollBar()->setValue(current_value - page_size);
        //m_scrollarea_1->verticalScrollBar()->setValue(m_scrollarea_1->verticalScrollBar()->minimum());
    }

}

void CNavigateLeft::slotSetDefaultSelected(int _id)
{
    if ((_id > kEnumListMin) && (_id < kEnumListMax - 1))
    {
        setCurrentType(_id);
    }

}

void CNavigateLeft::get_first_last_button()
{
    QMapIterator<int, NavigateLeftButton*> i(m_pButtonMap);
    char button_index = 0;
    char button_count = m_pButtonMap.size();

    while (i.hasNext()) {
        i.next();

        if(0 == button_index)
        {
            m_first_button = i.value();
            m_first_type = i.key();

        }
        else if((button_count - 1) == button_index)
        {
            m_last_button = i.value();
            m_last_type = i.key();
        }
        button_index++;
    }
}


bool CNavigateLeft::get_active_button(char &_type,bool _down)
{
    bool ret_value = false;
    QMapIterator<int, NavigateLeftButton*> i(m_pButtonMap);
    int button_index = 0;
    NavigateLeftButton *first_button = NULL;
    char first_type = 0;

    NavigateLeftButton *prev_button = NULL;
    char prev_type = 0;

    while (i.hasNext()) {
        i.next();

        if(NavigateLeftButton::kEnumActive == i.value()->get_button_status())
        {
            i.value()->updateNormalStyleSheet();
            if(_down)
            {
                if(i.hasNext())
                {
                    i.next();
                    if(NavigateLeftButton::kEnumPressed != i.value()->get_button_status())
                        i.value()->updateActiveStyleSheet();
                    _type = i.key();

                }
                else
                {
                    if(NavigateLeftButton::kEnumPressed != first_button->get_button_status())
                        first_button->updateActiveStyleSheet();
                    _type = first_type;
                }
            }
            else
            {
                if(prev_button)
                {
                    if(NavigateLeftButton::kEnumPressed != prev_button->get_button_status())
                        prev_button->updateActiveStyleSheet();
                    _type = prev_type;
                }
                else
                {
                    if(NavigateLeftButton::kEnumPressed != m_last_button->get_button_status())
                        m_last_button->updateActiveStyleSheet();
                    _type = m_last_type;
                }
            }

            ret_value = true;
            break;
        }
        if(0 == button_index)
        {
            first_button = i.value();
            first_type = i.key();
        }

        prev_button = i.value();
        prev_type = i.key();

        button_index++;
    }
    return ret_value;
}

bool CNavigateLeft::get_pressed_button(char &_type,bool _down)
{
    bool ret_value = false;
    QMapIterator<int, NavigateLeftButton*> i(m_pButtonMap);
    int button_index = 0;
    NavigateLeftButton *first_button = NULL;
    char first_type = 0;
    NavigateLeftButton *prev_button = NULL;
    char prev_type = 0;

    while (i.hasNext()) {
        i.next();

        if(NavigateLeftButton::kEnumPressed == i.value()->get_button_status())
        {
            if(_down)
            {
                if(i.hasNext())
                {
                    i.next();
                    i.value()->updateActiveStyleSheet();
                    _type = i.key();
                }
                else
                {
                    first_button->updateActiveStyleSheet();
                    _type = first_type;
                }
            }
            else
            {
                if(prev_button)
                {
                    if(NavigateLeftButton::kEnumPressed != prev_button->get_button_status())
                        prev_button->updateActiveStyleSheet();
                    _type = prev_type;
                }
                else
                {
                    if(NavigateLeftButton::kEnumPressed != m_last_button->get_button_status())
                        m_last_button->updateActiveStyleSheet();
                    _type = m_last_type;
                }
            }

            ret_value = true;
            break;
        }
        if(0 == button_index)
        {
            first_button = i.value();
            first_type = i.key();
        }

        prev_button = i.value();
        prev_type = i.key();

        button_index++;
    }
    return ret_value;
}
bool CNavigateLeft::active_button_clicked()
{
    bool ret_value = false;
    QMapIterator<int, NavigateLeftButton*> i(m_pButtonMap);

    while (i.hasNext()) {
        i.next();

        if(NavigateLeftButton::kEnumActive == i.value()->get_button_status())
        {
            i.value()->click();
            ret_value = true;
            break;
        }

    }
    return ret_value;
}

void CNavigateLeft::navigate_up_down(bool _flag)
{
    //int index = 0;
    //NavigateLeftButton *pButton = NULL;
    char button_type = 0;

    get_first_last_button();

    if(_flag)
    {
        if(get_active_button(button_type,false))
         {
             if(button_type >= kEnumNetwork)
             {
                set_scroll_value(true);
             }
             else
             {
                set_scroll_value(false);
             }
         }
         else if(get_pressed_button(button_type,false))
         {
             if(button_type >= kEnumNetwork)
             {
                set_scroll_value(true);
             }
             else
             {
                set_scroll_value(false);
             }
         }
    }
    else
    {
         if(get_active_button(button_type))
         {
             if(button_type >= kEnumNetwork)
             {
                set_scroll_value(true);
             }
             else
             {
                set_scroll_value(false);
             }
         }
         else if(get_pressed_button(button_type))
         {
             if(button_type >= kEnumNetwork)
             {
                set_scroll_value(true);
             }
             else
             {
                set_scroll_value(false);
             }
         }

    }
}










