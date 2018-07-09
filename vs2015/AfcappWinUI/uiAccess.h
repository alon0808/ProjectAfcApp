#ifndef _INCLUDE_UIACCESS_H_
#define _INCLUDE_UIACCESS_H_

#if WHICH_PLATFORM == _WIN32_PLATFORM_V


#define SCREEN_ROW			8
#define SCREEN_COLUMN		50
#define SCREEN_COLUMN_ALL	(SCREEN_COLUMN + 2)

#ifndef TYPEDEF_TEXT_POSITION
#define TYPEDEF_TEXT_POSITION
// 文本位置
typedef enum
{
	TP_INVALID = 0,
	TP_LEFT,
	TP_MIDDLE,
	TP_RIGHT
}emTextPostion;
#endif

/*
// 文本位置
typedef enum
{
	TP_INVALID = 0,
	TP_LEFT,
	TP_MIDDLE,
	TP_RIGHT
}emTextPostion;

// 字体颜色
typedef enum
{
	FC_INVALID = 0,
	FC_RED,
	FC_BLACK,
	FC_WHITE,
	FC_BLUE
}emFontColor;*/

extern void showInUIFatalError(char msg[]);


#endif

#endif