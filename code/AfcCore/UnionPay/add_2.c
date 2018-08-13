

/**************************************************
* �ļ��� :add_2.c
* ����   :
* ����   :
**************************************************/
#include "string.h"
#include "stdio.h"
#include "..\include\include.h"
#include "szct.h"

#include "citydifferent.h"
#include "SL8583.h"
#ifdef qPBOC_BUS
#include "qpboc_head.h"
#endif
#include "qpboc_8583.h"
#include "add.h"
#include "add_2.h"
#include "ProcCharacter.h"
extern unsigned int c_serial;
extern stcomtime SysTime;
extern unsigned char gchn_eng;//��Ӣ�ı�־
extern TCP_IP_PACKET1 tcpipPacket;
extern time_t BCDTime2Long(unsigned char*timee);
//********************************************************
// 
//
//
//******************************************************
extern unsigned char SWITCH_(unsigned char *temp,unsigned char len,unsigned char *data);
///////////////////////////
//
//��64k�洢�Ͽ�������Ϣ
#define RECORD_LEN_add 512  //��¼����

#define FLASH1_START_add     FLASH_RE //�洢��ʼ�ĵ�ַ
#define FLASH2_START1_add    FLASH_RE_END //�洢�����ĵ�ַ
#define FLASH1_size			1       //һ���ж��ٸ�64K
#define FLASH_RCLR_add    0x6000  //��¼����˵��������յ�ǰ��������
#define FLASH_MAX_add      0x8000  //�������洢��ַ�����˾�Ҫ����
#define LEN_EEPROM_add     0x10000 //���洢��ַ





