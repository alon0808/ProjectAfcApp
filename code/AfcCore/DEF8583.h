
#ifndef _def8583_head_
#define _def8583_head_

#include "SL8583.h"


extern st8583filedown gsl8583FileDownPara;
extern stsl8583Style gsl8583Style;//ǩ������״̬����ǩ��ʱ�Ĳ���
extern stsl8583filelist gsl8583filelist[sl8583fileNum];	//��ʱΪ6��,�ļ��汾Ϊ5��+1������汾(�������һ�飩

//�����Ӧ���ļ��б��������������������汾������Ҳ���Բ����
extern void clrgsl8583filelist(unsigned char *fileflag);
extern void initsl8583para(void);
//mode=1,�̶�����CRC; mode=0,CRCУ�����ʱ���ؼ����CRC��һ���򷵻�0,
extern unsigned int checkgsl8583FileDownPara(unsigned char mode);
#endif

