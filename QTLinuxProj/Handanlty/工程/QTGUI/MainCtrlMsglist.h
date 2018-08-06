
#ifndef _MAINCTRLMSGLIST_H_
#define _MAINCTRLMSGLIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>



void MainCtrlMsgListAdd(void* pData,unsigned int len,unsigned int clientAddr,char _type);
unsigned int MainCtrlMsgListGet(void* pData,unsigned int* len,unsigned int* addr, int bBlock,char *_type);
void MainCtrlMsgListClear();
unsigned int MainCtrlMsgListSize();




#endif

