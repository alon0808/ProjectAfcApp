#ifndef _INCLUDE_MACRO_PROJ_H_
#define _INCLUDE_MACRO_PROJ_H_

#include "TypeDef.h"
#include "SlzrTypeDef.h"

// some value for system
// software platform system
#define _WIN32_PLATFORM_V	1
#define _LINUX_PLATFORM_V	2
#define _STM32_PLATFORM_V	3

// hardware platform system
#define _ACR322_DEV_V		1
#define _3000E_DEV_V		2

#define REF_INVALID			-1

///////////////////////////////////////////////////
// Æ½Ì¨, Èç linux,windows,stm32
#ifdef _WIN32
#define WHICH_PLATFORM	_WIN32_PLATFORM_V
#else
#ifdef _linux_
#define WHICH_PLATFORM	_LINUX_PLATFORM_V
#else
#error ("None of platform is supported")
#endif
#endif

#if WHICH_PLATFORM == _WIN32_PLATFORM_V
//#pragma pack(4)
//#pragma warning(disable: 4103)
//'_open': This function or variable may be unsafe.Consider using _sopen_s instead.To disable deprecation, use _CRT_SECURE_NO_WARNINGS.See online help for details.SystemRelation
#pragma warning(disable: 4996)
#endif

#define WHICH_DEV	_ACR322_DEV_V

#define SWITCH_CODE_USED	0

#define SWITCH_DEBUG_PRJ	1

#if SWITCH_DEBUG_PRJ
#define SWITCH_DEBUG_PRICE		1	// transaction price force 0.01 yuan
#define SWITCH_DEBUG_UNIONPAY	2	// use HanDan test parameter
#define debug_GJ_TLVDeal_	1
#define SWITCH_DEBUG_GPS	1
#define SWITCH_DEBUG_UI		0
#define SWITCH_DEBUG_DATA	1
#define SWITCH_DEBUG_APDU	1
#define SWITCH_TEST_SHUAKA	1
#else
#define SWITCH_DEBUG_PRICE	0	// use GuiLin test parameter
#define SWITCH_DEBUG_UNIONPAY	0
#define SWITCH_DEBUG_GPS	0
#define SWITCH_DEBUG_UI		0
#define SWITCH_DEBUG_DATA	0
#define SWITCH_DEBUG_APDU	0
#define SWITCH_TEST_SHUAKA	0
#endif

#define CALL_CONVERT_STD	__stdcall

// for thread
typedef enum {
	TS_Invalid = 0,
	TS_Idle,
	TS_Running,
	TS_Terminate
}emThreadStatus;	// thread status

// bit operator
#ifndef _BITS8_ATTR_
#define _BITS8_ATTR_
typedef struct {
	unsigned char bit0 : 1;	//111
	unsigned char bit1 : 1;
	unsigned char bit2 : 1;
	unsigned char bit3 : 1;
	unsigned char bit4 : 1;
	unsigned char bit5 : 1;
	unsigned char bit6 : 1;
	unsigned char bit7 : 1;
}stBits8;

typedef union {
	unsigned char dataCh;
	stBits8 bits;
}unBits8;
#endif

typedef struct {
	unsigned char *pBuffer;
	int blen;
}stDataBuffer;

#ifndef BYTE_ORDER
# define LITTLE_ENDIAN	1234
# define BIG_ENDIAN	4321
// you must be declare int a; before use if use this
#define INIT_BYTE_ORDER	int a;
#define BYTE_ORDER (a=1,*(unsigned char *)&a ? LITTLE_ENDIAN : BIG_ENDIAN)
#endif

// function, little endian
#define SET_INT16(buf, len)	(buf)[0] = ((len) >> 8) & 0x0FF;\
							(buf)[1] = ((len)) & 0x0FF
#define GET_INT16(buf) ((buf)[0]<<8)|((buf)[1]&0x0FF)
#define GET_INT16S(buf) ((buf)[1]<<8)|((buf)[0]&0x0FF)
//
#define SET_INT32(buf, len)	(buf)[0] = ((len) >> 24) & 0x0FF;\
							(buf)[1] = ((len) >> 16) & 0x0FF;\
							(buf)[2] = ((len) >> 8) & 0x0FF;\
							(buf)[3] = ((len) ) & 0x0FF
#define GET_INT32(buf)	(((buf)[0]<<24)|((buf)[1]<<16|(buf)[2]<<8)|((buf)[3]&0x0FF))
#define GET_INT32S(buf) (((buf)[3]<<24)|((buf)[2]<<16|(buf)[1]<<8)|((buf)[0]&0x0FF))

#define LOOP_FOREVER		while(1);

#ifndef UNUSED_VAR
#define UNUSED_VAR(var1)	if((var1) == (var1)){}
#endif	

////////////////////////////////////////////////////////////////////////////////
// constant variable
#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

#define STR_NEW_LINE	"<br/>"


#include "debugOut.h"
#include "debugInfor.h"
#include "UtilityProc.h"

#include <unistd.h>

#endif