unsigned int getRECORD_LEEN_add(void)
{
	return RECORD_LEN_add ;
}
//�������ʽ��ǰ������
void lib_clr_Record_point_add(void)
{
	unsigned char buff[128];
	unsigned int crc32;
	unsigned int temp,uit;
#ifdef _debug_
	debugstring("lib Clr Record!!\r\n");
	beep(1,200,200);
#endif
	temp =FLASH1_START_add;
	for(uit=0; uit<FLASH1_size; uit++){
		clr_wdt();
		flasherase(ERASE64KBYTESECTOR, temp);
		temp+=0x10000;
		delayxms(2);
	}
	memset(buff, 0xEE, 64);
	memcpy(buff, "BDBD�˿��Ѿ�����! ", 18);
	//	debugstring("lib Clr 1 \r\n");
	crc32 = GenerateCRC32(buff, 60);
	//	debugstring("lib Clr 2 \r\n");
	memcpy(buff+60, (unsigned char*)&crc32, 4);
	flashwrite(FLASH1_START_add, buff, 64);
	//	debugstring("lib Clr 3 \r\n");
	return;
}
unsigned int RecRomCheckFF_add(unsigned char *datt, unsigned int len)
{
	unsigned int i;
	unsigned int counter = 0;
	
	for(i=0; i<len; i++){
		if(datt[i] == 0xFF){
			counter++;
		}
	}
	return counter;
}
unsigned int RecRomCheck00_add(unsigned char *datt, unsigned int len)
{
	unsigned int i;
	unsigned int counter = 0;
	
	for(i=0; i<len; i++){
		if(datt[i] == 0){
			counter++;
		}
	}
	return counter;
}
//ȡ��¼ָ��
//������Ҫд���ָ��,��������ͷָ��
//mode == 1 ֻ�����Ƿ���
int Get_Record_point_add(unsigned char *headp, unsigned char mode)
{
	unsigned char buff[RECORD_LEN_add];
	unsigned char tempbuf[RECORD_LEN_add];
	unsigned char pbuf0[RECORD_LEN_add];
	unsigned int crc32;
	unsigned int temp,uit;
	unsigned int addrF,current;
	unsigned int curdar,enddar,firstdar;
	unsigned int RECORDLEN;
	
	RECORDLEN = getRECORD_LEEN_add();
	
	temp=FLASH1_START_add;
	for(uit=0; uit<FLASH1_size; uit++){//ȡ����ʶ����ʶ���ĳ��Ⱥͼ�¼����һ��
		flashread(temp, buff, 64);
		flashread(temp, tempbuf, 64);
		if(memcmp(buff, tempbuf, 64) == 0)
			break;
	}
	if(uit >= FLASH1_size){
		debugstring("��ʶ��������:");
	}
	crc32 = GenerateCRC32(buff, 60);
	memcpy((unsigned char*)&uit, buff+60, 4);
	
// #ifdef _debug_
// 	debugstring("G Crc32:");
// 	debugdata((unsigned char *)&crc32, 4, 1);
// 	debugdata(buff, 64, 1);
// #endif

	if((crc32 == uit)&&(memcmp(buff, "BDBD�˿��Ѿ�����! ", 18) == 0)){//�Ѿ�����
		if(mode == 0x5a){//ֻ��Ҫ�����Ƿ��пռ䣬�鿴����ַ����
			temp = FLASH1_START_add+FLASH_MAX_add;//�鿴5100����¼�Ƿ���ȫFF,���Ǿ���Ϊ������
			memset(buff, 0, RECORDLEN);
			flashread(temp, buff, RECORDLEN);
			if(RecRomCheckFF_add(buff, RECORDLEN) > (RECORDLEN-4)){//ֻҪ����ȫ�������в���FF��С��4�������ǿյ�
				return 0xff;
			}
			else{
				temp += RECORDLEN;
				memset(buff, 0, RECORDLEN);
				flashread(temp, buff, RECORDLEN);
				if(RecRomCheckFF_add(buff, RECORDLEN) > (RECORDLEN-4)){
					return 0xff;
				}
				return -2;
			}
		}
	}
	else{//û�����ã�����
#ifdef _debug_
		debugstring("����");
#endif
		temp = FLASH1_START_add;
		for(uit=0; uit<FLASH1_size; uit++){//320KΪ5��
			flasherase(ERASE64KBYTESECTOR, temp);
			temp+=0x10000;
			Delay(2);
			//delayxms(2);
		}
		memset(buff, 0xEE, 64);
		memcpy(buff, "BDBD�˿��Ѿ�����! ", 18);
		crc32 = GenerateCRC32(buff, 60);
		memcpy(buff+60, (unsigned char*)&crc32, 4);
		flashwrite(FLASH1_START_add, buff, 64);

		temp = FLASH1_START_add+RECORDLEN;
		memcpy(headp, (unsigned char*)&temp, 4);
		return temp;
	}
//���ҵ�ǰ��Ҫд��ĵ�ַ
	temp = FLASH1_START_add+64;
	memset(tempbuf, 0xff, RECORDLEN);
	memset(pbuf0, 0, RECORDLEN);
	addrF = 0;
	firstdar = 0;
	curdar = 0;
	enddar = ((LEN_EEPROM_add/RECORDLEN)-50); //FLASH_REC_MAX;
	while(1){//���ҵ�ǰָ��
		curdar = ((enddar-firstdar)/2)+firstdar;
		temp = curdar*RECORDLEN;
		temp += (FLASH1_START_add+64);

		clr_wdt();
		flashread(temp, buff, RECORDLEN);
// #ifdef _debug_
// 		debugstring("temp:");
// 		debugdata((unsigned char *)&temp, 4, 1);
// 		debugstring("firstdar:");
// 		debugdata((unsigned char *)&firstdar, 4, 1);
// 		debugstring("curdar:");
// 		debugdata((unsigned char *)&curdar, 4, 1);
// 		debugstring("enddar:");
// 		debugdata((unsigned char *)&enddar, 4, 1);
// 		debugstring("buff:");
// 		debugdata(buff, RECORDLEN, 1);
// #endif
		if(RecRomCheckFF_add(buff, RECORDLEN) > (RECORDLEN-4)){
//		if(memcmp(buff, tempbuf, RECORDLEN) == 0){
			if(firstdar==curdar)//�Ѿ��ҵ�
				break;
			else
			{
				enddar=curdar;
			}
		}
		else{
			if(enddar==curdar)//�Ѿ��ҵ�,û�пռ��ˡ���Ҫ�ɼ�
				break;
			else
			{
				firstdar=curdar+1;
			}
		}
	}
	
// #ifdef _debug_
// 	debugstring("temp  temp:");
// 	debugdata((unsigned char *)&temp, 4, 1);
// #endif
	current = temp; //���ǵ�ǰָ��
	
// #ifdef _debug_
// 	debugstring("----------------------\r\ntemp:");
// 	debugdata((unsigned char *)&temp, 4, 1);
// 	debugstring("----------------------\r\n");
// #endif
//	����Ҫ��ͷָ��
	addrF = 0;
	firstdar = 0;
	curdar = 0;
	enddar = current - (FLASH1_START_add+64);
	enddar /= RECORDLEN; //��ǰ������

	while(1){
		curdar = ((enddar-firstdar)/2)+firstdar;
		temp = curdar*RECORDLEN;
		temp += (FLASH1_START_add+64);
		
		clr_wdt();
		flashread(temp, buff, RECORDLEN);
// #ifdef _debug_
// 		debugstring("temp:");
// 		debugdata((unsigned char *)&temp, 4, 1);
// 		debugstring("firstdar:");
// 		debugdata((unsigned char *)&firstdar, 4, 1);
// 		debugstring("curdar:");
// 		debugdata((unsigned char *)&curdar, 4, 1);
// 		debugstring("enddar:");
// 		debugdata((unsigned char *)&enddar, 4, 1);
// 		debugstring("buff:");
// 		debugdata(buff, RECORDLEN, 1);
// #endif
		if(RecRomCheck00_add(buff, RECORDLEN) < (RECORDLEN-2)){
//		if(memcmp(buff, pbuf0, RECORDLEN) != 0){
			if(firstdar==curdar)//�Ѿ��ҵ�
				break;
			else
			{
				enddar=curdar;
			}
		}
		else{
			if(enddar==curdar)//�Ѿ��ҵ�,û�пռ��ˡ���Ҫ�ɼ�
				break;
			else
			{
				firstdar=curdar+1;
			}
		}

	}
	addrF = temp;
	if(addrF == 0)
		addrF = 64;	//��ʶ������64�ֽ�
	memcpy(headp, (unsigned char*)&addrF, 4);

// #ifdef _debug_
// 	debugstring("headp point:");
// 	debugdata(headp, 4, 1);
// 	debugstring("current point:");
// 	debugdata((unsigned char*)&current, 4, 1);
// #endif
	return current;
}

