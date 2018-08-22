#ifndef _INCLUDE_RAM_STORAGE_H
#define _INCLUDE_RAM_STORAGE_H

extern unsigned char DriveCardNo[4];
extern unsigned int g24GDisFlash;//24G¿¨½çÃæË¢ÐÂ
extern int gOverTimes;

#ifdef __cplusplus
extern "C" {
#endif

	extern void flashread(unsigned int addr, unsigned char *rec_data, unsigned int length);
	extern void MessageBox(unsigned char mode, const char *dStr);

#ifdef __cplusplus
}
#endif

#endif
