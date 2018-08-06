#ifndef MSGLIST_H_
#define MSGLIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

void GuiMsgListAdd(void* pData,unsigned int len,unsigned int clientAddr,char _type);
unsigned int GuiMsgListGet(void* pData,unsigned int* len,unsigned int* addr, int bBlock,char *_type);
void GuiMsgListClear();
unsigned int GuiMsgListSize();
bool GuiMsgHasTask();

void init_touch_event();


#ifdef __cplusplus
}
#endif

#endif