unsigned int cal_space_add(void)
{
	int headp, curp;
	
	curp = Get_Record_point_add((unsigned char*)&headp, 0x5a);
	if (curp == -2)
		return 0;
	else
		return 0xff;
}

unsigned char libWriteRecord_add( unsigned char *buffer)
{
	unsigned char i=0;
	unsigned char buff[RECORD_LEN_add];
	unsigned int temp, itt=0;
	unsigned int RECORDLEN;
	 MSG_LOG("do %s\r\n",__FUNCTION__);
	RECORDLEN = getRECORD_LEEN_add();
	
	memset(buff, 0x00, RECORDLEN);
	memcpy(buff, buffer, RECORDLEN);
	
	temp = Get_Record_point_add(buff, 0);
	
#ifdef _debug_
	debugstring("-------------------\r\ncur:");

	debugdata((unsigned char*)&temp, 4, 1);	
	MSG_LOG("��ǰָ��=%04x\r\n",temp);
	memcpy((unsigned char *)&itt, (unsigned char*)&buff, 4);
	MSG_LOG("ͷָ��=%04x\r\n",itt);
	flashread(temp, buff, RECORDLEN);
	debugdata(buff, RECORDLEN, 1);
#endif
	
	for(i=0; i<3; i++) 
	{
		flashwrite(temp, buffer, RECORDLEN);
#ifdef _debug_
		debugstring("-------------------\r\ncur:");
		debugdata((unsigned char*)&temp, 4, 1);
		debugstring("WriteRecord : \r\n");		
		debugdata(buffer,RECORDLEN,1);
#endif
		
		flashread(temp, buff, RECORDLEN);
#ifdef _debug_
		debugstring("WriteRecordCheck ulofset2: \r\n");
		debugdata(buff, RECORDLEN, 1);
#endif
		if(memcmp(buffer, buff, RECORDLEN) == 0) 
		{
		
			return 0;
		}
		else{//д������дȫ01������дʵ���ݣ���д����һ���ݵ�ַ
#ifdef _debug_
			debugstring("write record ERROR!!!\r\n");
			debugdata((unsigned char*)&temp, 4, 1);
			beep(3,300,300);
#endif
			//			spiInit();
			//			delayxms(2);
			if(RecRomCheckFF_add(buff, RECORDLEN) <= (RECORDLEN-4)){//����ȫFF��д�˲��֣���ûд�ԡ���ַ�ۼ�
#ifdef _debug_
				debugstring("@@@@@@@@@@@@@@@@@@@@");
				debugdata(buff, RECORDLEN, 1);
#endif
				memset(buff, 0x01, RECORDLEN);
				flashwrite(temp, buff, RECORDLEN);
				temp += RECORDLEN;
				delayxms(2);
			}
			else{//����ȫFF��ûд��ȥ�����ۼӵ�ַ
				if(i>0)
					i--;	//���Բ�ͣ��д�����д�˺���ȫFF.
				itt++;
				if(itt > 10){	//д10�β��ɹ������˳���
					i = 3;
					break;
				}
				delayxms(1);
#ifdef _debug_
				debugstring("$$$$$$$$$$$$$$$$$$$");
				debugdata((unsigned char*)&itt, 4, 1);
#endif
			}
		}
	}

	cls();
	display(2, 2, "���� Data Flash Error!", 1);
	memset(buffer, 0, 20);
	memcpy(buffer, "ADDR:", 5);
	BCD2Ascii((unsigned char *)&temp, buffer+5, 4);
	display(6,0,(char*)buffer, 0);
	led_on(LED_RED|LED_GREEN);
	for(;;)
		delayxms(50);
}




