
#ifndef __include_head_
#define  __include_head_
extern unsigned char DEBUG_COM; //COM4
//�ڿ⺯���ж���,���ú���ʱ����������ʹ��
#define  MaxPackLen    	1024
extern unsigned char irda_rxBuf[MaxPackLen];
extern volatile unsigned char PVDflag;
//��ѹ��־   1��ѹ����   2��ѹ����
#define PWR_LOW 1
#define PWR_HIG 2
//��ʼ��CPU���˿�
extern void port_init(void);
//ȡGPRS����汾��,��Ӧ�ó������64�ֽ�,�汾�ŷ���ǰ14�ֽ�
extern void getGpsProVER(unsigned char *ver);
//ȡ�⺯���汾
extern int getlibVer(void);
//mode = 0 �������к�
//mode = 1 �����ʶ
extern void getposSN(unsigned char *possn, unsigned char mode);
//��CPU���кţ�ֻ�������8λ ѹ��BCD��
extern void Get_SerialNum(unsigned char *ser);
//��F4����CPU���кţ�ֻ�������8λ ѹ��BCD��
extern void Get_SerialNumF4(unsigned char *ser);
/*****************************************
* ����: ��ʱ
* ����: ��ʱ��ʱ�����������ʱ,1��Լ=1ms��
* ���: NULL
******************************************/
extern void Delay(unsigned int ticks);
extern void delay ( void );
/*      LED        */					  
#define LED_RED        ((unsigned char)1)	//��ɫ
#define LED_GREEN      ((unsigned char)2)	//��ɫ
#define LED_BLUE       ((unsigned char)4)	//��ɫ
#define LED_YELLOW     ((unsigned char)8)	//��ɫ
#define ALL_LED			(LED_RED|LED_GREEN|LED_BLUE|LED_YELLOW)
/*****************************************
* ����: led�ڳ�ʼ��
* ����: led��� = LED_RED/LED_GREEN
* ���: NULL
******************************************/
extern void led_init(void);
/*****************************************
* ����: led��
* ����: led��� = LED_RED/LED_GREEN
* ���: NULL
******************************************/
extern void led_on(unsigned char type);
/*****************************************
* ����: led��
* ����: led��� = LED_RED/LED_GREEN
* ���: NULL
******************************************/
extern void led_off(unsigned char type);
/*****************************************
* ����: ledȡ��
* ����: led��� = LED_RED/LED_GREEN
* ���: NULL
******************************************/
extern void led_convert(unsigned char type);
/*      LED        */
/******************************************/
/*      BEEP       */
/*******************************
 *	���ܣ�����������
 *	���룺
 *	  ucTimes:��Ĵ���
 *	  ulBeepms:����ʱ
 *	  ulNoms:������ʱ
 *	�������
 ********************************/
extern void beepinit(void);
extern void beep(unsigned char ucTimes, unsigned int ulBeepms, unsigned int ulNoms);
/*                KEY              */
#define KEY_1 (1<<0)
#define KEY_2 (1<<1)
#define KEY_3 (1<<2)
#define KEY_4 (1<<3)
#define KEY_5 (1<<4)
#define KEY_6 (1<<5)

extern unsigned char getkey(unsigned char beepE) ;
extern void key_init(void);

/**************************************/
/****************************************
*����: �����˿ڳ�ʼ��
*����: NULL
*���: NULL
*****************************************/
extern void audioinit_old(void);
extern void audioinit(void);
extern void audio_reset(void);
/****************************************
*����: �������
*����: x-����Ķκ� 1-16
*���: NULL
*****************************************/
extern void audio_old(unsigned char x);
extern void audio(unsigned char x);
// LCD
#define DIS_CONVERT	(0x1)
#define DIS_CENTER	(0x1 << 1)
#define DIS_RIGHT	(0x1 << 2)
#define DIS_ClsLine	(0x1 << 3)
#define DIS_Cls		(0x1 << 4)
//LCD ��ʾ��ʼ��
extern void disinit(void);
/***************************************
*����: ���⿪�غ���
*����: flag 1:�� 0:��
*���: NULL
***************************************/
extern void dislight(unsigned char flag);
/***************************************
*����: ��ʾ����
*����: x-�� y-�� str-��ʾ��STRING mode-�Ƿ���
*���: NULL
***************************************/
extern void display(unsigned char x, unsigned char y, const char *str, unsigned char mode);
//��ά���������ʾ16�У�ÿ��40������ַ�,
extern void miniDispstr(unsigned char x, unsigned char y, const char *lcd_string, unsigned char mode);
//0<=antNum <=6 antNum=0xff,����ʾ
extern void antDispstr(unsigned char x, unsigned char y, unsigned char antNum, unsigned char mode);

//32(��)*32(��)������,��ʾ�ĵ���Χ�� 224*128
//x: 0--15, ����ʾ4�У�һ��ȡֵ0��4��8��12��		
//y: 0--27, һ����ʾ14��3216 ascii��(3232����7��)
/***************************************
*����: ��ʾ����
*����: x-�� y-�� str-��ʾ���ַ��� mode-�Ƿ���,���л�����ʾ
*���: NULL
***************************************/
extern void display_3232(unsigned char x, unsigned char y, const char *str, unsigned char mode);
/*********************************
*����: ����ʾ��
*����: NULL
*���: NULL
************************************/
extern void cls(void);



