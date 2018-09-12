#ifndef _INCLUDE_ICCARDLIB_H_
#define _INCLUDE_ICCARDLIB_H_
//creat by lks

//物理卡号存贮变量
extern stCardInfo gCardinfo;

//----请重刷变量-----
extern unsigned char cardSound;	//请重刷标识
extern unsigned int card_ser;	//请重刷时的卡的物理卡号
extern stcomtime SysTime;

extern fFlagVary_1 gBuInfo;	//设备状态，是否停车，是否刷设置卡,是否刷新界面
extern oldPursIfo old_Infor;
extern stDeviceParatable gDeviceParaTab;

extern stPsamInfo psamZJB;		//住建部PSAM
extern stPsamInfo psamJTB;		//交通部SPAM
extern sam_pub SamPubInf_ZJB;

extern unsigned int s_sum1,a_sum1,dis_sum2;
extern unsigned int a_sumR;//手机钱包中的真实金额

extern Parameter6 ErrorDetail[ERROR_DETAIL_SUM];
extern unsigned char error_pointer;		  //缓冲区的指针

#ifdef __cplusplus
extern "C" {
#endif

	extern void restore_disp(void);
	extern int month_decide(void);
	extern void disp_no_swipe(void);
	extern void money_msg(unsigned char dmode, unsigned int remM, unsigned int pucM, unsigned char cMOde);
	extern void SoundMessage(unsigned char cmd);
	extern void save_h_month(void);
	extern void BuildRecorde(unsigned char delType, unsigned char *recBuf);
	extern unsigned int IsInBlackList(unsigned char *sno);
	extern void save_normal(void);
	extern unsigned char Delay_Normal(void);
	extern unsigned int get_s_sum1(unsigned char mode);
	extern void Sound_cityCMD(unsigned char sMode);
	extern void DisRetry(void);
	extern void DisRetry2(void);
	extern unsigned char profile(unsigned char mode);
	extern void error(unsigned char code1, unsigned char Index);
	extern unsigned char FreeConsume(void);
	extern void end_card(void);
	extern unsigned char GetDateTime(void);
	extern void Save_delayCardinfo(unsigned char Cmode);
	extern unsigned char getMiKey(void);

	extern void addStatMoney(unsigned char mode, unsigned int moneyv);
	extern void saveDeviceParaTab(unsigned char mode, unsigned char *dat);
	//外部键盘输入
	extern void *main_ExKeyBoard(void *arg);
	extern void SYSgetbussinessNO(unsigned char *obuf);
	extern unsigned char PsamInitialize(void);
	//IC卡消费时的变量参数初始化
	extern void ICCardInit(void);
	extern void main_card(void);

#ifdef __cplusplus
}
#endif

#endif