unsigned char FeRC_Dlelt_add(void)
{
	unsigned int  headp=0;//curp=0,
	unsigned int RECORDLEN=0;
	unsigned int addr;
	unsigned char disbuf[RECORD_LEN_add],disbuf_1[RECORD_LEN_add];
	RECORDLEN = getRECORD_LEEN_add();
	//curp =
	Get_Record_point_add((unsigned char*)&headp, 0);
	addr=headp;
	if(addr < FLASH2_START1_add){//��ǰ����д0,��ʷ����д0
		memset(disbuf, 0, RECORDLEN);
		flashwrite(addr, disbuf, RECORDLEN);//������д��ȫ0��
		
		delayxms(1);
		memset(disbuf_1,0xff,RECORDLEN);
		flashread(addr,disbuf_1,RECORDLEN);
		MSG_LOG("������==����\r\n");
		BCD_LOG(disbuf_1,RECORDLEN,1);
		if (memcmp(disbuf,disbuf_1,RECORDLEN)!=0)
		{
			MSG_LOG("дʧ��\r\n");
			return 1;
		}
	}

	return 0;
}
int GJRec_Send_add(void)
{

	unsigned int sum;
	unsigned int curp, headp;
	unsigned int RECORDLEN;
		unsigned char tmpChs[10];
	RECORDLEN = getRECORD_LEEN_add();
		curp = Get_Record_point_add((unsigned char*)&headp, 0);
		//�����¼������û�м�¼�ɷ�������E2PROM��û�з��ɹ��ļ�¼
		if (curp > headp)
		{
			sum = (curp - headp) / RECORDLEN;
#ifdef _debug_
			debugstring("�м�¼!\r\n");
#endif		
						flashread(headp, tmpChs, 4);
			MSG_LOG("ǰ��4���ֽڸ�ʽ:");
			BCD_LOG(tmpChs, 4, 1);
			MSG_LOG("ָ��״̬:");
			if (memcmp(tmpChs, "\x55\xAA", 2) != 0) {
				sum=0;
				
				save_infor_add(FeRC_Dlelt, NULL);
		}
		}
		else {
			sum = 0;
			if (curp >= (FLASH1_START_add + FLASH_RCLR_add)) {//0x3E800�����¼��ַ�Ѿ��ﵽ��40000����64�ֽڣ��������ô����������¿�ʼ
				//clr_Record_point();
				lib_clr_Record_point_add();//ԭ���õ��� clr_Record_point();   ���������ѱ������ĵ�ַҲ�������
				
			}
			
		}
	
	return sum;
}