//��ʾ��ASCII�������С
//#define Dis_Font16		16		//1608
#define Dis_Font32		32		//3216
#define Dis_Font48		48		//4824
#define Dis_Font64		64		//6432

/***************************************
*����: ��ʾ������+С���㣩�ַ�������
*����: x   : ��, 0--15 
*      y    :��, 0--29 
*      str  : ��ʾ���ַ��� 
*      mode : �Ƿ���,���л�����ʾ�� 
*      size_font: 32/48/64
*���: NULL
***************************************/
extern void display_AsciiDigit(unsigned char x, unsigned char y, const char *str, unsigned char mode, unsigned char size_font);

extern void displayBCD_fone(unsigned char x, unsigned char y, unsigned char *bcd, unsigned char len, unsigned char size_fone);


//��ʾ�ַ�����С���㣬�����ʾ�����ַ������ÿ�����ֶ���С����Ϊ6���ַ�
extern void LED_Dis3(char *str);
//��ʼ������ܶ˿�
extern void LED_3INT(void);
/*************************************
*����: �����λ
*����: NULL
*���: NULL
**************************************/
extern void ReStartCPU(void);
//�忴�Ź�
extern void clr_wdt(void);
/********************************************
* ���ܣ�IIC��ʼ��
****************************************************/
extern void I2C_EE_Init(void);

//GPRS���ػ��ţ�ʹ�ܽ�Ϊ�ߵ�ƽ
extern void gprs_pwon_hi(void);
//extern void gprs_power_open(void);
//�رգ�ʹ�ܽ�Ϊ�͵�ƽ
extern void gprs_pwon_lo(void);
//extern void gprs_power_close(void);
//����MC55iģ�飬�ӳ�����200ms
extern void gprs_power_switch(void);
//����4.0��Դ����
extern void gprs_4V_ON(void);
//�ر�4.0��Դ����
extern void gprs_4V_OFF(void);
//GPRS�Ĵ���TXD,RXD��ΪGPIOģʽ���͵�ƽ���
extern void com2port(void);
/*             SPI                   */
/****************************************************SPI*/
/***************************************************
* ���ܣ�SPI��ʼ������
* ���룺NULL
* �����NULL
****************************************************/
//������������ 1�����߸�  0�����ߵ�
extern void XFSoundPower(unsigned char ctrl);
//��������ģ�鸴λ��,ģ�齫������,ͬʱ���������ŵ�ԴҲ�Ͽ���
extern void XF_Reset_LOW(void);
//��ȡ����ģ��״̬�ߣ�1æ��0��
extern unsigned char XF_GetBusy(void);
extern void spiInit(void);
extern void XFcmdsend(unsigned char cmd, unsigned int uiLen, char *str);
#define voice2busIn 1	//�������������(Ĭ��)
#define voice2busOut 2	//�������������
#define voice2board 3	//���������Ŵ�
extern void voice_switch(unsigned char mode);
extern void voice_init(void);
/********************************************
* flash����2Ƭ2M byte�Ŀռ�,��һƬ��ַ��0-1FFFFFH,�ڶ�Ƭ��200000H-3FFFFFH
* ���ܣ�SPI Flash ������, ��д�뺯������AAIд�뷽ʽ,д��ĳ��ȱ�����2�ı���
* ���룺addr����ַ; length-����; writebuf-����ָ��
* �����NULL
****************************************************/
extern void flashwrite(unsigned int addr, unsigned char *writebuf, unsigned int length);
extern void flashwritefont(unsigned int addr, unsigned char *writebuf, unsigned int length);
/********************************************
* flash����2Ƭ2M byte�Ŀռ�,��һƬ��ַ��0-1FFFFFH,�ڶ�Ƭ��200000H-3FFFFFH
* ���ܣ�SPI Flash ������
* ���룺addr����ַ; length-����; rec_data-����ָ��
* �����NULL
****************************************************/
extern int flashread(unsigned int addr,unsigned char *rec_data,unsigned int length);
// read status register
extern void flashRDSR(unsigned char chips, unsigned char *rec_data);
extern void flashReadID(unsigned char chips, unsigned char *rec_data);
extern char get_back_para(unsigned char *bpara);
#define BACK_HEAD	0	//4 ���ݽṹͷ 00 FF 55 AA
#define BACK_DEVICE 4	//8 �豸�ű���
#define BACK_LINENO 12	//3 ��·�ű���
#define BACK_CURRENT_POINT 15 //4 ��ǰָ�뱸��
#define BACK_HEAD_POINT 19 //4 ͷָ�뱸��
#define BACK_BCC_CHECK 23	//1 ����У�� ��24�ֽ�
#define BACK_MAX_REC 2500	//����2500��
#define BACK_NUM 24//һ���ĳ���
// extern int gBack_point;
// extern char save_back_para(unsigned char* device, unsigned char*line, unsigned char *cuP, unsigned char *hp)
// extern char init_back_para(void)

