#ifndef LTYCOMMONSTYLE_H
#define LTYCOMMONSTYLE_H


#define LTY_PUSHBUTTON_STYLE_SAVE    "QPushButton {\
                                      border:0px;\
                                      background:#fa5135;\
                                      font:30px;\
                                      border-radius: 32px;\
                                      color:#ffffff;\
                                      } \
                                      QPushButton:hover:pressed {background:#fa5135;} \
                                      QPushButton:hover:!pressed {background:#23ad63;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}"


#define LTY_PUSHBUTTON_STYLE_RESET    "QPushButton {\
                                      border:2px solid #41a6ec;\
                                      background:#ffffff;\
                                      font:30px;\
                                      border-radius: 32px;\
                                      color:#41a6ec;\
                                      } \
                                      QPushButton:hover:pressed {background:#ffffff;} \
                                      QPushButton:hover:!pressed {background:#ffffff;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}"

#define LTY_PUSHBUTTON_HELP    "QPushButton {\
                                      border:0px;\
                                      background:transparent;\
                                      border-image:url(:/img/res/main_page/menu/system_manage/button_help_default.png);\
                                      font:34px;\
                                      color:#80e3ff;\
                                      } \
                                      QPushButton:pressed {border-image:url(:/img/res/main_page/menu/system_manage/button_help_selected.png);} \
                                      QPushButton:hover:!pressed {border-image:url(:/img/res/main_page/menu/system_manage/button_help_selected.png);}"


#define LTY_PUSHBUTTON_OK    "QPushButton {\
                                      border:0px;\
                                      background:transparent;\
                                      border-image:url(:/img/res/main_page/menu/system_manage/button_confirm_default.png);\
                                      font:34px;\
                                      color:#80e3ff;\
                                      } \
                                      QPushButton:pressed {border-image:url(:/img/res/main_page/menu/system_manage/button_confirm_selected.png);} \
                                      QPushButton:hover:!pressed {border-image:url(:/img/res/main_page/menu/system_manage/button_confirm_selected.png);}"

    #define LTY_PUSHBUTTON_CONFIRM    "QPushButton {\
                                      border:0px;\
                                      background:transparent;\
                                      border-image:url(:/img_p2/res/start/button_confirm_default.png);\
                                      font:19px;\
                                      color:#80e3ff;\
                                      } \
                                      QPushButton:pressed {border-image:url(:/img_p2/res/start/button_confirm_selected.png);} \
                                      QPushButton:hover:!pressed {border-image:url(:/img_p2/res/start/button_confirm_selected.png);}"
  #define LTY_PUSHBUTTON_FINISH   "QPushButton {\
                                      border:0px;\
                                      background:transparent;\
                                      border-image:url(:/img_p2/res/start/button_complete_default.png);\
                                      font:19px;\
                                      color:#80e3ff;\
                                      } \
                                      QPushButton:pressed {border-image:url(:/img_p2/res/start/button_complete_selected.png);} \
                                      QPushButton:hover:!pressed {border-image:url(:/img_p2/res/start/button_complete_selected.png);} \
                                      QPushButton:disabled{border-image:url(:/img_p2/res/start/button_complete_disable.png);}"

