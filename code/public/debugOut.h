#ifndef _INCLUDED_DEBUG_OUT_H
#define _INCLUDED_DEBUG_OUT_H

#include <stdio.h>

#define STR_SPLIT_LINE1	"=========================================================="
#define STR_SPLIT_LINE2	"----------------------------------------------------------"
#define STR_SPLIT_LINE3	"***************************"

typedef enum {
	PL_INVALID = 0,
	PL_INFOR,
	PL_WARNING,
	PL_ERROR,
	PL_DEBUG
}EPrintLevel;


#define PRINT_INFOR_LOCATION(str, str1)		printf("[Info][%s-%d]%s %s\r\n", __FUNCTION__, __LINE__, str, str1)
#define PRINT_ERR_LOCATION(str, str1)		printf("[Err][%s-%d]%s %s\r\n", __FUNCTION__, __LINE__, str, str1)
#define PRINT_INFOR(...)		print2debug(PL_INFOR, 1, __VA_ARGS__)
#define PRINT_ERROR(...)		print2debug(PL_ERROR, 1, __VA_ARGS__)
#define PRINT_WARNING(...)		print2debug(PL_WARNING, 1, __VA_ARGS__)
#define PRINT_FATAL_ERROR(...)	printf(__VA_ARGS__)

#define PRINT_INFORBYS(...)		print2debugBys(PL_INFOR, 1, __VA_ARGS__)
#define PRINT_ERRORBYS(...)		print2debugBys(PL_ERROR, 1, __VA_ARGS__)
#define PRINT_WARNINGBYS(...)	print2debugBys(PL_WARNING, 1, __VA_ARGS__)

#if SWITCH_DEBUG_PRJ
#define PRINT_DEBUG(...)		print2debug(PL_DEBUG, 1, __VA_ARGS__)
#define PRINT_DEBUGBYS(...)		print2debugBys(PL_DEBUG, 1, __VA_ARGS__)
#define PRINT_DEBUG_LOCATION(str, str1)		printf("[%s-%d]%s %s\r\n", __FUNCTION__, __LINE__, str, str1)
#else
#define PRINT_DEBUG(...)		//printf(__VA_ARGS__)
#define PRINT_DEBUGBYS(...)		//print2debug(PL_DEBUG, 1, __VA_ARGS__)
#define PRINT_DEBUG_LOCATION(str, str1)		//printf("[%s-%d]%s %s\r\n", __FUNCTION__, __LINE__, str, str1)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @Description - printf to debug port(console or debug COM)
	 *
	 * @Param printLevel(int) - level of print(enum EPrintLevel)
	 * @Param pMsg(char *) - buffer of message
	 * @Param line(int) - position of font (emFontPostion)
	 *
	 * @Return
	 */
	extern void print2debug(TINT32 printLevel, TINT32 line, void *pMsg, ...);
	extern void print2debugBys(TINT32 printLevel, TINT32 line, void *pMsg, void *pBuf, int bLen);


#ifdef __cplusplus
};
#endif


#endif



