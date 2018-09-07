#ifndef _INCLUDE_RAM_STORAGE_H
#define _INCLUDE_RAM_STORAGE_H

extern unsigned char DriveCardNo[4];
extern unsigned int g24GDisFlash;//24G������ˢ��
extern int gOverTimes;
extern stCardInfo gCardinfo;

#ifdef __cplusplus
extern "C" {
#endif

	extern void flashread(unsigned int addr, unsigned char *rec_data, unsigned int length);
	extern void sysferead(unsigned int Address_f, unsigned int length, unsigned char *rec_data);
	extern void sysfereadOffset(unsigned int Address_f, unsigned int offsetAddr, unsigned int length, unsigned char *rec_data);
	extern void MessageBox(unsigned char mode, const char *dStr);

#ifdef __cplusplus
}
#endif

#endif
