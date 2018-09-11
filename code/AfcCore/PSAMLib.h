
//creat by lks

extern unsigned char ASCII2ZBcod(char *strin, unsigned char *ZBbuf, unsigned char ZBlen);
extern unsigned char ZBcod2ASCII(unsigned char *ZBbuf, unsigned char ZBlen, char *strin);


//���4��,���ö���ms
extern void set_timer0(unsigned int tick, unsigned char tID);
//���4��,���ػ��ж���ms
extern unsigned int get_timer0(unsigned char tID);
//����7���ֽڵ�BCD��
//extern void timewrite(unsigned char *bcdDateTime);

extern unsigned char time_invalid(unsigned char *time_buff);
//extern void MessageBox(unsigned char mode, const char *dStr);
extern void keyBoardStart(void);
extern unsigned char KeyDeal(void);
extern void KeyBoardDeal(unsigned char vdat);
extern unsigned char KeyMange(unsigned char kkvv);
extern void KeyDis(unsigned int value,unsigned char *ptr);

/*      LED        */					  
#define LED_RED        ((unsigned char)1)
#define LED_GREEN      ((unsigned char)2)
#define LED_BLUE       ((unsigned char)4)
#define LED_YELLOW     ((unsigned char)8)

extern void led_on(unsigned char mode);
extern void led_off(unsigned char mode);

#define Audio_INVALID		2		//�Ƿ���
#define Audio_NOMONEY		3		//����//����
#define Audio_FAMILY		4		//������
#define Audio_Zilu			5		//��Ů��
#define Audio_YWJYu			6		//���鿨  ������  ������������Ž���
#define Audio_STUDENT		7		//ѧ����
#define Audio_STUFF			9		//Ա����
#define Audio_OLDMAN		10		//���˿�
#define Audio_LOVER			11		//���Ŀ�
#define Audio_TuanTi		12		//���忨
#define Audio_Wuxiao		13		//��Ч��
#define Audio_zuanxian		14		//ר�߿� ǩ�����, ���³�
#define Audio_MONTH			15		//��Ʊ��
#define Audio_PuTong		16		//��ͨ��
#define Audio_YH			17		//�Żݿ�
#define Audio_DONG			18		//��
#define Audio_DingDang		19		//���ˣ����ſ�
#define Audio_Di			20		//��
#define Audio_YongJun		21		//ӵ����
#define Audio_SuaKaOK		22		//ˢ���ɹ�
#define Audio_YouDai		23		//�Ŵ���
#define Audio_Siji			24		//˾����
#define Audio_PLUS_MONEY	25		//���ֵ..B=�뼰ʱ��ֵ
#define Audio_BIRTHDAY		26		//���Ͽ�
#define Audio_CY			27		//�ŷ���
#define Audio_FULI			28		//������
#define Audio_GuaShi		29		//��ʧ��,�뽻��
#define Audio_XiaoXueShen	30		//Сѧ����
#define Audio_XueShenYP		31		//ѧ����Ʊ��
#define Audio_YuerDi5		32		//������5Ԫ

#define Audio_no_di			0xee	//û������ֻ�Ƿ�������

#define Audio_TRY_AGAIN		1		//����ˢ 
#define Audio_FLING_MONEY	8		//��Ͷ��

extern void dis_time_mini(unsigned char x, stcomtime *time);
//������ʾ
extern void audio(unsigned char flag);
extern void soundPlay(char * filename);

//������,����������ʱ��ͣ��ʱ
extern void beep(unsigned char ucTimes, unsigned int ulBeepms, unsigned int ulNoms);

// LCD
#define DIS_CONVERT	(0x1)
#define DIS_CENTER	(0x1 << 1)
#define DIS_RIGHT	(0x1 << 2)
#define DIS_ClsLine	(0x1 << 3)
#define DIS_Cls		(0x1 << 4)
extern void cls(void);
extern void display(unsigned char x, unsigned char y, const char *str, unsigned char mode);
extern void miniDispstr(unsigned char x, unsigned char y, const char *lcd_string, unsigned char mode);

//��LED��ʾ,4�ַ�
extern void LED_Dis3(char *str);

//----------------IC����غ���---------------------
//M1
extern unsigned char MifareWrite(unsigned char cBlockNo, unsigned char *pWrData);

extern unsigned char MifareAuthKey(unsigned char auth_mode,
							unsigned char *snr,
							unsigned char *keys,
							unsigned char block);
extern unsigned char MifareRead(unsigned char cBlockNo, unsigned char *pRdData);
extern unsigned char MifIsPurse(unsigned char *buffer);
extern unsigned char MiBlockInvalid(unsigned char mode,unsigned char count,unsigned char *ptr);
extern unsigned char GetTypeAUID( unsigned char *pUID );
extern void MakeSureCardLeave(int timeout);
extern void MifareHalt(void);
extern unsigned char MifareValueDebit(unsigned char cSubCommand, unsigned char cBlockNo, unsigned char *pValue);
// ==================================================================
//
extern unsigned char MifareTransfer(unsigned char cBlockNo);


extern void deal2or3(unsigned char*sndd);
extern unsigned int CPURats(unsigned char *rbuf);
extern unsigned int RfPassthrough( unsigned char *dst, unsigned int iMaxlen, unsigned char *src, unsigned int sendlen);


//--------------PSAM����غ���--------------------------

/*---------------------------------------------------------
NAME �� SamColdReset
FUNC :  �临λSAM ��
IN   ��
OUT  �� �ɹ����ؽ��յ��ĳ���
ʧ�ܷ���0
-----------------------------------------------------------*/
extern unsigned char SamColdReset(unsigned char CardSelect, unsigned int uiRate, 
								  unsigned char ucVoltage, unsigned char *atr);

extern unsigned char Get_Challenge(unsigned char Slot, unsigned char *Receivebuf, unsigned char len);
extern unsigned char  Select_FileF(unsigned char Slot,unsigned char *file_ID, unsigned char len);
extern unsigned char  Select_File(unsigned char Slot,unsigned int file_ID);
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

//���貿��CPU������ָ֤��
extern unsigned char CpuCardCheck(unsigned char slot, unsigned char *pMac, unsigned char *pData, unsigned char len);

extern unsigned char CPUMAC2(unsigned char PsamID, unsigned char *pMac, unsigned char *pData, unsigned char len);
extern unsigned char CpuCardSetMac(unsigned char PsamID, unsigned char *pMac, unsigned char *pData, unsigned char len);
extern unsigned char CpuBlackMac(unsigned char PsamID, unsigned char *pMac, unsigned char *pData, unsigned char len);
extern unsigned char CpuCardSetMac2(unsigned char PsamID, unsigned char *pMac, unsigned char *pData, unsigned char len);




