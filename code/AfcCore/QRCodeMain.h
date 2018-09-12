#ifndef _INCLUDE_QRCODEMAIN_H_
#define _INCLUDE_QRCODEMAIN_H_


#ifdef __cplusplus
extern "C" {
#endif

	extern void BuildQRCRecorde(unsigned char delType, unsigned char *orecBuf, unsigned char *iQRCdat, unsigned int iQRCdatLen, unsigned char pbocResult);
	extern void *main_tidgetQRCode(void *arg);
	extern void main_QRCode_Deal(void);

#ifdef __cplusplus
}
#endif

#endif
