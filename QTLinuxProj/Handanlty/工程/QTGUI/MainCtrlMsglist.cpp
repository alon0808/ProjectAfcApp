

#include "MainCtrlMsglist.h"


typedef struct _node
{
    //unsigned int len;
    unsigned int addr;  //保存地址
    char _type;         //类型
    //void * content;
    struct _node *next;
}S_Node;


static S_Node* pTaskMsgHead = NULL;
static S_Node* pTaskMsgTail = NULL;


static unsigned char g_msg_count = 0;


static pthread_cond_t gui_msg_cond_com1 = 	PTHREAD_COND_INITIALIZER;
static pthread_mutex_t gui_msg_mutex_com1 = 	PTHREAD_MUTEX_INITIALIZER;




void MainCtrlMsgListAdd(void* pData,unsigned int len,unsigned int clientAddr,char _type)
{

    pthread_mutex_lock(&gui_msg_mutex_com1);


    S_Node *temp = (S_Node*)malloc(sizeof(S_Node));

    if(NULL == temp)
    {
        return;
    }
    temp->addr = (unsigned int)pData;
    temp->_type = _type;
    //temp->len = len;
    //temp->addr = clientAddr;
    temp->next = NULL;
#if 0
    g_msg_count += 1;
#endif

    if(NULL == pTaskMsgHead)
    {
        pTaskMsgHead = pTaskMsgTail = temp;
    }
    else
    {
        pTaskMsgTail->next = temp;
        pTaskMsgTail = temp;
    }
   

    pthread_mutex_unlock(&gui_msg_mutex_com1);
    pthread_cond_signal(&gui_msg_cond_com1);
}


unsigned int MainCtrlMsgListGet(void* pData,unsigned int* len,unsigned int* addr, int bBlock,char *_type)
{
	int retval;
    unsigned int iRet = 0;
   // printf("GuiMsgListGet...............!\n");
    if(/*NULL == pData  ||  NULL == len  ||*/  NULL == addr)
	{
		printf("param is NULL \n");
		return iRet;
	}
	
    pthread_mutex_lock(&gui_msg_mutex_com1);

	if(bBlock) 
	{
		retval = 0;
        if(NULL == pTaskMsgHead)
		{		
            retval = pthread_cond_wait(&gui_msg_cond_com1, &gui_msg_mutex_com1);
		}

		if (retval < 0)
		{
			printf("wait the conditions error.\n");
            pthread_mutex_unlock(&gui_msg_mutex_com1);
			return iRet;
		}
	}
    if(pTaskMsgHead)
    {
       // printf(">>>>>>>>>>>>>>>>>>>>>>>>GuiMsgListGet()  pTaskMsgHead \n");
        S_Node *temp = pTaskMsgHead;

        pTaskMsgHead = pTaskMsgHead->next;

        *addr = temp->addr;
        *_type = temp->_type;
        iRet = 1;

        free(temp);
        temp = NULL;
    }

    pthread_mutex_unlock(&gui_msg_mutex_com1);
	
	return iRet;
}


void MainCtrlMsgListClear()
{
	S_Node *temp = 0;

    pthread_mutex_lock(&gui_msg_mutex_com1);
#if 0
    while(pGuiMsgHead)
	{
        temp = pGuiMsgHead;
        pGuiMsgHead = pGuiMsgHead->next ;
        #if 0
        if(temp->content)
		{
			free(temp->content);
			temp->content = 0;
		}
        #endif
		free(temp);
		temp = 0;
	}
#endif
    pthread_mutex_unlock(&gui_msg_mutex_com1);
}

unsigned int MainCtrlMsgListSize()
{
#if 0
    pthread_mutex_lock(&gui_msg_mutex_com1);
    unsigned int size=0;
    S_Node *temp = pGuiMsgHead;
	
	for(;temp!=0;temp=temp->next)
		size++;

    pthread_mutex_unlock(&gui_msg_mutex_com1);
	return size;
#endif
    pthread_mutex_lock(&gui_msg_mutex_com1);
    unsigned int size=0;

    size = g_msg_count;
    pthread_mutex_unlock(&gui_msg_mutex_com1);

    return size;
}


