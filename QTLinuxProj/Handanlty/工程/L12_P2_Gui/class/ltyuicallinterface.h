#ifndef LTYUICALLINTERFACE_H
#define LTYUICALLINTERFACE_H

#include <stdio.h>
#include "cJSON.h"

typedef unsigned short LTY_U16;

typedef struct ltyTOUCH_ATTR_S{
    LTY_U16   x;
    LTY_U16   y;
    LTY_U16  up;

}TOUCH_ATTR_S;

enum
{
    kEnumUiMessageTouch, //触摸屏消息
    kEnumUiMessageKeypad,//按键
    kEnumUiMessageOther,  //其它消息
    kEnumUiMessageCallBack//设置回调
};

typedef int  (*ltySendMessge2UiCallback_)(cJSON *);
int lytSendMsgToUiAppCall(cJSON *root, char _type,ltySendMessge2UiCallback_ sendMsgCallBack);
int lytUiappStart();
int gui_send_message2system(cJSON *root);
int lytTouchEvent(void *_param);

#endif // LTYUICALLINTERFACE_H