//ֻ������ǰ���ػ����ݣ�ÿ�ζ���0��ʼ���ɼ����ȫ�����������Ƶ���ʷ��
//GPRS���������´�����ǰ���Ѿ����͵�д��0�����Ƶ���ʷ����
//ǰһ���ֽ�д��ʹ�ñ�ʶ������˱�ʶ����ȷ������Ϊ�ǵ�һ��ʹ�ã�ȫ������
#define FLASH1_START 0
#define FLASH2_START1 0x200000
#define FLASH2_START2END 0x300000
#define FLASH2_BAKR_LEN 0x100000

//��¼����˵��������յ�ǰ�������� 0x80000 �ɴ�8192��
#define FLASH_RCLR 0x80000
//��¼������˵����¼���� �����֮����7616���ռ�
#define FLASH_MAX  0xF7000
// 1024K�ռ������ǰ���ݣ��ɴ�16384��
#define	LEN_EEPROM 0x100000
// ������������ ��Get_Record_point ��ʹ�ã�
#define FLASH_REC_MAX 16000

#define FLASH_M_REC_START 0x100000//�ƶ���¼������ ����256K�ռ� ��0x140000
#define FLASH_M_REC_END 0x140000//������
#define LEN_M_REC 0x40000//����������С256K

#define FLASH_FD_Ticket_START 0x150000 //�ֶ��շ�32K���ɴ�120*120��վ���Ʊ�ۡ�ǰ16�ֽ���ͷ�̶�Ϊ������·��Ϣ��Ч��
#define FLASH_FD_Ticket_END 0x158000 //��170000û��ʹ��

#define FLASH_FD_Ticket_TEMP_START 0x158000//�ݴ�ֶ�Ʊ��
#define FLASH_FD_Ticket_TEMP_END   0X160000

#define FLASH_backRec_Pointer 0x170000	//ÿ�����ʷָ����,ÿ��4K * (31+1)�� �� 128K  ��PROLIB ��Ҳ���ⶨ��,���ܸ���
#define FLASH_HisAddr_END 0x190000

#define FLASH_TXQRC_START	0x1A0000	//-0x1A1000 4K�ռ�   ��Ѷ�ܶ�ά����Կ�� 4K   //�������ظ������õ���Ѷ������ʱ����
#define FLASH_TXQRCE_END	0x1A1000	

// #define FLASH_TEST_BLOCK	0x1A1000	//-0x1A2000 4K�ռ䡡���ڲ���FLASH�Ƿ�����������ʱ��д�˿�
// #define FLASH_TEMP_PRICE	0x1A2000	//-0x1A3000 4K�ռ䡡���ڴ������ص�Ʊ����Ϣ
// #define FLASH_TEMP_RECORD	0x1A6000	//-0x1A7000 4K�ռ䡡���ڴ���׼��д��flash�ļ�ϵͳ����ʱ��¼�����������жϣ���ɾ��
//ֱ��0x1B0000 ��û����
#define FLASH_WHITE_START 0x1A7000//���л�����ͨ���������ռ䣺4K    ǰ��6�ֽڴ�����Ч���ͳ���2�ֽڣ���λ��ǰ
#define FLASH_WHITE_END	  0x1A8000

#define FLASH_WHITE_START_JTB	0x1A8000//JTB���л�����ͨ���������ռ䣺4K    ǰ��6�ֽڴ�����Ч���ͳ���2�ֽ� ����λ��ǰ  ����������ǰ2�ֽ��ǰ汾��λ��ǰ
#define FLASH_WHITE_END_JTB		0x1A9000


 #define FLASH_TAC_START	0x1AE000	//- 4K�ռ�   ��Ѷ��ά����������У�鱾�����ݵ�tackey 4K 
 #define FLASH_TAC_END	0x1AF000

#if 0	// �����������������
#define FLASH_MOBILE_START	0x1A0000	//-0x1A1000 4K�ռ�   �ƶ����������� 4K 
#define FLASH_MOBILE_END	0x1A1000	
#define FLASH_TEST_BLOCK	0x1A1000	//-0x1A2000 4K�ռ䡡���ڲ���FLASH�Ƿ�����������ʱ��д�˿�
#define FLASH_TEMP_PRICE	0x1A2000	//-0x1A3000 4K�ռ䡡���ڴ������ص�Ʊ����Ϣ
#define FLASH_TEMP_RECORD	0x1A6000	//-0x1A7000 4K�ռ䡡���ڴ���׼��д��flash�ļ�ϵͳ����ʱ��¼�����������жϣ���ɾ��

