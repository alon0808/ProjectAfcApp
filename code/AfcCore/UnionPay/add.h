
#ifndef _ADD_H_
#define _ADD_H_
#include "../include/include.h"
#if 1

// 
// #define FLASH1_START_O  FLASH_M_REC_START
// #define FLASH2_START1_O FLASH_M_REC_END
// 
// 
// //记录到这说明可以清空当前数据区了可存条
// #define FLASH_RCLR_O 0x68000
// //记录到这条说明记录满了 和清除之间有条空间
// #define FLASH_MAX_O  0x65000
// // 1024K空间存贮当前数据，可存1条
// #define	LEN_EEPROM_O 0x70000
// // 最多存贮的条数 在Get_Record_point 中使用，
// #define FLASH_REC_MAX_O 800



#define FLASH1_START_O  FLASH_ODA
#define FLASH2_START1_O FLASH_0DA_END
#define FLASH_SIZE 16

//记录到这说明可以清空当前数据区了可存条
#define FLASH_RCLR_O 0xc0000
//记录到这条说明记录满了 和清除之间有条空间
#define FLASH_MAX_O  0xF7000
// 1024K空间存贮当前数据，可存1条
#define	LEN_EEPROM_O 0x100000
// 最多存贮的条数 在Get_Record_point 中使用，
#define FLASH_REC_MAX_O 1500
#define RECORD_LEN_O  512

#endif

#if 0


#define FLASH1_START_O 0
#define FLASH2_START1_O 0x200000
#define FLASH2_START2END_O 0x300000
#define FLASH2_BAKR_LEN_O 0x100000

//记录到这说明可以清空当前数据区了 0x80000 可存8192条
#define FLASH_RCLR_O 0x80000
//记录到这条说明记录满了 和清除之间有7616条空间
#define FLASH_MAX_O  0xF7000
// 1024K空间存贮当前数据，可存16384条
#define	LEN_EEPROM_O 0x100000
// 最多存贮的条数 在Get_Record_point 中使用，
#define FLASH_REC_MAX_O 1600

#define RECORD_LEN_O  512
#endif

#define time_over_re 3
#define time_over_shuangmian 7
#define time_SOCKET_OVER_TIME 30 //s
#define Switch_sign_OVER 0xad
#define set_delay_send_val 3
typedef struct
{
	
	unsigned char card_no[20];			//上一笔卡号
	unsigned char c_serial_qpoc[4];  //物理卡号
	unsigned char NS[4];				//上一笔流水号
	unsigned char time_last[7];      //上一笔时间
    unsigned char flge; //标志
	unsigned char RES[20];

}qpboc_pos_infor;

extern int save_ODA_infor(unsigned char mode, unsigned char *re_infor);
extern unsigned int cal_space_O(void);
extern void lib_clr_Record_point_O(void);
extern void init_Q_pos_infor(void);
extern void set_pos_infor_1(unsigned char mode);
extern  qpboc_pos_infor Q_pos_infor;
extern unsigned char SWITCH_(unsigned char *temp,unsigned char len,unsigned char *data);
extern unsigned char get_q_(unsigned char mode);

extern int ProcessTmsData(void);
extern void showGPRStR_add(void);
extern void set_over_time_re(void);
extern void set_over_time_shuang(void);
extern void Judge_timeout_re(unsigned char mode,unsigned char switch_over );
extern void save_over_infor(unsigned int uiTemp);
extern void init_timeout_infor(void);
extern unsigned char  over_GPRStR_add2(void);
extern void Switch_sign(unsigned char mode);
extern unsigned char get_over_time_shuang(void);
extern unsigned char get_over_time_re(void);
extern qpboc_pos_infor timeout_infor;
extern void init_delay_send(void);
extern void set_delay_send(unsigned int val);
extern unsigned int get_delay_send(void);
extern void reduce_delay_send(void);
extern unsigned char  DELAY_SEND(void);
extern void add_init_sum(void);
extern void add_init_sum_frist(void);
extern  void set_SOCKET_OVER_TIME(void);
extern unsigned char get_SOCKET_OVER_TIME(void);
#endif


