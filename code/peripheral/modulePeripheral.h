#ifndef _INCLUDED_MODULE_PERIPHERAL_H
#define _INCLUDED_MODULE_PERIPHERAL_H

typedef enum {
	PT_Invalid = 0,
	PT_Display,
	PT_Audio
}emPeripheralType;

#ifndef TYPEDEF_TEXT_POSITION
#define TYPEDEF_TEXT_POSITION
// ÎÄ±¾Î»ÖÃ
typedef enum
{
	TP_INVALID = 0,
	TP_LEFT,
	TP_MIDDLE,
	TP_RIGHT
}emTextPostion;
#endif

#ifndef defFuncShowInUI
#define defFuncShowInUI
typedef void(*TFuncShowInUI)(int row, int column, char msg[], int textPosition);
#endif

#ifndef _MODULE_PERIPHERAL_N_FUNCTION_C

#ifdef __cplusplus
extern "C"
{
#endif

	extern int initPeripheral(char curPath[], TFuncShowInUI funcShowInUi);

	extern int sendDisplayMsg(int row, int column, char *pMsg, int textPosition);

	extern int feedWatchDog(void);

#ifdef __cplusplus
}
#endif

#endif
#endif