//ֱ��0x1B0000 ��û����
#define FLASH_QRCTAC_START  0X1A8000    // ��Գ���Կ��Ϣ
#define FLASH_QRCTAC_END    0X1A9000
#else   //��ʱ������Ѷ
#define FLASH_MOBILE_START	0x1A0000	//-0x1A1000 4K�ռ�   �ƶ����������� 4K 
#define FLASH_MOBILE_END	0x1A1000	
#define FLASH_TEST_BLOCK	0x1A1000	//-0x1A2000 4K�ռ䡡���ڲ���FLASH�Ƿ�����������ʱ��д�˿�
#define FLASH_TEMP_PRICE	0x1A2000	//-0x1A3000 4K�ռ䡡���ڴ������ص�Ʊ����Ϣ
#define FLASH_WHT_BANK		0x1A3000	//-0x1A4000 4K�ռ䡡���ڴ������ص�����������
#define FLASH_TEMP_RECORD	0x1A4000	//-0x1A5000 4K�ռ䡡���ڴ���׼��д��flash�ļ�ϵͳ����ʱ��¼�����������жϣ���ɾ��
#define FLASH_CACERT		0x1A5000	//-0x1A6000 4K�ռ䡡���ڴ���֤���ļ� 4 �ֽڳ���+  cacert-test.pem֤���ı�
#define FLASH_PBOC_PARA		0x1A6000	//-0x1A6000 4K�ռ䡡���ڴ��������Ĳ�������Կ֮��ģ������һ�ݣ����һ�ݿ�����⣬
#define FLASH_QRCTAC_START  0X1A8000    // ��Գ���Կ��Ϣ
#define FLASH_QRCTAC_END    0X1A9000
#define FLASH_CACERT_BACKUP	0x150000	//-0x1A6000 4K�ռ䡡���ڴ���֤���ļ� ������汸��
#define FLASH_RE_RECORD		0x1A000	//-0x1AB000 4K�ռ䡡���ڴ���֤���ļ�
//ֱ��0x1B0000 ��û����
#endif





#define FLASH_TEMP_START	0x1B0000	//-0x1C0000 ��ʱ���ݴ�������64K
#define FLASH_TEMP_END		0x1D0000	//-0x1C0000 ��ʱ���ݴ�������64K  //GPS

#define FLASH_GPS_LINE_START 0x1C0000	//GPS ��·��Ϣ��ʼ 128K
#define FLASH_GPS_LENE_VER	 0x1CFFC0	//�ܰ��İ汾�ţ����ں�64�ֽڴ�, ������·�����١����
#define FLASH_GPS_LENE_END	 0x1D0000	//GPS ��·��Ϣ���� 

#define FLASH_GPS_SOUND_START 0x1D0000	//GPS ������ʼ 64K  ���ﱻ���ڹ�Կ����
#define FLASH_GPS_SOUND_END	  0x1E0000	//GPS ��������


#define FLASH_PUBLICKEY_START	0x1E0000	//�����RSA�����湫Կ����32K
#define FLASH_PUBLICKEY_END		0x1E8000

#define FLASH_BLK_START 0x1F0000	//�������ռ䣺64K 65536 ÿ��4K
#define FLASH_BLK_END	0x200000


#define FLASH_GPS_AD_START 0x3B0000	//GPS������濪ʼ 64K
#define FLASH_GPS_AD_END	  0x3B8000	//GPS ����������

#define FLASH_ZK_START 0x3C0000		//�ֿ�ռ�:256K
#define FLASH_ZK_END 0x400000

#define FLASH_ZK_START_3232		0x500000	//�ֿ�ռ�:1M, 32*32����
#define FLASH_ZK_END_3232		0x5FFFFF

#define FLASH_BU_PARA			0x3AF000	//���ػ��̶������� 4K 2009.2.18 �ڿ⺯������д��

#define FLASH_RECORD_POINT		0x3B0000	//����ָ����Ϣ 64K
#define FLASH_RECORD_POINT_END	0x3C0000	//-0x3C0000 ����ָ����Ϣ

#define FLASH_RE     0x410000
#define FLASH_RE_END 0x420000
#define  FLASH_ODA        0x600000
#define FLASH_0DA_END     0x700000
//����F407����ռ��F103����϶࣬���ɴ�512- 64 = 448K
/*********************************
//F103����
//#define FLASH_PRO_FLAG		0x33FFC0	//���������־��64�ֽ�
//#define FLASH_PRO_START		0x300000	//���������ʼλ��   
//#define FLASH_PRO_END			0x33B800	//0x33C000//238K����ռ� 18K�������ռ�,���64�ֽڷ�GPRS��汾��
*********************************/
#define FLASH_PRO_FLAG			0x3700C0	//���������־��64�ֽ�	(0x3700C0-0x3700FF)
#define FLASH_PRO_START			0x300000	//���������ʼλ��   
#define FLASH_PRO_END			0x370000	//448k����ռ�,32K�������ռ�,32K�����ռ�


#define ERASE4KBYTESECTOR 1
#define ERASE32KBYTESECTOR 2
#define ERASE64KBYTESECTOR 3
#define ERASECHIP 0xaa

#define MASK4K 0x1000
#define MASK32K 0x8000
#define MASK64K 0x10000

#define FLASH1 1
#define FLASH2 2


