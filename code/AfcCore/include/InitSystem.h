#ifndef INITSYSTEM_H
#define INITSYSTEM_H

#include  "apparel.h"

/******** printf *******/
#define DEBUG			1
#define DBG_RC500		1
#define DBG_DATA		0
#define DBG_ISO14443	0
#define DBG_ISO7816		0
#define DBG_QPBOC		1
#define	DBG_RF_TRANS_TIME       0


#define  UartDis			0
#define  GPRSPR			0
#define DBG_AUDIO		0

#define VERIFY_PIN		1



//#define	SAVE_CONSUM_DATA_DURECT
//#undef	SAVE_CONSUM_DATA_DURECT

#if DEBUG
#include "stdio.h"
#define DBG_PRINTF					printf
#else
#define DBG_PRINTF(...)  
#endif


#if DBG_RC500
#define DBG_RC500_PRINTF			DBG_PRINTF
#else
#define DBG_RC500_PRINTF(...)  
#endif

#if DBG_AUDIO
#define DBG_AUDIO_PRINTF			DBG_PRINTF
#else
#define DBG_AUDIO_PRINTF(...)  
#endif

#if UartDis
#define DBG_UART_PRINTF			DBG_PRINTF
#else
#define DBG_UART_PRINTF(...)  
#endif

#if DBG_DATA
#define DBG_DATA_PRINTF			DBG_PRINTF
#else
#define DBG_DATA_PRINTF(...)  
#endif

#if DBG_QPBOC
#define DBG_QPBOC_PRINTF			DBG_PRINTF
#else
#define DBG_QPBOC_PRINTF(...)  
#endif

#if(DBG_RF_TRANS_TIME==0)
#undef	DBG_RF_TRANS_TIME
#endif


#define GC_ENTER()					DBG_PRINTF("[%-20s] : Enter...\n", __FUNCTION__)
#define GC_LEAVE()					DBG_PRINTF("[%-20s] : Leave...\n", __FUNCTION__)


// LCD keep display time 3s
#define	LCD_KEEP_DISP_TIME 	(3)


//#define  EXCHANGE   0x80
//#define  EXC6BIT    0x20

#define  HENDDAT    "80"
#define  ENDDATA    "0D"
#define  ST_RX      0x80
#define  ST_END     0x0D
#define  CMDSTART   "DATA:"

#define  PRINT_START     "/var/run/start.txt"
#define  PRINT_END       "/var/run/end.txt"

#define fclose_nosync(x) {fclose(x); /*system("sync");*/}

#define fclose(x) {fclose(x);system("sync;");}



//文件操作状态
#define         SYSFILE                 1
#define         MUESRPWD                2
#define         MSEVERIP                3
#define         MUSERKEY                4
#define         MUSERSETOR              5
#define         OPERADORED              6
#define         MTEMNO                  7
//#define         SETTOPUP              8
#define         SLEEPOVER               9
#define         DEVSERIALID             10
#define         CODEFILE                11
#define         RCODEFILE               12
#define         OPENBUTTON              13
#define         OPERBUFFER              14
//#define         MACMONEY                14
#define         SELFIP                  15



#define         RSDATA                16
#define         WSDATA                17
#define         DRIVER                18

#define         SETSECTION            19
#define         SETCARDFIRST          20
#define         SETSECTIONUP          21
#define         SETSECTINO_           22
//#define         DRIVER                19
#define         RSDATABAK                23
#define         WSDATABAK                24
#define         LTYSYSFILE                25
#define         WBLKVER                26
#define         RBLKVER                27
#define         WDRIVERFLAG       28
#define         RDRIVERFLAG       29

#define  SectionPar   (SYSFILE+1)
#define  SectionParup   (SYSFILE+2)


//################################-----M1卡测试程序-----########################################
//KEYA KEYB
#define		KEYA			0x00
#define		KEYB			0x01

#define 	DO_TYPEA_M1             0xB0
#define		DO_TYPEA_CPU            0xB1
#define		DO_TYPEB_CPU		    0xB4
#define     FIFO_RCV_LEN            0xB3
#define		WRITE_TYPE		        0xB2
//WRITE type
#define	    W_CARD                  0x01
#define 	W_CHAR			        0x02
#define 	W_CPU                   0x03


#define 	RC531_M1_CSN            0xAB
//TYPEA--------------M1 Command
#define		RC531_MFOUTSELECT       0xA0
#define 	RC531_REQIDL            0xA1
#define 	RC531_REQALL            0xA2
#define 	RC531_ANTICOLL          0xA3
#define 	RC531_SELECT            0xA4
#define 	RC531_AUTHENT           0xA5
#define 	RC531_READ              0xA6

