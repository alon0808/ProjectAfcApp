#ifndef _INCLUDED_APPLICATION_H
#define _INCLUDED_APPLICATION_H

#ifndef defFuncShowInUI
#define defFuncShowInUI
typedef void(*TFuncShowInUI)(int row, int column, char msg[], int textPosition);
#endif

#define THREAD_AppTransaction	1




#ifdef __cplusplus
extern "C"
{
#endif

	extern void InitApp(char curPath[], TFuncShowInUI funcShowInUI);
	/**
	* @Description - start application, start thread
	*
	* @Param curPath(char[]) - current path of the executable binary
	* @Param funcShowInUI(FuncShowInUI) - function of show user interface
	*
	* @Return void
	*/
//	extern void StartApp(void);

#ifdef __cplusplus
};
#endif


#endif



