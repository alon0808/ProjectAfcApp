#ifndef _INCLUDE_MYDES_H_
#define _INCLUDE_MYDES_H_

#ifdef __cplusplus
extern "C" {
#endif
	/*
	Key     ：计算SESLK的3DES的密钥，比如在圈存时计算SESLK时，3DES的密钥为“圈存子密钥”(详见个人化信息)；
	pInData1：参加计算SESLK的源数据，比如在圈存时pInData1为  伪随机数（ICC）||电子存折联机交易序号或电子钱包联机交易序号||“8000”
	pInData2：参加计算MAC的源数据，比如在圈存时pInData2为  电子存折余额（交易前）或者电子钱包余额（交易前）||交易金额 || 交易类型标识 || 终端机编号。
	data2len：pInData2的长度；
	initData：8 bytes的0x00 数组；
	mac     ：需要计算出来的MAC值结果。
	*/
	extern void Mac(unsigned char *Key, unsigned char *pInData1, unsigned char *pInData2, int data2len, unsigned char *initData, unsigned char *mac);
	//8字节密钥
	/************************************************************************/
	/* key : 密钥 8字节
	   inittext : 初始值 8字节
	   Source : 需加密的数据
	   le : 加密数据的长度
	   destin : 加密后的结果 4字节
																		 */
																		 /************************************************************************/
	extern void SCountMac(unsigned char *key, unsigned char *inittext, unsigned char *Sdat, unsigned int le, unsigned char *destin);
	//16字节密钥
	/************************************************************************/
	/* key : 密钥 16字节
	   inittext : 初始值  8字节
	   Source : 需加密的数据
	   le : 加密数据的长度
	   destin : 加密后的结果 4字节
																		 */
																		 /************************************************************************/
	extern void CountMac(const char *key, unsigned char *inittext, unsigned char *Sdat, unsigned int le, unsigned char *destin);
	extern unsigned char getJiuJiang_CPUKey(unsigned char *Sdata, unsigned char *Rkey);
	extern void getCPUMAC(unsigned char mode, unsigned char *inittext, unsigned char *Source, unsigned int le, unsigned char *destin);

	extern void CountMac1(unsigned char *Key, unsigned char *random, unsigned char *Sdat, unsigned int len, unsigned char *Result);

#ifdef __cplusplus
}
#endif

#endif