#define 	RC531_WRITE             0xA7
#define 	RC531_INC               0xA8
#define 	RC531_DEC               0xA9
#define 	RC531_HALT              0xAA
#define		RC531_TRANSFER          0xC0
#define		RC531_RESTORE           0xC1


//TYPEA--------------FM1208------------------
#define		TYPEA_CPU_REST		    0xAC
#define		TYPEA_CPU_PPS		    0xAD
//TYPEB--------------SFEN------------------
#define		GET_SFEN_CSN	       0xAE

#define 	MI_OK		           0x00
#define 	E_FORBID   0
#define 	E_ALLOW    1

#define		MIFARE_X		        10
#define		MIFARE_Y		        70

/* GPIO group definition */
#define GPIO_OUTPUT		0
#define GPIO_VALUE		1
#define GPIO_GROUP_A 0
#define GPIO_GROUP_B 1
#define GPIO_GROUP_C 2
#define GPIO_GROUP_D 3
#define GPIO_GROUP_E 4

#define w55fa93_setio(group,num,state)	ioctl(bp_fd, GPIO_OUTPUT, ((group << 4) | (num << 8) | state));
#define w55fa93_io(group,num,state)		ioctl(bp_fd, GPIO_VALUE,  ((group << 4) | (num << 8) | state));



/*external muc gpio define*/
#define SPI_SERVER_GPIO_LEDRED		0x01
#define SPI_SERVER_GPIO_LEDGREEN	0x02
//#define SPI_SERVER_GPIO_LEDRED		0x02
//#define SPI_SERVER_GPIO_LEDGREEN	0x01


#define SPI_SERVER_GPIO_GPRSP		0x03
#define SPI_SERVER_GPIO_GPRSPD		0x04
#define SPI_SERVER_GPIO_RC531		0x05
#define SPI_SERVER_GPIO_6P5V		0x06




/////////////////////////////////////
#define DATA_START          100
#define DATA_END            1115

#define CARD_SPEC_M1_8BIT 		0
#define CARD_SPEC_M1_32BIT 	0
#define CARD_SPEC_M1_LINUX 	0
#define CARD_SPEC_CPU_PBCO20 	1
#define CARD_SPEC_CPU_PBCO30 	2
#define CARD_SPEC_EMV_DATA 	3
#define CARD_SPEC_SCAN_DATA 	4
#define CARD_SPEC_DRIVER_LINUX 	5
#define CONSUME_MODE_PRESET		0
#define CONSUME_MODE_FLEXIBLE		1
#define CONSUME_MODE_SECTIONAL	2
#define CONSUME_MODE_TIMING		3


#define UDISK_MOUNT_NOD "/dev/diskUSB1"
#define UDISK_MOUNT_PATH "/mnt/udisk"
#define UDISK_MOUNT_NOD4 "/dev/diskUSB4"
struct gpio_config
{
	int port;
	int num;
	int data;
};


typedef  struct
{
	unsigned short date;
	unsigned short min;
}Interval;

//扇区缓冲
struct card_buf
{
        LTY_U8 key[6];
        LTY_U8 mode;
        LTY_U8 rwbuf[16];
        LTY_U8 money[4];
};

//公用体
typedef union
{
              LTY_U8 intbuf[2];
              unsigned short i;
} ShortUnon;

typedef union
{
              LTY_U8 longbuf[4];
              LTY_U32  i;
} LongUnon;

typedef union
{
       LTY_U8 longlongbuf[8];
		long int  i;
} LongLongUnon;

//用户扇区
typedef  struct
{
// LTY_U8 Zer;
 LTY_U8 One;
 LTY_U8 Two;
 LTY_U8 Thr;
 LTY_U8 For;
 LTY_U8 Fiv;
 LTY_U8 Six;
 LTY_U8 Sev;
 LTY_U8 Eig;
 LTY_U8 Nin;
 LTY_U8 Ten;
 LTY_U8 Ele;
 LTY_U8 Twe;
 LTY_U8 Thi;
 LTY_U8 Fou;
 LTY_U8 Fif;
 LTY_U8 Sixt;
 LTY_U8 NOP[6];
 LTY_U8 ADFNUM[2];
}CardLanSector;

// 查询缓存
typedef  struct
{
 	LTY_U8 LOC_TIME[8];     //定位时间
 	//LTY_U8 date2[3];     //时分秒
 	LTY_U8 LON[4];  //经度
 	LTY_U8 LAT[4];  //纬度
 	LTY_U8 VEC1[2]; //速度
 	LTY_U8 VEC2[2]; //速度
 	LTY_U8 DISTANCE[4]; //车辆当前总里程数
 	LTY_U8 DIRECTION[2];    //方向
 	LTY_U8 ALTITUDE[2];  //海拔高度
 	LTY_U8 DIS_NXT_STOP[2];
	LTY_U8 TIME_NXT_STOP[4];
	LTY_U8 INDOOR_TEMP;
	LTY_U8 NXT_STOP_NO;
	LTY_U8 GPS_SIGNAL;
}GPSDATA;



