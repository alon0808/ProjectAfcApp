#ifndef __SYSDESK_H__
#define __SYSDESK_H__

#include "LtyMsgItsSD.h"
//#include "hi_type.h"

typedef struct ltyPOP_MSG_S{
    const char* msg;
    int         bconfirm;  //是否需要确认
    LTY_U32     breponse;
    LTY_U32     u32MsgSN;  //手动确认时需要这个东西 
}POP_MSG_S;
#define ALARM_BUSSTATUS  	0
#define ALARM_VDT			1
#define ALARM_DRIVEREMERG	2
#define ALARM_FRONT_PASSENGERHELP	3
#define ALARM_BACK_PASSENGERHELP	4
#define ALARM_SPEEDBEYOND			5
#define ALARM_QUICKSPEED			6
#define ALARM_TEMPATRUER			7
#define ALARM_OIL					8
#define ALARM_FRONTDOOR				15
#define ALARM_BACKDOOR				16
#define ALARM_COINCASE				17
#define ALARM_WINDOW				18
#define ALARM_HARDDISK				25
#define ALARM_OFFLINE				26
#define ALARM_DELAYATSTOP 			27
#define ALARM_OVERTAKESTATION		28

class SysDesk{
public:
    SysDesk();
    ~SysDesk();

public:
    LTY_S32 RegDeskMsg();
    LTY_RESULT MsgHandle(LTY_MSG_TYPE msg, LTY_U32 chan, LTY_U32 buf, 
              LTY_U32 bufsize, LTY_LPVOID lpPriv);
	void station_info(LTY_U32 buf,LTY_U32 _type);
private:
	LTY_U32 m_warnflag;
	LTY_U32 m_curspeed;
    LTY_U32 m_curlimspeed;
};

#ifdef SYSDESK_TEST
int  sysdesk_test_init();
#endif
#endif



