#ifndef LTYUICALLINTERFACE_H
#define LTYUICALLINTERFACE_H

#include <stdio.h>
#include "cJSON.h"

enum
{
    kEnumUiMessageTouch, //��������Ϣ
    kEnumUiMessageKeypad,//���� 
    kEnumUiMessageOther,  //������Ϣ
    kEnumUiMessageCallBack//���ûص�
};
typedef int  (*ltySendMessge2UiCallback_)(cJSON *);
int lytSendMsgToUiAppCall(cJSON *root, char _type,ltySendMessge2UiCallback_ sendMsgCallBack);
int lytUiappStart();
int lytTouchEvent(void *_param);



#endif // LTYUICALLINTERFACE_H