// 查询缓存
typedef  struct
{
 LTY_U32 sqlall;     //
 LTY_U32 SRFcsn;     //
 LTY_U32 SRFvaluej;  //
 LTY_U32 SRFvalueh;  //
 LTY_U8 RFtime[7]; //
 LTY_U8 RFtran;    //
 char End;
}SQLBUFFOUT;

/*
typedef struct 
{
	LTY_U8  VEH_STATE; //车辆运行状态
	LTY_U8 STOP_NO[4];//LTY_U32 STOP_NO;   //当前公交站点序号
	LTY_U8  DRIVER_ID[10]; //司机编号
	LTY_U8  VEHICLE_ID[6]; //车辆编号 
	LTY_U8  STOP_FLAG;  //公交站点标识；0x00，首战；0xff，末站；0x02，拐弯点；0x03，安全事故黑点；
	LTY_U8  STOP_RPT;   //报站方式，0x01，GPS自动报站；0x00，手动报站；0x02，表示调试；
	LTY_U8  IN_OUT_FLAG; //进出站标识；0，进站；1，离站
	LTY_U8  ONDIRECTION_FLAG; //0:上行 1:下行

}lty_station_data_t;

*/





extern LongUnon DevNum;	//终端机机号
extern LongUnon HostValue,DecValue;
extern LongUnon FValue,LonDbuf,BFValue;
extern LongUnon DevSID;

extern void beepopen (LTY_U8 Mode);
extern LTY_U8 cdma_rest(LTY_U32 dev);
extern LTY_U8 mystrncmp(const LTY_U8 *istra,const LTY_U8 *istrb,LTY_U8 len);
extern char Rd_timed (struct tm * tm);
extern void CardLanFile (LTY_U8 RW_Type);
extern char ethopen(void);


extern void ReadOrWriteFileB(LTY_U8 RW_Type);
extern LTY_U8 CheckGprsStu (void);
extern int initialize (const char *com, int speed);
extern void Check_db(void);
extern LTY_U8 SendHandData(LTY_U8 *send55AA,LTY_U32 len);
extern char FindSavedata(void);
extern char SaveCardData(int spec, int Mode);
extern LTY_U8 InitSystem(void);
extern void LEDL(LTY_U8 ON);
extern void LEDR(LTY_U8 ON);
extern void HEX_PRINTF(char *info,LTY_U8 *data,int len);
extern void Bcd_To_Asc(LTY_U8 *Asc, LTY_U8 *Bcd, LTY_U8 nlen);
extern int hex_2_ascii(LTY_U8 *INdata, LTY_U8 *buffer, LTY_U32 len);
extern LTY_U8 HEX2BCD(LTY_U8 hex_data);
extern LTY_U8 BCD2HEX(char bcd_data);
extern void HEX8TOBCD(LTY_U32 In, LTY_U8 *Pdata);
extern LTY_U32  BCDToDec(const LTY_U8 *bcd, LTY_U8 length);


////////////////////////// SDcard.c
int MmcCardDetect(void);
int MmcCardClose(void);
int SDcard_Look(LTY_U8 Type);
int USB_Updata(void);

//////////////  savesql.c
extern char RMAllRecord(LTY_U32 recordnum);
extern int SqlCheckNewDat (LTY_U32 Dtype);
extern char UPdateRecord(LTY_U32 Drdevnum,int SValue);
extern LTY_U8 SaveDirverData(void);

extern void Display_signal(LTY_U8 type);

//////////////// sendtotcp.c
//extern int write_datas_gprs(int fd, LTY_U8 *buffer, int buf_len);


//////////////// printer.c
extern void InitUart(int *fd,char *uart,int speed);
extern int Uart_Printf(int fd,char *uartdata);
extern int Uart_Readdata(int fd,LTY_U8 *buffer, int size);

extern void * Readuart_Pthread (void * args);

//线程锁
extern pthread_mutex_t m_sysfile;
#define ReadOrWriteFile(a)  { pthread_mutex_lock(&m_sysfile);  ReadOrWriteFileB(a);  pthread_mutex_unlock(&m_sysfile); }
#define Dis 0

long int gettime_usec(void);
#define NEW_VandD_Api //新的显示及语音提示模块






#endif
