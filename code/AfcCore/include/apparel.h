#ifndef _APPAREL_H_
#define _APPAREL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
//#include <popt.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>

/*
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
//#include <minigui/dti.c>
#include <minigui/mgext.h>
*/


#define		VERSION_NUMB	"LTY-CardRead-17.06.13.01"
//#define		VERSION_NUMB	"V1.0"




/* 是否需要刷卡不成功，重新刷卡*/
#define		NO_RESWIPING_CARD
#undef		NO_RESWIPING_CARD

// 定义银行卡分段使用的M1起始扇区，总共使用2个扇区
#define	PBOC_SECTION_START_SECTOR	3



/* 定义设备类型*/
/*用于游乐场*/
#define		PLAY_GROUND_USE
#undef		PLAY_GROUND_USE


// 定义标准版本
#define		STANDARD_VERSION	1

#define	  	SICHUAN_BAZHONG		0

// 测试用户
#define		TEST_USER			0

#ifndef u8
typedef unsigned char		u8;
#endif
#ifndef u16
typedef unsigned short		u16;
#endif
#ifndef u32
typedef unsigned long		u32;
#endif



#if(STANDARD_VERSION==0)
#undef	STANDARD_VERSION
#endif
#if(TEST_USER==0)
#undef	TEST_USER
#endif
#if(SICHUAN_BAZHONG==0)
#undef	SICHUAN_BAZHONG
#endif




#if (!(defined TEST_USER) && \
	 !(defined STANDARD_VERSION)&&!(defined SICHUAN_BAZHONG))
	 
#error  "!!!!!!!!!!! Please Select One User In includes.h !!!!!!!!!!"
#endif

#if(TEST_USER)
	#define  CONFIG_LINUXBUS8BIT	 //Linux公交8位
	//#define  CONFIG_LINUXBUS32BIT    //Linux公交32位
#else
	#define  CONFIG_BZLINUXBUS    //linux标准公交
#endif



#ifdef		TEST_USER
	#define	 PBOC_SUPPORT_SECTION
	#define		FORCE_SECTION
#endif
#ifdef		STANDARD_VERSION
	//#define	 PBOC_SUPPORT_SECTION
#endif


////////////////////  dev/buzzer  /////
#define        BEEPL                 0
#define        BEEPH                 1

#define        LEDAL                 2
#define        LEDAH                 3

#define        LEDBL                 4
#define        LEDBH                 5

#define        RELAYL                6
#define        RELAYH                7

#define        KEYPOWERL             8
#define        KEYPOWERH             9
///////////////////////////////////////



/*************************************
	HAL   Degfine
*************************************/
#if 1
#define		OFF_LINE_CONSUM_FILE	"/mnt/posdata/cardsave.bin"
#define		OFF_LINE_CONSUMBAK_FILE	"/mnt/posdatabak/cardsave_Bak.bin"
#define		SYS_PARAM_FILE          "/mnt/posdata/system.sys"
#define		LTY_SYS_PARAM_FILE          "/mnt/posdata/ltysystem.sys"
#define		SAVE_CONSUM_DATA_DIRECT
#else
#define		OFF_LINE_CONSUM_FILE	"/var/run/cardsave.bin"
#define		SYS_PARAM_FILE          "/var/run/system.sys"
#endif

extern char* mk_time (char* buff);
extern char  Wr_time (char *dt);
extern int ascii_2_hex(unsigned char *O_data, unsigned char *N_data, int len);
extern unsigned char ReadIPCard (char type);
extern unsigned char ReadKeyCard (void);

extern pthread_mutex_t m_datafile;

/***************  SDcard.c  *****************/
extern int MmcCardDetect(void);
extern int MmcCardClose(void);

extern  void * ReadGprs_Pthread (void * args);
extern  void * Readsql_Pthread (void * args);
extern void * Server_Pthread(void * args);
extern void * Client_Pthread(void * args);

extern unsigned char Error_writelog(const char * format, ...);
#endif
