#ifndef _INCLUDE_LIB_HANDANCORE_H_
#define _INCLUDE_LIB_HANDANCORE_H_

#include "DllDef.h"

#define LEN_DEV_ID	9
#define LEN_LINE_ID	3
#define LEN_MESSAGE	100

#ifndef SLZRKEY_UP
#define SLZRKEY_UP		115	// Arrow Up
#define SLZRKEY_DOWN	114	// Arrow Down
#define SLZRKEY_ESC		28		// Esc
#define SLZRKEY_ENTER	139		// Enter

#define SLZRKEY_F1		KEY_F1	// Function
#define SLZRKEY_F2		KEY_F2
#endif

typedef enum {
	dpt_invalid = 0,
	dpt_devId,
	dpt_unionpayTerId,
	dpt_unionpayDownKey,
}emDevParamType;

typedef struct {
	unsigned char ud_isNeedUpdate;
	unsigned char ud_isGJOk;
	unsigned char ud_isDDOk;
	unsigned char ud_isUnpayOk;
	unsigned char ud_isGpsOk;
	unsigned char ud_stopflag;
	unsigned char  ud_lineId[LEN_LINE_ID];
	char ud_devId[LEN_DEV_ID];
	char ud_unionpayDevId[LEN_DEV_ID];
	int ud_task;
	int ud_linkStatus;
	int ud_uploadRec;
	int ud_version;
	int ud_modVer;
	int ud_basePrice;
	int ud_delayTime;		// delay time for user interface refresh

}stUIData;

#ifdef __cplusplus
extern "C" {
#endif

	extern AFC_CORE__API stUIData *GetStatusData(int timerTrige);

	extern AFC_CORE__API int GetStatusMessage(int timerTrige, char *pMsg, int *pTimeDelay);

	extern AFC_CORE__API void* StartApp(void *argv);

	extern AFC_CORE__API int SetDevParam(int paramType, unsigned char *pData, int dLen);

#ifdef __cplusplus
}
#endif


typedef enum {
	//mi_Invalid = 0,
	mi_Yuan = 0,
	mi_Piao,
	mi_Jia,
	mi_Zheng,
	mi_Zai,
	mi_,
}emMessageId;

#endif