int save_infor_add(unsigned char mode, unsigned char *re_infor) {


//	unsigned char buff[512];
//	unsigned int addr = 0;
	unsigned int curp=0, headp=0;
	unsigned int RECORDLEN=0;
	int re_inforLen = 0;
	//	int tmpI = 0;
	//	stTlv tlvCardStyle;
//#ifdef _debug_
	//unsigned char buff_crc[512];
//#endif
	int retCode = Ret_OK;
	if (mode != ODA_FeRC_Check) {
		MSG_LOG("save_repurse_infor(%02X):%d\n", mode, re_inforLen);
	}
	switch (mode)
	{
	case ODA_FeRC_Init://��ʼ��
		MSG_LOG("��ʼ��Ŀ¼�ṹ\n ");
		lib_clr_Record_point_add();

		break;
	case ODA_FeRC_Write://д��¼
		MSG_LOG("дFLAHS 8583��¼:");
		re_inforLen = GET_INT16(re_infor + 2) + 4;
		MSG_LOG("��¼:%d\n", re_inforLen);
		if (re_inforLen > 512) {
			MSG_LOG("��¼�޷�д�볬��512B�ļ�¼:%d\n", re_inforLen);
			retCode = Ret_Err_Overflow;
			break;
		}
		libWriteRecord_add(re_infor);
		break;
	case ODA_FeRC_Dlelt://ɾ��
		MSG_LOG("ɾODA8583��¼\n ");
	
		if (FeRC_Dlelt_add()==1) {
		
			retCode = Ret_Err_Format;
			break;
		}

		break;
	case ODA_FeRC_Check://�鿴�Ƿ��м�¼
		//MSG_LOG("C");

		
		if (GJRec_Send_add()!=0) {
					MSG_LOG("�м�¼\r\n");
			retCode = Ret_YES;
		}
		else
		{
			//MSG_LOG("�鿴�Ƿ��м�¼\r\n");
			retCode = Ret_NO;
			break;
		}
		break;
	case ODA_FeRC_READ://

		MSG_LOG("����¼\r\n");
		RECORDLEN = getRECORD_LEEN_add();
		curp = Get_Record_point_add((unsigned char*)&headp, 0);
		flashread(headp, re_infor, RECORDLEN);

#ifdef _debug_
	debugstring("-------------------\r\ncur:");
	
	MSG_LOG("��ȡ��ǰָ��=%04x\r\n",headp);

	MSG_LOG("ͷָ��=%04x\r\n",curp);
	debugdata(re_infor, RECORDLEN, 1);
// 	memcpy( buff_crc,re_infor,512);
// 	memset(buff_crc,re_infor[30],512);
// 	if(memcpy(buff_crc,re_infor,512)!=0)
// 	{
// 		beep(4,200,200);
// 		MSG_LOG("��������\r\n");
// 	}
#endif

		re_inforLen = GET_INT16(re_infor + 2) + 4;
		if (re_inforLen > 512) {
			MSG_LOG("��¼������512 B�ļ�¼:%d\n", re_inforLen);
			retCode = Ret_Err_Overflow;
			break;
		}
		break;
	default:
			MSG_LOG("����\r\n");
		break;
	}

	return retCode;
}
//δ�ϴ�
void showGPRStR_add_2(void)
{
#ifndef NO_MOBILE_24G_
	unsigned int ltemp1;
	unsigned int sendenda;
#endif
	unsigned int ltemp;
	int headp, curp;

	char buffer[50];
	cls();
	curp = Get_Record_point_add((unsigned char*)&headp, 0);
	ltemp = 0;
	ltemp = curp - headp;
	ltemp /= RECORD_LEN_O;
#ifndef NO_GJ_SERVISE
	if (tcpipPacket.packetStatus == RECORD_DEAL)
	{
		if (gchn_eng == 'E')
			sprintf(buffer, "card:%d/1", ltemp);
		else
			sprintf(buffer, "δ����¼:%d/1", ltemp);
	}
	else
	{
		if (gchn_eng == 'E')
			sprintf(buffer, "card:%d/0", ltemp);
		else
			sprintf(buffer, "δ����¼:%d/0", ltemp);
	}
#else
	if (gchn_eng == 'E')
		sprintf(buffer, "IC card:%d/0", ltemp);
	else
		sprintf(buffer, "IC����:%d/0", ltemp);
#endif	
	display(0, 0, buffer, 0);

	// #ifdef qPBOC_BUS
	// 	curp = qPbocGet_Record_point((unsigned char*)&headp, 0);
	// 	ltemp=0;
	// 	ltemp=(curp - headp)/256;
	// 	sprintf(buffer,"������:%d",ltemp);
	// 	display(2,0,buffer,0);
	// #endif

	while (getkey(1) == 0)
	{
		if (profile(0) != ST_ERROR)
			break;//2003.9.15
		delayxms(100);
	}
}
/*******************************************/
extern void save_repurse_infor(unsigned char mode, unsigned char *re_infor);
extern unsigned char read_re_infor(unsigned char *out_infor, int *pOlen);
void write_re_flash(void)
{
	unsigned char infor_temp[1000];
	int inforLen =1000;
	unsigned short len_infor=0;
	memset(infor_temp,0,sizeof(infor_temp));
	if (get_repurse_num() == ST_OK  )  //�����Q_pos_infor.flge!=0xea
	{
	//	clr_wdt();
		delayxms(1);
		read_re_infor(infor_temp+4, &inforLen);
		delayxms(1);
		
		len_infor=inforLen;
		over_turn(2,(unsigned char *)&len_infor);
		memcpy(infor_temp, "\x55\xAA", 2);
		memcpy(infor_temp+2, (unsigned char *)&len_infor, 2);
		save_infor_add(ODA_FeRC_Write,infor_temp);

			MSG_LOG("ɾ����--\r\n");
		memset(repurse_infor, 0, sizeof(repurse_infor));
		save_repurse_infor(FeRC_Dlelt, NULL);
		init_timeout_infor(); //�������ʱ����
	}
		else
		{
				MSG_LOG("û�г���--\r\n");
		}
	
}

unsigned char  over_GPRStR_add2(void)
{

	int headp, curp;
	curp = Get_Record_point_add((unsigned char*)&headp, 0);
	MSG_LOG("headp=%d\r\n",headp);
	MSG_LOG("curp=%d\r\n",curp);
	if(curp-headp>(2*RECORD_LEN_add))
	{
		MSG_LOG("��������3��\r\n");
		return ST_ERROR;
	}
	else
		return ST_OK;
}

void add_2_init_sum(void)
{
	#ifdef switch_RE
	write_re_flash();//�����ж���û�г���
	#endif	
}
void add_2_init_sum_frist(void)
{
	
	lib_clr_Record_point_add();//��������װ��
	//		lib_clr_Record_point_2(FLASH1_START_2,FLASH_size); //����
}
