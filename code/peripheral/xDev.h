#ifndef _INCLUDED_X_DEV_H
#define _INCLUDED_X_DEV_H

#ifndef defFuncShowInUI
#define defFuncShowInUI
typedef void(*TFuncShowInUI)(int row, int column, char msg[], int textPosition);
#endif

#ifndef _XDEV_N_FUNCTION_C

#ifdef __cplusplus
extern "C"
{
#endif

	extern int xDev_Init(TFuncShowInUI funcShow);

	extern void ShowInUI(int row, int column, char msg[], int textPosition);

#ifdef __cplusplus
};
#endif

#endif

#endif



