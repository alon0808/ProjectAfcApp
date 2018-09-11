
//creat by lks

extern unsigned char ASCII2ZBcod(char *strin, unsigned char *ZBbuf, unsigned char ZBlen);
extern unsigned char ZBcod2ASCII(unsigned char *ZBbuf, unsigned char ZBlen, char *strin);


//最多4组,设置多少ms
extern void set_timer0(unsigned int tick, unsigned char tID);
//最多4组,返回还有多少ms
extern unsigned int get_timer0(unsigned char tID);
//输入7个字节的BCD码
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

#define Audio_INVALID		2		//非法卡
#define Audio_NOMONEY		3		//金额不足//余额不足
#define Audio_FAMILY		4		//家属卡
#define Audio_Zilu			5		//子女卡
#define Audio_YWJYu			6		//亲情卡  请年审  义务教育卡（九江）
#define Audio_STUDENT		7		//学生卡
#define Audio_STUFF			9		//员工卡
#define Audio_OLDMAN		10		//老人卡
#define Audio_LOVER			11		//爱心卡
#define Audio_TuanTi		12		//团体卡
#define Audio_Wuxiao		13		//无效卡
#define Audio_zuanxian		14		//专线卡 签到完毕, 请下车
#define Audio_MONTH			15		//月票卡
#define Audio_PuTong		16		//普通卡
#define Audio_YH			17		//优惠卡
#define Audio_DONG			18		//咚
#define Audio_DingDang		19		//咚咚，电信卡
#define Audio_Di			20		//嘀
#define Audio_YongJun		21		//拥军卡
#define Audio_SuaKaOK		22		//刷卡成功
#define Audio_YouDai		23		//优待卡
#define Audio_Siji			24		//司机卡
#define Audio_PLUS_MONEY	25		//请充值..B=请及时充值
#define Audio_BIRTHDAY		26		//敬老卡
#define Audio_CY			27		//优扶卡
#define Audio_FULI			28		//福利卡
#define Audio_GuaShi		29		//挂失卡,请交回
#define Audio_XiaoXueShen	30		//小学生卡
#define Audio_XueShenYP		31		//学生月票卡
#define Audio_YuerDi5		32		//余额低于5元

#define Audio_no_di			0xee	//没语音，只是蜂鸣器响

#define Audio_TRY_AGAIN		1		//请重刷 
#define Audio_FLING_MONEY	8		//请投币

extern void dis_time_mini(unsigned char x, stcomtime *time);
//语音提示
extern void audio(unsigned char flag);
extern void soundPlay(char * filename);

//蜂鸣器,次数、响延时、停延时
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

//后背LED显示,4字符
extern void LED_Dis3(char *str);

//----------------IC卡相关函数---------------------
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


//--------------PSAM卡相关函数--------------------------

/*---------------------------------------------------------
NAME ： SamColdReset
FUNC :  冷复位SAM 卡
IN   ：
OUT  ： 成功返回接收到的长度
失败返回0
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
//                   更新记录文件
//
////////////////////////////////////////////////////////////////////////
extern unsigned char  Update_Record(unsigned char Slot, unsigned char file_ID,unsigned char recordNO,unsigned char length,unsigned char *ptr);
///////////////////////////////////////////////////////////////////////////
//
//                       追加记录文件
//
///////////////////////////////////////////////////////////////////////////
extern unsigned char Append_Record(unsigned char Slot, unsigned char cla,unsigned char file_ID,unsigned char length,unsigned char *ptr);
/////////////////////////////////////////////////////////////////////////////
//
//                 内部认证
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
//        init_for_descrypt(uchar *cardNO)说明
//             输入: cardNO--指向用户卡物理卡号(4字节)
//             输出: PSAM 卡将利用卡中指定的密钥,产生一个
//                   临时密钥(过程密钥)，临时密钥在PSAM卡
//                  下电后自动消失，不可读。
//
///////////////////////////////////////////////////////////////////////////
extern unsigned char init_for_descrypt(unsigned char Slot, unsigned char p1,unsigned char p2,unsigned char length,unsigned char *ptr);
///////////////////////////////////////////////////////////////
//
//       generate key 生成子密钥（过程密钥）
//
//////////////////////////////////////////////////////////////
extern unsigned char MAC(unsigned char Slot, unsigned char p1,unsigned char p2,unsigned char length,unsigned char *ptr);

//建设部对CPU卡的认证指令
extern unsigned char CpuCardCheck(unsigned char slot, unsigned char *pMac, unsigned char *pData, unsigned char len);

extern unsigned char CPUMAC2(unsigned char PsamID, unsigned char *pMac, unsigned char *pData, unsigned char len);
extern unsigned char CpuCardSetMac(unsigned char PsamID, unsigned char *pMac, unsigned char *pData, unsigned char len);
extern unsigned char CpuBlackMac(unsigned char PsamID, unsigned char *pMac, unsigned char *pData, unsigned char len);
extern unsigned char CpuCardSetMac2(unsigned char PsamID, unsigned char *pMac, unsigned char *pData, unsigned char len);