extern void CheckHzk(unsigned char mode);
/*********************************************
*����: flash����
*����: mode-����һҳ�Ĵ�С addr-����ҳ�ڵĵ�ַ
*���: NULL
**********************************************/
extern void flasherase(unsigned char mode, unsigned int addr);
extern void flasherasefont(unsigned char mode, unsigned int addr);
/*           timer                */
extern void timer0_init (void);
/*---------------------------------------------------------
NAME: SET TIMER
FUNC������ȫ�ֵ�TIME[] ����
-----------------------------------------------------------*/
extern void set_timer(unsigned int tick);
extern void set_timer1(unsigned int tick);
/*---------------------------------------------------------
NAME: SET TIMER
FUNC������ȫ�ֵ�TIME1[] ����
-----------------------------------------------------------*/
extern void set_timer0(unsigned int tick, unsigned char mode);
/*---------------------------------------------------------
              �õ���ʱ��ʱ��
-----------------------------------------------------------*/
extern unsigned int get_timer(void);
extern unsigned int get_timer0(unsigned char mode);
extern unsigned int get_timer1(void);
/********************************************************/
/***************************************************
* ���ܣ�s��ʱ����
* ���룺xms-��ʱ��ֵ
* �����NULL
****************************************************/
extern void delayxs(unsigned int xs);
/***************************************************
* ���ܣ�ms��ʱ����
* ���룺xms-��ʱ��ֵ
* �����NULL
****************************************************/
extern void delayxms(unsigned int xms);
extern void delayus ( void );
extern void Delay(unsigned int ticks);
extern void spicssel(unsigned char index);
extern unsigned char spisndbyte(unsigned char data);
// USART 
#define COM1 1
#define COM2 2
#define COM3 3
#define COM4 4
#define COM5 5
#define COM6 6
#define COM_QR  7	//F407����6, �Ӷ�ά��ģ��
/***************************************************
* ���ܣ����ڿ��жϳ�ʼ��
* ���룺com�����ں� 1����1��DEBUG��RS232�����TLL[JP1])��2����2(���⼰GPRS)��4����4(RS232),5����5(TLL[JP2])
* �����NULL
* ע��:����3�Ǵ���1�ĸ���,���������Э���
		�����ж�ʱҪд�жϷ������(��stm32f10x_it.c��)
****************************************************/
extern void COM_it_enable(unsigned char com);
/***************************************************
* ���ܣ����ڹر��жϳ�ʼ��
* ���룺com�����ں� 1����1��DEBUG��RS232�����TLL[JP1])��2����2(���⼰GPRS)��4����4(RS232),5����5(TLL[JP2])
* �����NULL
* ע��:����3�Ǵ���1�ĸ���,���������Э���
****************************************************/
extern void COM_it_disable(unsigned char com);
/***************************************************
* ���ܣ����ڳ�ʼ������
* ���룺com�����ں� 1����1��DEBUG��RS232�����TLL[JP1])��2����2(���⼰GPRS)��4����4(RS232),5����5(TLL[JP2])
		uibaud-������
* �����NULL
* ע��:����3�Ǵ���1�ĸ���,���������Э���
****************************************************/
extern void com_init(unsigned char com, unsigned int uibaud);
/***************************************************
* ���ܣ����ڷ��ͳ���
* ���룺com�����ں�; uiLen-�������ݳ���; SenBuf-�������ݻ���
* �����NULL
* ע�⣺����1��2��3����DMA���ͷ�ʽ��COM2��෢��1024���ֽ�  COM1\3��෢��300�ֽ�
	����COM2�õ���"comSendbuff"  com1\3�õ���"irda_rxBuf"
	DMA��ʽ�����ܽ����ŵ��ö�Σ�������һ��û���꣬�ͷ�����ε����ϴε����ݷ��Ͳ�����
	COM2:ʹ��DMA��ʽ��COM1\3������com��4λ��Ϊ0ʱ����DMA��ʽ��������DMA��
****************************************************/
extern unsigned char comSendbuff[1024];
extern void com_snd(unsigned char com, unsigned int uiLen, unsigned char *SendBuf);
/***************************************************
* ���ܣ����ڽ��ճ���
* ���룺com�����ں�; RevBuf-�����ֽڻ���
* �����0 �ɹ������� ��ʱ
****************************************************/
extern int com_rev(unsigned char com, unsigned int *RevBuf);
/***************************************************
* ���ܣ����ڴ�ӡ���ַ���
* ���룺com�����ں�; str-�ֽڻ���
* �����0 �ɹ������� ��ʱ
****************************************************/
extern void debugstringCOM(unsigned char com, char *str);
/***************************************************
* ���ܣ����ڴ�ӡ��ʮ��������
* ���룺com�����ں�; value-�ֽڻ���,uclen-����,mode-������ɺ��Ƿ����س�(\r\n)
* �����0 �ɹ������� ��ʱ
****************************************************/
extern void debugdataCOM(unsigned char com, unsigned char *value, unsigned int uclen, unsigned char mode);
//�����ǹ̶����ڷ���
extern void debugstring4(char *str);
//extern void debugstring(char *str);
//mode���λΪ1ʱΪ����4���,���򴮿�1���
//extern void debugdata(unsigned char *value, unsigned int uclen, unsigned char mode);
//mode���λΪ1ʱΪ����4���,���򴮿�1���
extern void debugstringC(char *str, unsigned char mode);
/***************************************************
* ���ܣ����ں��⹦�ܳ�ʼ������
* ���룺uibaud-������
* �����NULL
****************************************************/
extern void irda_init(unsigned int uibaud);
extern void Irda_It( void );
/***************************************************
* ���ܣ����ⷢ��,��ʹ�ܷ��ͣ���ֹ����
* ���룺uilen=�����ֽ���,RevBuf-�����ֽڻ���
* �����0=������ȷ������-��Ч
****************************************************/
extern int irda_snd(unsigned int uilen, unsigned char *value);
extern void irda_it_En(void);
extern void irda_it_dis(void);
/********************************************
* ���ܣ�ʱ���ʼ��
* ���룺NULL
* �����NULL
******************************************/
extern void timeinit(void);
/********************************************
* ���ܣ���ȡʱ��
* ���룺time-ʱ������ָ��
* �����0
*********************************************/
extern int timeread(unsigned char *time);
/********************************************
* ���ܣ�����RTCʱ��
* ���룺time-ʱ������ָ�� ssmmhh ddmmyyyy
* �����0
****************************************************/
extern int timewrite(unsigned char *time);

