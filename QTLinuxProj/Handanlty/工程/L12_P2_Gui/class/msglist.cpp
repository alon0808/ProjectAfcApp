#include "msglist.h"
#include "common.h"

#include "ltyuicallinterface.h"
#include "common_interface.h"

//static S_Node* pGuiMsgHead = NULL;
//static S_Node* pGuiMsgTail = NULL;

static S_Node* pTaskMsgHead = NULL;
static S_Node* pTaskMsgTail = NULL;

#define TOUCH_POINT_MAX 50
static TOUCH_ATTR_S gTouchPointAttr[TOUCH_POINT_MAX];
static int g_touch_front = 0;
static int g_touch_rear = 0;

static unsigned char g_msg_count = 0;
extern bool g_touch_release;

pthread_cond_t gui_msg_cond_com1 = 	PTHREAD_COND_INITIALIZER;
pthread_mutex_t gui_msg_mutex_com1 = 	PTHREAD_MUTEX_INITIALIZER;

static bool full_queue()
{
    if(g_touch_front == (g_touch_rear + 1)%TOUCH_POINT_MAX)
        return true;
    else
        return false;
}
/*static*/ bool EmptyQueue()
{
    if(g_touch_front == g_touch_rear)
        return true;
    else
        return false;
}
static bool Enqueue(TOUCH_ATTR_S *_val)
{
    if(full_queue())
    {
        //printf("Enqueue() full_queue()............................");
        return false;
    }
    else
    {
       // Q->pBase[Q->rear]=val;
       // Q->rear=(Q->rear+1)%Q->maxsize;

       if(EmptyQueue())
            g_touch_release = false;

        gTouchPointAttr[g_touch_rear].x = _val->x;
        gTouchPointAttr[g_touch_rear].y = _val->y;
        gTouchPointAttr[g_touch_rear].up = _val->up;

       g_touch_rear = (g_touch_rear + 1) % TOUCH_POINT_MAX;
        return true;
    }
}

bool Dequeue(TOUCH_ATTR_S *_val)
{
    if(EmptyQueue())
    {
        return false;
    }
    else
    {
        //*val=Q->pBase[Q->front];
        _val->x = gTouchPointAttr[g_touch_front].x;
        _val->y = gTouchPointAttr[g_touch_front].y;
        _val->up = gTouchPointAttr[g_touch_front].up;

        g_touch_front = (g_touch_front + 1) % TOUCH_POINT_MAX;
        return true;
    }
}


void init_touch_event()
{
    for(int index = 0;index < TOUCH_POINT_MAX;index++)
    {
        gTouchPointAttr[index].x = 0;
        gTouchPointAttr[index].y = 0;
        gTouchPointAttr[index].up = false;
    }
}


void GuiMsgListAdd(void* pData,unsigned int len,unsigned int clientAddr,char _type)
{

    pthread_mutex_lock(&gui_msg_mutex_com1);


    if(kEnumUiMessageTouch == _type)
    {
        Enqueue((TOUCH_ATTR_S *)pData);
       // printf("GuiMsgListAdd...............kEnumUiMessageTouch  %d\n",ret);
    }
    else
    {
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
    }

    pthread_mutex_unlock(&gui_msg_mutex_com1);
    pthread_cond_signal(&gui_msg_cond_com1);
}

//读头指针
unsigned int GuiMsgListGet(void* pData,unsigned int* len,unsigned int* addr, int bBlock,char *_type)
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
        if(EmptyQueue() && (NULL == pTaskMsgHead))
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
        if(CCommonInterface::notify_ui_printf())
            printf("GuiMsgListGet...............kEnumUiMessageOther \n");

        *addr = temp->addr;
        *_type = temp->_type;
        iRet = 1;

        free(temp);
        temp = NULL;
    }
    else //if(pGuiMsgHead)
	{
        TOUCH_ATTR_S *pTouch = (TOUCH_ATTR_S *)pData;
        bool ret = Dequeue(pTouch);

        if(ret)
        {
            if(CCommonInterface::notify_ui_printf())
            printf("GuiMsgListGet...............kEnumUiMessageTouch %d\n",ret);

            *_type = kEnumUiMessageTouch;
            iRet = 1;
        }
	}

    pthread_mutex_unlock(&gui_msg_mutex_com1);
	
	return iRet;
}

//清空的时候从头部开始删除
void GuiMsgListClear()
{
//	S_Node *temp = 0;

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

unsigned int GuiMsgListSize()
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

bool GuiMsgHasTask()
{
    bool ret_value = false;
    static bool first_run = true;

    pthread_mutex_lock(&gui_msg_mutex_com1);

    if(first_run)
    {
        first_run = false;
        printf("GuiMsgHasTask()\n");
    }

    if(pTaskMsgHead)
        ret_value = true;
    else if(!EmptyQueue())
        ret_value = true;

    pthread_mutex_unlock(&gui_msg_mutex_com1);

    return ret_value;
}








