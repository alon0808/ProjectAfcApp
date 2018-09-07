#ifndef _INCLUDE_LIB_HANDANCORE_H_
#define _INCLUDE_LIB_HANDANCORE_H_

#include "DllDef.h"

#define LEN_DEV_ID	9
#define LEN_LINE_ID	3
#define LEN_MESSAGE	50

typedef struct {
	char message[LEN_MESSAGE + 1];
	unsigned char isNeedUpdate;
	unsigned char isGJOk;
	unsigned char isDDOk;
	unsigned char isGpsOk;
	unsigned char stopflag;
	unsigned char  lineId[LEN_LINE_ID];
	char devId[LEN_DEV_ID];
	int task;
	int linkStatus;
	int uploadRec;
	int version;
	int modVer;
	int basePrice;
	int delayTime;		// delay time for user interface refresh

}stUIData;

#ifdef __cplusplus
extern "C" {
#endif

	extern AFC_CORE__API stUIData *GetStatusData(void);

	extern AFC_CORE__API void* StartApp(void *argv);


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