/********************************************
* ���ܣ����ʱ���Ƿ���Ч
* ���룺time-ʱ������ָ�� 7�ֽ� BCD��
* �����0-��Ч   1-��Ч
****************************************************/
extern unsigned char time_invalid(unsigned char *time_buff);
/********************************************
* ���ܣ����ʱ���Ƿ���Ч�ַ�
* ���룺time-ʱ������ָ��  14�ֽ� ASC��
* �����0-��Ч   1-��Ч
****************************************************/
extern unsigned char time_invalid_str(unsigned char *time_buff);
extern void setdatetime(void);
//14�ֽ�����ʱ�� �Ƿ�Ϸ� 0���Ϸ�  1�Ϸ�
extern unsigned char time_long_invalid(unsigned char *time_long_buff);
//7�ֽڳ���ʱ��Ƚ�, 1 scrt>dest  -1 dest>scrt 0 ���
extern int cmpttime(unsigned char *scrt, unsigned char *dest);
extern int SysSetTime(unsigned char *time);
/********************************************
* ���ܣ�д����
* ���룺Address_f-�����ַ length-���� Framdata-����ָ��
* �����NULL
****************************************************/
extern void fewrite(unsigned int Address_f,unsigned int length, unsigned char *Framdata);
/********************************************
* ���ܣ���ȡ��������
* ���룺Address_f-�����ַ length-���� Framdata-����ָ��
* �����NULL
****************************************************/
extern void  feread(unsigned int Address_f,unsigned int length,unsigned char *rec_data);
/********************************************
* ���ܣ�д����  д���ٶ����Ƚ� ֻ�Ƚ�ǰ256��
* ���룺Address_f-�����ַ length-���� Framdata-����ָ��
* �����NULL
****************************************************/
extern void sysfewrite(unsigned int Address_f,unsigned int length, unsigned char *Framdata);
/********************************************
* ���ܣ���ȡ�������� ��ζ����Ƚ� ֻ�Ƚ�ǰ256��
* ���룺Address_f-�����ַ length-���� Framdata-����ָ��
* �����NULL
****************************************************/
extern void sysferead(unsigned int Address_f,unsigned int length,unsigned char *rec_data);

extern void open_wdt(void);
//�ļ�ϵͳ�յ��������ֵ��ȥ8M��
extern unsigned int ugflash_capacity;

//mifare

#define PICC_REQIDL        0x26         //!< request idle
#define PICC_REQALL        0x52         //!< request all
#define PICC_ANTICOLL1     0x93         //!< anticollision level 1 106 kBaud
#define PICC_ANTICOLL2     0x95         //!< anticollision level 2
#define PICC_ANTICOLL3     0x97         //!< anticollision level 3
#define PICC_AUTHENT1A     0x60         //!< authentication using key A
#define PICC_AUTHENT1B     0x61         //!< authentication using key B
#define PICC_READ          0x30         //!< read block
#define PICC_WRITE         0xA0         //!< write block
#define PICC_UL_WRITE      0xA2         //!< ultralight write block
#define PICC_DECREMENT     0xC0         //!< decrement value
#define PICC_INCREMENT     0xC1         //!< increment value
#define PICC_RESTORE       0xC2         //!< restore command code
#define PICC_TRANSFER      0xB0         //!< transfer command code
#define PICC_HALT          0x50         //!< halt

extern unsigned char RC531FIFOLevelCheck(void);
extern char PcdRfReset(unsigned char ms);
extern unsigned char InitRC531( void );
extern unsigned char GetTypeAUID( unsigned char *pUID );

extern unsigned char MifareAuth(unsigned char key_type,
								unsigned char *snr,
								unsigned char key_sector,
								unsigned char block);
// ==================================================================
//
extern unsigned char MifareAuthKey(unsigned char auth_mode,
								   unsigned char *snr,
								   unsigned char *keys,
								   unsigned char block);