#if 0
#define LTY_COMBOBOX_STYLE           "QComboBox { border-image:url(:/img/res/systemManage/eidt_frame_b.png);min-height: 26px; min-width: 100px; }\
    QComboBox QAbstractItemView::item {\
    min-height: 30px; \
    min-width:  100px;\
}\
\
QListView::item {\
    background-color: #ffffff;\
    color:#666666;\
    font:12;\
    min-height: 50px; \
}\
\
QListView::item:hover {\
    background-color: #41A6EC;\
}\
QComboBox::down-arrow {\
    border-image:url(:/img/res/systemManage/xljt.png);\
    min-width:26;\
}"
#endif
//系统设置
#define LTY_PUSHBUTTON_STYLE_SETUP    "QPushButton {\
                                      border:0px;\
                                      background:#555857;\
                                      border-radius: 3px;\
                                      color:#ffffff;\
                                      } \
                                      QPushButton:hover:pressed {background:#fa5135;} \
                                      QPushButton:hover:!pressed {background:#23ad63;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}"


//系统维护
#define LTY_PUSHBUTTON_STYLE_MAINTAIN    "QPushButton {\
                                      border:0px;\
                                      background:#555857;\
                                      border-radius: 3px;\
                                      color:#ffffff;\
                                      } \
                                      QPushButton:hover:pressed {background:#fa5135;} \
                                      QPushButton:hover:!pressed {background:#23ad63;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}"

//历史信息
#define LTY_PUSHBUTTON_STYLE_HISTORY    "QPushButton {\
                                      border:0px;\
                                      background:#727171;\
                                      border-radius: 3px;\
                                      color:#ffffff;\
                                      } \
                                      QPushButton:hover:pressed {background:#fa5135;} \
                                      QPushButton:hover:!pressed {background:#23ad63;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}"


#define LTY_PUSHBUTTON_STYLE_UP_DOWN    "QPushButton {\
                                      border:1px solid #ffffff;\
                                      background:#000000;\
                                      font:30px;\
                                      border-radius: 2px;\
                                      color:#ffffff;\
                                      } \
                                      QPushButton:hover:pressed {background:#000000;} \
                                      QPushButton:hover:!pressed {background:#000000;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}"

#define LTY_MESSAGEBOX_OK    "QPushButton {\
                                      border:1px solid #3A98EF;\
                                      background:#3ea2ff;\
                                      font:33px;\
                                      border-radius: 0px;\
                                      color:#03111f;\
                                      } \
                                      QPushButton:hover:pressed {background:#000000;color:#ffffff;} \
                                      QPushButton:hover:!pressed {background:#000000;color:#ffffff;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}"

#define LTY_MESSAGEBOX_CANCEL    "QPushButton {\
                                      border:1px solid #3A98EF;\
                                      background:#03111E;\
                                      font:33px;\
                                      border-radius: 0px;\
                                      color:#3EA1FF;\
                                      } \
                                      QPushButton:hover:pressed {background:#000000;color:#ffffff;} \
                                      QPushButton:hover:!pressed {background:#000000;color:#ffffff;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}"

#define LTY_QSCROLLBAR_RIGHT_VERTICAL \
"QScrollArea{border: 0px solid #275ba7;background:transparent;}"\
"QScrollBar:vertical{width:50px;background:transparent;margin:%1px,59px,0px,0px;padding-top:50px;padding-bottom:50px;}"\
"QScrollBar::handle:vertical{width:50px;border-image:url(:/img/res/main_page/menu/system_manage/content_scrollbar.png);border-radius:5px;min-height:100px;}"\
"QScrollBar::handle:vertical:hover{width:50px;border-image:url(:/img/res/main_page/menu/system_manage/content_scrollbar.png);border-radius:5px;min-height:100px;}"\
"QScrollBar::add-line:vertical{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/content_down_default.png);subcontrol-position:bottom;}"\
"QScrollBar::sub-line:vertical{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/content_up_default.png);subcontrol-position:top;}"\
"QScrollBar::add-line:vertical:hover{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/content_down_selected.png);subcontrol-position:bottom;}"\
"QScrollBar::sub-line:vertical:hover{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/content_up_selected.png);subcontrol-position:top;}"\
"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:transparent;}"

///////////////////////////////////////////////////////////////

#define BUTTON_CONFIG_SHEET "QPushButton {\
                                      border:1px solid #24508F;\
                                      background:#0F1D31;\
                                      font:30px;\
                                      border-radius: 3px;\
                                      color:#7CA7D8;\
                                      } \
                                      QPushButton:pressed {background:#3EA2FF;color:#000000;} \
                                      QPushButton:hover:!pressed {background:#3EA2FF;color:#000000;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}"


#define CHECK_BOX_SHEET "\
                         QCheckBox{\
                         background:transparent;\
                         }\
                         QCheckBox::indicator {\
                         width:%1px;\
                         height:%2px;\
                         }\
                         QCheckBox::indicator:unchecked {\
                         border-image: url(:/img/res/main_page/menu/system_manage/storage_manage/Non_check.png);\
                         }\
                         QCheckBox::indicator:checked {\
                         border-image: url(:/img/res/main_page/menu/system_manage/storage_manage/check.png);\
                         }"



#define MENU_STYLE_SHEET "QMenu{\
border:1px solid #e5e5e5;\
background-color:white; \
padding-top:6px;\
padding-bottom:6px;\
padding-left:0px;\
padding-right:0px;\
}\
\
QMenu::item{\
background-color:#ffffff;\
font-size:26px;\
margin:0px;\
padding-top:6px;\
padding-bottom:6px;\
padding-left:23px;\
padding-right:30px;\
color:#494949;\
height:80px;\
}\
\
QMenu::indicator{\
width:30px;\
height:80px;\
}\
\
QMenu::item:hover{\
background:rgb(231,245,253); \
color:#0897f2;\
}\
QMenu::item:selected{\
background:rgb(231,245,251); \
color:#0897f2;\
}\
\
QMenu:item:disabled{color:#999999; background:rgb(250,250,250);}\
QMenu::separator{\
height:1px;\
background-color:#e5e5e5;  \
margin:0px 1px 0px 1px;\
}"


#define VIDEO_BUTTON_SHEET_TRANSPARENT "QPushButton{border:0px;background:transparent;}"

 #define BUTTON_SELECT   "QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/check.png);}"
 #define BUTTON_SELECT_NO   "QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/Non_check.png);}"


#endif // LTYCOMMONSTYLE_H
