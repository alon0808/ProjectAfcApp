#ifndef INCLUDED_TYPE_DEFINE_H
#define INCLUDED_TYPE_DEFINE_H


#ifndef BOOL_TRUE
#define BOOL_TRUE			1
#define BOOL_FALSE			0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif
//Define NULL pointer value
#ifndef NULL
#define NULL    ((void *)0)
#endif


// basic type define
#ifndef TYPE_BASIC_DEFINE
#define TYPE_BASIC_DEFINE

typedef char TBOOL;

typedef char TINT8;

typedef unsigned char TUINT8;

typedef signed short TINT16;

typedef unsigned short TUINT16;

typedef signed int TINT32;

typedef unsigned int TUINT32;

typedef signed long TINT64;

typedef unsigned long TUINT64;

typedef void *TLPVOID;

typedef void *THANDLE;

#endif

typedef unsigned char SM_Module_ID;
typedef unsigned char SM_Signal_ID;

// use for function declare
#ifndef IN
#define IN
#define OUT
#define INOUT
#endif

#ifndef _TYPE_ERet_Code_
#define _TYPE_ERet_Code_
typedef enum {
	Ret_Error = -32768,       // error occur,0x8000
	Ret_Err_Overflow = -32767,  // overflow,0x8001
	Ret_Err_Param = -32766,     // parameter is error ,0x8002
	Ret_Err_Fatal = -32765,     // fatal error,0x8003
	Ret_Err_TimeOut = -32764,   // timeout,0x8004
	Ret_Err_N_Support = -32763, // not support,0x8005
	Ret_Err_Format = -32762,    // error in format,0x8006
	Ret_Err_Repeat = -32761,    // repeat data element,0x8007
	Ret_Err_N_Exist = -32760,   // not exist,0x8008
	Ret_OK = 0,            // successful,0x0000
	Ret_NO = 0,            // ,0x0000
	Ret_YES = 1,            // YES,or NO,0x0001
	Ret_Continue = 2,           // more operators need to be done, some special situation,0x0002
}ERet_Code;
#endif


typedef struct
{
	TUINT32 tag;
	TUINT16 len;
	/*@null@*/
	/*@keep@*/TUINT8 * pValue;
	TUINT16 maxSize;
	TUINT8 isPresent;
}TLV_PTR;


typedef struct  {
	short tm_sec;     /* seconds after the minute - [0,59] */
	short tm_min;     /* minutes after the hour - [0,59] */
	short tm_hour;    /* hours since midnight - [0,23] */
	short tm_mday;    /* day of the month - [1,31] */
	short tm_mon;     /* months since January - [0,11] */
	short tm_year;    /* years since 1900 */
	short tm_wday;    /* days since Sunday - [0,6] */
	short tm_yday;    /* days since January 1 - [0,365] */
	short tm_isdst;   /* daylight savings time flag */
}tm_slzr;

#if !defined __time_t_defined
# define __time_t_defined	1

# include <bits/types.h>

__BEGIN_NAMESPACE_STD
/* Returned by `time'.  */
typedef __time_t time_t;
__END_NAMESPACE_STD
#if defined __USE_POSIX || defined __USE_MISC || defined __USE_SVID
__USING_NAMESPACE_STD(time_t)
#endif

#endif /* time_t not defined and <time.h> or need time_t.  */
#undef	__need_time_t

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
};
#endif

#endif


