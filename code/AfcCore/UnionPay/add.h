
#ifndef _ADD_H_
#define _ADD_H_
#include "../include/include.h"
#if 1

// 
// #define FLASH1_START_O  FLASH_M_REC_START
// #define FLASH2_START1_O FLASH_M_REC_END
// 
// 
// //��¼����˵��������յ�ǰ�������˿ɴ���
// #define FLASH_RCLR_O 0x68000
// //��¼������˵����¼���� �����֮�������ռ�
// #define FLASH_MAX_O  0x65000
// // 1024K�ռ������ǰ���ݣ��ɴ�1��
// #define	LEN_EEPROM_O 0x70000
// // ������������ ��Get_Record_point ��ʹ�ã�
// #define FLASH_REC_MAX_O 800



#define FLASH1_START_O  FLASH_ODA
#define FLASH2_START1_O FLASH_0DA_END
#define FLASH_SIZE 16

//��¼����˵��������յ�ǰ�������˿ɴ���
#define FLASH_RCLR_O 0xc0000
//��¼������˵����¼���� �����֮�������ռ�
#define FLASH_MAX_O  0xF7000
// 1024K�ռ������ǰ���ݣ��ɴ�1��
#define	LEN_EEPROM_O 0x100000
// ������������ ��Get_Record_point ��ʹ�ã�
#define FLASH_REC_MAX_O 1500
#define RECORD_LEN_O  512

#endif

#if 0


#define FLASH1_START_O 0
#define FLASH2_START1_O 0x200000
#define FLASH2_START2END_O 0x300000
#define FLASH2_BAKR_LEN_O 0x100000

//��¼����˵��������յ�ǰ�������� 0x80000 �ɴ�8192��
#define FLASH_RCLR_O 0x80000
//��¼������˵����¼���� �����֮����7616���ռ�
#define FLASH_MAX_O  0xF7000
// 1024K�ռ������ǰ���ݣ��ɴ�16384��
#define	LEN_EEPROM_O 0x100000
// ������������ ��Get_Record_point ��ʹ�ã�
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
	
	unsigned char card_no[20];			//��һ�ʿ���
	unsigned char c_serial_qpoc[4];  //������
	unsigned char NS[4];				//��һ����ˮ��
	unsigned char time_last[7];      //��һ��ʱ��
    unsigned char flge; //��־
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


