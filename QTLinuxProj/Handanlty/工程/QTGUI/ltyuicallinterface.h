#ifndef LTYUICALLINTERFACE_H
#define LTYUICALLINTERFACE_H

#include <stdio.h>
#include "cJSON.h"

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
int lytTouchEvent(void *_param);



#endif // LTYUICALLINTERFACE_H
