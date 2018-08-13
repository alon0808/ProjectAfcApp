/*
Head file of qPBOC.c extern file
2011.09.21
*/
#ifndef _Qpboc_head_file_
#define _Qpboc_head_file_
typedef enum {
	qr_invalid = 0,
		qr_aliPay,
		qr_weiXin = 2,
		qr_unPay = 4,
		car_=8
}emSupportQr;

extern unsigned char g_supportQR;
extern void cpuPBOCmain(void);
extern int qPBOC_INIT(void);
extern void showqPbochis(void);
extern void qPboc_clr_rec_point(void);
extern int clrSamPkey(void);
extern void qPbocDealPkey(void);
extern int checkSamKey(unsigned char mode);
#endif