// ==================================================================
//
extern unsigned char MifareRead(unsigned char cBlockNo, unsigned char *pRdData);
// ==================================================================
//
extern unsigned char MifareWrite(unsigned char cBlockNo, unsigned char *pWrData);
// ==================================================================
//
extern unsigned char MifareValueDebit(unsigned char cSubCommand, unsigned char cBlockNo, unsigned char *pValue);
// ==================================================================
//
extern unsigned char MifareTransfer(unsigned char cBlockNo);
// ==================================================================
//
extern unsigned char MifareValue(unsigned char cSubCommand, unsigned char cBlockNo, unsigned char *pValue);

extern unsigned char MifareHalt(void);

extern unsigned char RC531Close( void );
extern unsigned int RfPassthrough( unsigned char *dst, unsigned int iMaxlen, unsigned char *src, unsigned int sendlen);
//���ش���Ĵ�����ֵ
extern unsigned short RC531errReg(void);

extern void Rc531TxClose(void);
extern void Rc531TxOpen(void);

//---type B
extern unsigned char GetTypeBUID( unsigned char *pUID );
extern unsigned char RC531ReqB( unsigned char *rev );
extern unsigned int RfPassthrough_TypeB(unsigned char *dst, unsigned int iMaxlen, unsigned char *src, unsigned int sendlen);
//���أ�22=RC522��ͷ��31=RC531��ͷ������=��ͷ��ʼ������
extern unsigned char getRCMode(void);
//PSAM

#define SAM1 1
#define SAM2 2
#define SAM3 3
#define SAM4 4

#define SAM_VOL3 3
#define SAM_VOL5 5

typedef struct
	{
		unsigned char CLA;
		unsigned char INS;
		unsigned char P1;
		unsigned char P2;

		unsigned char LC;
		unsigned char DATA[256];

		unsigned char LE;
	}ISO7816_ADPU_SEND;

typedef struct
	{
		unsigned char LE;
		unsigned char DATA[256];

		unsigned char SW1;
		unsigned char SW2;
	}ISO7816_ADPU_RESPONSE;

/*********************************
*����: SAM�����Ƴ�ʼ��
*����: chips-SAM����,vol-��ѹ,baud-������
*���: NULL
**********************************/
extern void saminit(void);
/*******************************************
* ���ܣ���TDA8020״̬
* ���룺sam-SAM����� value��������ֵ
* ���: NULL
**********************************************/
#define PSAM_PRES 0x01
#define PSAM_PRESL 0x01<<1
#define PSAM_IO 0x01<<2
#define PSAM_SUPL 0x01<<3
#define PSAM_PROT 0x01<<4
#define PSAM_MUTE 0x01<<5
#define PSAM_EARLY 0x01<<6
#define PSAM_ACTIV 0x01<<7
extern unsigned char DeactiveCard(unsigned char ucCardSelec);
extern void TDA8020Read(unsigned char sam, unsigned char* value);

extern unsigned char SamColdResetNPPS(unsigned char CardSelect, unsigned int uiRate, 
						   unsigned char ucVoltage, unsigned char *atr);
/*---------------------------------------------------------
NAME �� SamColdReset
FUNC :  �临λSAM ��
IN   ��
OUT  �� �ɹ����ؽ��յ��ĳ���
		ʧ�ܷ���0
-----------------------------------------------------------*/
extern unsigned char SamColdReset(unsigned char CardSelect, unsigned int uiRate, 
						   unsigned char ucVoltage, unsigned char *atr);
//						   unsigned char *Rec_len);
/*---------------------------------------------------------
NAME �� SamSendApdu
FUNC :  ��SAM������APDU���ݰ�(T0Э��)
IN   ��
OUT  �� �ɹ�����1
		ʧ�ܷ���0
-----------------------------------------------------------*/
extern unsigned short SamdSendAPDUT0(unsigned char *buffer, unsigned short length,
							  unsigned char *rbuffer);
/*---------------------------------------------------------
NAME �� SamSendApdu
FUNC :  ��SAM������APDU���ݰ�
IN   ��
OUT  �� �ɹ�����0
		ʧ�ܷ�������
-----------------------------------------------------------*/
//extern unsigned char SamSendApdu(unsigned char *pTxBuf, unsigned short iLen, unsigned char *pRxBuf);
//extern unsigned char IccCommand(unsigned char Slot, ISO7816_ADPU_SEND *ApduSend, 
//						 ISO7816_ADPU_RESPONSE *ApduRecv);

