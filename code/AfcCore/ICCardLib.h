#ifndef _INCLUDE_ICCARDLIB_H_
#define _INCLUDE_ICCARDLIB_H_
//creat by lks

//�����Ŵ�������
extern stCardInfo gCardinfo;

//----����ˢ����-----
extern unsigned char cardSound;	//����ˢ��ʶ
extern unsigned int card_ser;	//����ˢʱ�Ŀ���������
extern stcomtime SysTime;

extern fFlagVary_1 gBuInfo;	//�豸״̬���Ƿ�ͣ�����Ƿ�ˢ���ÿ�,�Ƿ�ˢ�½���
extern oldPursIfo old_Infor;
extern stDeviceParatable gDeviceParaTab;

extern stPsamInfo psamZJB;		//ס����PSAM
extern stPsamInfo psamJTB;		//��ͨ��SPAM
extern sam_pub SamPubInf_ZJB;

extern unsigned int s_sum1,a_sum1,dis_sum2;
extern unsigned int a_sumR;//�ֻ�Ǯ���е���ʵ���

extern Parameter6 ErrorDetail[ERROR_DETAIL_SUM];
extern unsigned char error_pointer;		  //��������ָ��

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
	//�ⲿ��������
	extern void *main_ExKeyBoard(void *arg);
	extern void SYSgetbussinessNO(unsigned char *obuf);
	extern unsigned char PsamInitialize(void);
	//IC������ʱ�ı���������ʼ��
	extern void ICCardInit(void);
	extern void main_card(void);

#ifdef __cplusplus
}
#endif

#endif




