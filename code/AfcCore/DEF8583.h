
#ifndef _def8583_head_
#define _def8583_head_

#include "SL8583.h"


extern st8583filedown gsl8583FileDownPara;
extern stsl8583Style gsl8583Style;//签到任务状态，和签到时的参数
extern stsl8583filelist gsl8583filelist[sl8583fileNum];	//暂时为6组,文件版本为5组+1组参数版本(放在最后一组）

//清除相应的文件列表参数，下载完后清除，如版本更新了也可以不清楚
extern void clrgsl8583filelist(unsigned char *fileflag);
extern void initsl8583para(void);
//mode=1,固定返回CRC; mode=0,CRC校验错误时返回计算的CRC，一致则返回0,
extern unsigned int checkgsl8583FileDownPara(unsigned char mode);
#endif

