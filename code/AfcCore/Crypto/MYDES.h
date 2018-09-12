#ifndef _INCLUDE_MYDES_H_
#define _INCLUDE_MYDES_H_

#ifdef __cplusplus
extern "C" {
#endif
	/*
	Key     ������SESLK��3DES����Կ��������Ȧ��ʱ����SESLKʱ��3DES����ԿΪ��Ȧ������Կ��(������˻���Ϣ)��
	pInData1���μӼ���SESLK��Դ���ݣ�������Ȧ��ʱpInData1Ϊ  α�������ICC��||���Ӵ�������������Ż����Ǯ�������������||��8000��
	pInData2���μӼ���MAC��Դ���ݣ�������Ȧ��ʱpInData2Ϊ  ���Ӵ���������ǰ�����ߵ���Ǯ��������ǰ��||���׽�� || �������ͱ�ʶ || �ն˻���š�
	data2len��pInData2�ĳ��ȣ�
	initData��8 bytes��0x00 ���飻
	mac     ����Ҫ���������MACֵ�����
	*/
	extern void Mac(unsigned char *Key, unsigned char *pInData1, unsigned char *pInData2, int data2len, unsigned char *initData, unsigned char *mac);
	//8�ֽ���Կ
	/************************************************************************/
	/* key : ��Կ 8�ֽ�
	   inittext : ��ʼֵ 8�ֽ�
	   Source : ����ܵ�����
	   le : �������ݵĳ���
	   destin : ���ܺ�Ľ�� 4�ֽ�
																		 */
																		 /************************************************************************/
	extern void SCountMac(unsigned char *key, unsigned char *inittext, unsigned char *Sdat, unsigned int le, unsigned char *destin);
	//16�ֽ���Կ
	/************************************************************************/
	/* key : ��Կ 16�ֽ�
	   inittext : ��ʼֵ  8�ֽ�
	   Source : ����ܵ�����
	   le : �������ݵĳ���
	   destin : ���ܺ�Ľ�� 4�ֽ�
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