extern unsigned char Get_Challenge(unsigned char Slot, unsigned char *Receivebuf);
extern unsigned char  Select_FileF(unsigned char Slot,unsigned char *file_ID, unsigned char len);
extern unsigned char  Select_File(unsigned char Slot, unsigned int file_ID);
extern unsigned char Verify_private_PIN(unsigned char Slot, unsigned char length, unsigned char *ptr);
extern unsigned char Change_private_PIN(unsigned char Slot, unsigned char length,unsigned char *ptr);
extern unsigned char  Read_Binary(unsigned char Slot, unsigned char file_ID,unsigned char offset,unsigned char length,unsigned char *ptr);
extern unsigned char  Update_Binary(unsigned char Slot, unsigned char cla,unsigned char file_ID,unsigned char offset,unsigned char length,unsigned char *ptr);
extern unsigned char Get_MifKey(unsigned char Slot, unsigned char p1, unsigned char p2, unsigned char length,unsigned char *ptr);
extern unsigned char  Read_Record(unsigned char Slot, unsigned char file_ID,unsigned char recordNO,unsigned char length,unsigned char *ptr);
////////////////////////////////////////////////////////////////////////
//
//                   ���¼�¼�ļ�
//
////////////////////////////////////////////////////////////////////////
extern unsigned char  Update_Record(unsigned char Slot, unsigned char file_ID,unsigned char recordNO,unsigned char length,unsigned char *ptr);
///////////////////////////////////////////////////////////////////////////
//
//                       ׷�Ӽ�¼�ļ�
//
///////////////////////////////////////////////////////////////////////////
extern unsigned char Append_Record(unsigned char Slot, unsigned char cla,unsigned char file_ID,unsigned char length,unsigned char *ptr);
/////////////////////////////////////////////////////////////////////////////
//
//                 �ڲ���֤
//
/////////////////////////////////////////////////////////////////////////////
extern unsigned char  Internal_Auth(unsigned char Slot, unsigned char *ptr, unsigned char Keyno);
///////////////////////////////////////////////////////////////
//
//       though  external authen  psam
//
///////////////////////////////////////////////////////////////

extern unsigned char  External_Auth(unsigned char Slot, unsigned char *ptr, unsigned char Keyno);
////////////////////////////////////////////////////////////////////////////
//
//        init_for_descrypt(uchar *cardNO)˵��
//             ����: cardNO--ָ���û���������(4�ֽ�)
//             ���: PSAM �������ÿ���ָ������Կ,����һ��
//                   ��ʱ��Կ(������Կ)����ʱ��Կ��PSAM��
//                  �µ���Զ���ʧ�����ɶ���
//
///////////////////////////////////////////////////////////////////////////
extern unsigned char init_for_descrypt(unsigned char Slot, unsigned char p1,unsigned char p2,unsigned char length,unsigned char *ptr);
///////////////////////////////////////////////////////////////
//
//       generate key ��������Կ��������Կ��
//
//////////////////////////////////////////////////////////////
extern unsigned char MAC(unsigned char Slot, unsigned char p1,unsigned char p2,unsigned char length,unsigned char *ptr);
extern unsigned char IccCommand(unsigned char Slot, ISO7816_ADPU_SEND *ApduSend, ISO7816_ADPU_RESPONSE *ApduRecv);
//"G" GPS��Ϣ    ��B�� ������   ��S��GPS����
extern void Move_temp(char typee);
//headaddr:ͷָ��  len:��ǰ��¼����
extern void com_up_data(unsigned char *headaddr, unsigned int len);
//mode == 1 ��Ҫ����β =2 ��ˮ���ݸ�ʽ��Ӱ�찴ʱ��ɼ� 32�ֽڼ�¼�ݽ�û�˹��ܣ�
//mode == 0x53 ���Ѻ����������ں�����������������ʱ��
extern void get_data(unsigned char mode);
extern void get_data_more(unsigned char mode);
extern unsigned char sys_bu_para_save(unsigned char *para, unsigned int uilen);
extern unsigned char sys_bu_para_load(unsigned char *para, unsigned int uilen);
//CRC32 Ӳ���㷨
extern unsigned int __cpuCrc32(unsigned char *val, unsigned int len);

//�Ӵ����ռ�4�ֽڴ���ʼ���룬���Ȳ�����220�ֽ�
extern void cpuInfoWrite(unsigned char *val, unsigned char len);
extern void cpuInfoRead(unsigned char *val, unsigned int offset, unsigned int len);
//��������ʶ��д���ʶλ��:0x4000+240,ʹ�ÿռ�8�ֽ�
//mode:=1д�� 0��� ֻ�������������ʹ��,
extern void cpuWNewSoundChip(unsigned char mode);
//����1 �£�0��
unsigned char getAudioStyle(void);

extern void over_turn(unsigned char length,unsigned char *ptr);
extern unsigned char ValidDevNo(unsigned char *dvcbuf);
extern unsigned char ValidlineNo(unsigned char*lineB);

extern unsigned int __atol(char *strASC);

//-----------CAN����-----------------------------
extern void CAN_Initiation(void);
//����,ÿ�������8���ֽ�.����Է����͵����ݴ���8���ֽ�,��Ҫ��ν��պ����.
extern unsigned char can_rev(unsigned char *RevBuf);
//���� ����0�ɹ�,1ʧ��[���������Ƿ�Ӻ�]
extern unsigned char can_snd(unsigned char *sndbuf, unsigned int sndlen);
//--------------------------------------

extern void CountPoint(unsigned char mode);
extern void SetTcpPacketTTLTime(unsigned char val);

 extern int close_rfile_Wrecord(void);
 extern int open_rfile_Wrecord(void);
#endif

