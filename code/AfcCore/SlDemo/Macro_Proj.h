#ifndef _INCLUDE_Macro_Proj_LanTai_H_
#define _INCLUDE_Macro_Proj_LanTai_H_


#include <string.h>
#include <stdio.h>

#ifndef _TYPE_ERet_Code_
#define _TYPE_ERet_Code_
typedef enum {
	Ret_Error = -32768,			// error occur,0xFFFFFF8000
	Ret_Err_Overflow = -32767,			// overflow,0xFFFFFF8001
	Ret_Err_Param = -32766,			// parameter is error ,0xFFFFFF8002
	Ret_Err_Fatal = -32765,			// fatal error,0xFFFFFF8003
	Ret_Err_TimeOut = -32764,			// timeout,0xFFFFFF8004
	Ret_Err_N_Support = -32763,			// not support,0xFFFFFF8005
	Ret_Err_Format = -32762,			// ,0xFFFFFF8006
	Ret_Err_Repeat,
	Ret_Err_N_Exist,	//
	Ret_OK = 0,			// successful,0x0
	Ret_NO = 0,			// successful,0x0
	Ret_YES = 1,			// successful,0x0
	Ret_Continue = 1,			// more operators need to be done, some special situation,0x1
}ERet_Code;
#endif

#define MSG_LOG(...)	printf(__VA_ARGS__)

#endif
