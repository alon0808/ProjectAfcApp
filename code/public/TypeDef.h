#ifndef INCLUDED_TYPE_DEFINE_H
#define INCLUDED_TYPE_DEFINE_H


#ifdef __cplusplus
extern "C"
{
#endif

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
		Ret_Error = -32768,	// error occur
		Ret_Err_Overflow = -32767,	// overflow
		Ret_Err_Param = -32766,	// parameter is error 
		Ret_Err_Fatal = -32765,	// fatal error
		Ret_Err_TimeOut = -32764,	// timeout
		Ret_Err_N_Support = -32763,	// not support
		Ret_OK = 0,	// successful
		Ret_Continue = 1,	// more operators need to be done, some special situation
	}ERet_Code;	// return code
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


#ifdef __cplusplus
};
#endif

#endif


