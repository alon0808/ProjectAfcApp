

/**************************************************
* �ļ��� :add.c
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
//  �������غ��� BCDTime2Long
//
//
//******************************************************
qpboc_pos_infor Q_pos_infor;  //ȫ�ֱ���
void init_Q_pos_infor(void)
{
	memset((unsigned char *)&Q_pos_infor,0,sizeof(qpboc_pos_infor));
#ifndef switch_oda
	if (switch_both)
	{
		shuangmian=1;
	}
#endif
}
void set_pos_infor_1(unsigned char mode)
{
	if(switch_both)
	{
		Q_pos_infor.flge = mode;
		shuangmian = 0;       //��ʱ�е��ѻ�
	}
}

#define Q_T  3
unsigned char get_q_(unsigned char mode)
{
	int t1,t2;
   //�ж��Ƿ���Ҫ�����
 
	if(	switch_both==1 && Q_pos_infor.flge==0xad)
	{

		t1 = BCDTime2Long(&SysTime.year_h);
		t2= BCDTime2Long(Q_pos_infor.time_last);

		if (mode==1)  //ֻ�ж�ʱ��
		{
				MSG_LOG("==����ʱ��%d\r\n",t1);
				MSG_LOG("==��һ��ʱ��%d\r\n",t2);
			if(t1-t2>=Q_T)
			{
				MSG_LOG("==����ʱ��%d\r\n",t1);
				MSG_LOG("==��һ��ʱ��%d\r\n",t2);
				Q_pos_infor.flge=0;
			}
		}
		else  //ʱ������Ŷ��ж�
		{
			MSG_LOG("����ʱ��%d\r\n",t1);
			MSG_LOG("��һ��ʱ��%d\r\n",t2);
            

			if( memcmp(Q_pos_infor.c_serial_qpoc,(unsigned char *)&c_serial,4)==0 )  //
			{
				
				return ST_OK;
			   
			}
			else if( memcmp(Q_pos_infor.c_serial_qpoc,(unsigned char *)&c_serial,4) !=0 && t1-t2<=Q_T)
			{
				return ST_ERROR;
			}
			else
			{
				Q_pos_infor.flge=0;
				return ST_OK;
			
			}
		}
		

	}
	else
	{
			return ST_OK;
	}

	return ST_OK;
}

unsigned char SWITCH_(unsigned char *temp,unsigned char len,unsigned char *data)
{
//	unsigned char BLK_TEMP[30];
	unsigned char BLK_TEMP_1[30];
	unsigned int uitemp;
	
   memcpy((unsigned char *)&uitemp,temp,4);
//uitemp-=1;
	if (len==6)
	{
		sprintf((char *)BLK_TEMP_1,"%06u",uitemp);//��ʮ������ת��ASCII��ʮ����
		memcpy(data,BLK_TEMP_1,len);
		
#ifdef _debug_

		debugstring((char *)BLK_TEMP_1);
		debugstring("\r\n");
	
#endif
	}
	else if (len==8)
	
	{
		sprintf((char *)BLK_TEMP_1,"%08u",uitemp);//��ʮ������ת��ASCII��ʮ����
		memcpy(data,BLK_TEMP_1,len);
		
#ifdef _debug_
	
		debugstring((char *)BLK_TEMP_1);
		debugstring("\r\n");

#endif
	}


return ST_OK;

}
//
//////////////////////
//��ʱ����
/////////////////////


void set_over_time(unsigned char mode,unsigned char len){

	unsigned char U_time=0;
    unsigned int i;
	char buffer[30], buffer_2[30];
char U_format[30];
	unsigned char ret;
	getMobileParameter(mode, (unsigned char *)&U_time);
	
///////////////


	i=0;
	memset(U_format,0,sizeof(U_format));
	U_format[0]='%';
	U_format[1]='0';
	sprintf(U_format+2,"%d",len);
	strcat(U_format,"d");
	MSG_LOG("��ʽ=%s\r\n",U_format);
	cls();
	display(0,2,"���ó�ʱʱ��",DIS_CENTER);
	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,U_format,U_time);
	sprintf(buffer_2,U_format,U_time);
	display(2,4,(char*)buffer_2,0);
	while(1)
	{

		delayxms(10);
		memset(buffer_2,0,sizeof(buffer));
		buffer_2[0]=buffer[i];
		display(2,4+i,(char*)&buffer_2,DIS_CONVERT);
		ret =getkey(1);
		switch(ret)
		{

		case UP:
			if((buffer[i]<='0')||(buffer[i]>'9'))
				buffer[i]='9';
			else if((buffer[i]<='9')&&(buffer[i]>'0'))
				buffer[i]--;
			break;

		case ESC:
			//memcpy(pFistVary.DeviceNo, pFistVary.DeviceNo_1, 8);
			goto SetupDevi_exit;
		case ENTER:

			i++;
			memset(buffer_2,0,10);
			memcpy(buffer_2,buffer,len);
			display(2,4,(char*)buffer_2,0);
			if(i >=len){
				U_time=Ascii2INT((unsigned char *)buffer,2);
			MSG_LOG("����=%d\r\n",U_time);
			//	Ascii2BCD(buffer,(unsigned char *)&U_time,2);
			   saveMobileParameter(mode,(unsigned char *)&U_time);
			   display(3,3,"���óɹ�",DIS_CENTER);
			   delayxms(1500);
			   MSG_LOG("����\r\n");
				goto SetupDevi_exit;
			}
			break;
		default:
			break;
		}
	}
SetupDevi_exit:

}

unsigned char get_over_time_re(void)
{
	unsigned char out_data=0;
	if(getMobileParameter(15,(unsigned char *)&out_data)==ST_OK)
	{
		return out_data;
	}
	else
	{
	//	MSG_LOG("��ȡʧ�ܣ�ȡĬ��\r\n");
		return time_over_re;
	}
	
}
unsigned char get_over_time_shuang(void)
{
	unsigned char out_data=0;
	if(getMobileParameter(14,(unsigned char *)&out_data)==ST_OK)
	{
		return out_data;
	}
	else
	{
		MSG_LOG("��ȡʧ�ܣ�ȡĬ��\r\n");
		return time_over_shuangmian;
	}
}
unsigned char get_SOCKET_OVER_TIME(void)
{
	unsigned char out_data=0;
	if(getMobileParameter(21,(unsigned char *)&out_data)==ST_OK)
	{
		return out_data;
	}
	else
	{
		MSG_LOG("��ȡʧ�ܣ�ȡĬ��\r\n");
		return time_SOCKET_OVER_TIME;
	}
}
void set_over_time_re(void)
{
	set_over_time(15,2);
	Q_QPBOC_para.chongzheng_OVER_TIME=get_over_time_re();

}
void set_over_time_shuang(void)
{
	set_over_time(14,2);

	Q_QPBOC_para.shuangmian_OVER_TIME=get_over_time_shuang();

}
void set_SOCKET_OVER_TIME(void)
{
	set_over_time(21,2);

	Q_QPBOC_para.SOCKET_OVER_TIME=get_SOCKET_OVER_TIME();

}

qpboc_pos_infor timeout_infor;  //ȫ�ֱ���
void init_timeout_infor(void)
{
	memset((unsigned char *)&timeout_infor,0,sizeof(qpboc_pos_infor));
}

//�л���־
void Switch_sign(unsigned char mode)
{
// 	if(shuangmian==1)
// 	{
		timeout_infor.flge = mode;
//	}
}
extern void DisRetry2(void);
 extern unsigned char restore_flag;
unsigned char Judge_timeout(unsigned char mode,unsigned int time_out,qpboc_pos_infor *timeout_infor,unsigned char switch_over )//�жϳ�ʱ.switch_over���Ƿ���Ҫ�ж� =1,��Ҫ�ж�
{
	int t1,t2;
	qpboc_pos_infor  * U_timeout_infor;
//	unsigned char disbuff[10];
	//memcpy((unsigned char *)&U_timeout_infor,(unsigned char *)timeout_infor,sizeof(qpboc_pos_infor));
   
	U_timeout_infor=timeout_infor;
	//�ж��Ƿ�ʱ
 	if(	switch_over==1 && U_timeout_infor->flge==Switch_sign_OVER )
	{

		t1 = BCDTime2Long(&SysTime.year_h);
		t2= BCDTime2Long(U_timeout_infor->time_last);
		if (mode==1)  //ֻ�ж�ʱ��
		{

				if(switch_both==1)
				{
					MSG_LOG("==����ʱ��%d\r\n",t1);
					MSG_LOG("==��һ��ʱ��%d\r\n",t2);
					DisRetry2();
					restore_flag=0;
				}
			


			if(t1-t2>time_out+3)
			{
			//	MSG_LOG("==����ʱ��%d\r\n",t1);
			//	MSG_LOG("=xx=��һ��ʱ��%d\r\n",t2);
				U_timeout_infor->flge=0;
				restore_flag=3;
				return ST_OK;
			}
			else
			{
				return ST_ERROR;
				
			}
		}
		else if(mode==2) //ʱ������Ŷ��ж�
		{
			MSG_LOG("222����ʱ��%d\r\n",t1);
			MSG_LOG("22��һ��ʱ��%d\r\n",t2);
            
			//��ʱֻ�ж��п������Զ����������������
			if( memcmp(U_timeout_infor->c_serial_qpoc,(unsigned char *)&c_serial,4)==0 )  //
			{
				U_timeout_infor->flge=0;
				return ST_OK;
			   
			}
// 			else if( memcmp(U_timeout_infor->c_serial_qpoc,(unsigned char *)&c_serial,4) !=0 && t1-t2<=time_out)
// 			{
// 				return ST_ERROR;
// 			}
			else
			{
				U_timeout_infor->flge=0;
				return ST_OK;
			
			}
		}

		else
		{
			
			if(switch_both==1)
			{
				if( memcmp(U_timeout_infor->c_serial_qpoc,(unsigned char *)&c_serial,4)!=0 )  //
				{
					MSG_LOG("��ͬ��\r\n");
					U_timeout_infor->flge=0;
					return ST_OK;
					
				}
				else
				{
					MSG_LOG("ͬ��\r\n");
					//	U_timeout_infor->flge=0;
					return ST_ERROR;
					
				}
			}
			else
			{
				MSG_LOG("��ͨ������\r\n");
				U_timeout_infor->flge=0;
				return ST_OK;
			}
			
		}
		

	}
	else
	{
		return ST_ERROR;
	}

	//return ST_OK;
}
void save_over_infor(unsigned int uiTemp)
{
// 	if(shuangmian)
// 	{
		memcpy(timeout_infor.NS,(unsigned char *)&uiTemp,4);
		memcpy(timeout_infor.time_last,(unsigned char*)&SysTime, 7);
		memcpy(timeout_infor.c_serial_qpoc,(unsigned char*)&c_serial, 4);  //��ʱ����nfc�ֻ�
//	}
}
void Judge_timeout_re(unsigned char mode,unsigned char switch_over )//mode=1,ֻ�Ƚ�ʱ�䣬2  //���߲Ŵ���
{
	unsigned int time_out=0;
	if(timeout_infor.flge==Switch_sign_OVER )  //��ʱ����
	{
		
		
		time_out=get_over_time_re();
		MSG_LOG("time_out=%d\r\n",time_out);

			
		if(Judge_timeout(mode,time_out,&timeout_infor,1)==ST_OK) //switch_over Ĭ�ϴ�
		{
			
			write_re_flash();
			
		}
	}
}

/**************************************************
* ����	 :	DELAY_SEND
* ����   :  ��ʱ�����񣬷���
* ����   :
**************************************************/

typedef struct
{
	
	unsigned int time;			//
//	unsigned char RES[20];

}DELAY_SEND_STRUCT;
DELAY_SEND_STRUCT delay_send;
void init_delay_send(void)
{
	memset((unsigned char *)&delay_send,0,sizeof(DELAY_SEND_STRUCT));
}
void set_delay_send(unsigned int val)
{
	delay_send.time=val;
}
unsigned int get_delay_send(void)
{
	
	return delay_send.time;
}
void reduce_delay_send(void)
{
	if(delay_send.time>0)
	{
	//	MSG_LOG("��ʱ�������1==%d==\r\n",delay_send.time);
		delay_send.time--;
	}
}
unsigned char  DELAY_SEND(void)
{
	if(delay_send.time==0)
	{
		return ST_OK;
	}
	else
	{
//		MSG_LOG("��ˢ�꿨\r\n");
		return ST_ERROR;
	}
}
//ODA�洢��غ���
///////////////////////////
unsigned int getRECORD_LEEN_O(void)
{
	return RECORD_LEN_O ;
}
//�������ʽ��ǰ������
void lib_clr_Record_point_O(void)
{
	unsigned char buff[128];
	unsigned int crc32;
	unsigned int temp,uit;
#ifdef _debug_
	debugstring("lib Clr Record!!\r\n");
#endif
	temp =FLASH1_START_O;
	for(uit=0; uit<FLASH_SIZE; uit++){
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
	flashwrite(FLASH1_START_O, buff, 64);
	//	debugstring("lib Clr 3 \r\n");
	return;
}
unsigned int RecRomCheckFF_O(unsigned char *datt, unsigned int len)
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
unsigned int RecRomCheck00_O(unsigned char *datt, unsigned int len)
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
int Get_Record_point_O(unsigned char *headp, unsigned char mode)
{
	unsigned char buff[RECORD_LEN_O];
	unsigned char tempbuf[RECORD_LEN_O];
	unsigned char pbuf0[RECORD_LEN_O];
	unsigned int crc32;
	unsigned int temp,uit;
	unsigned int addrF,current;
	unsigned int curdar,enddar,firstdar;
	unsigned int RECORDLEN;
	
	RECORDLEN = getRECORD_LEEN_O();
	
	temp=FLASH1_START_O;
	for(uit=0; uit<FLASH_SIZE; uit++){//ȡ����ʶ����ʶ���ĳ��Ⱥͼ�¼����һ��
		flashread(temp, buff, 64);
		flashread(temp, tempbuf, 64);
		if(memcmp(buff, tempbuf, 64) == 0)
			break;
	}
	if(uit >= FLASH_SIZE){
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
			temp = FLASH1_START_O+FLASH_MAX_O;//�鿴5100����¼�Ƿ���ȫFF,���Ǿ���Ϊ������
			memset(buff, 0, RECORDLEN);
			flashread(temp, buff, RECORDLEN);
			if(RecRomCheckFF_O(buff, RECORDLEN) > (RECORDLEN-4)){//ֻҪ����ȫ�������в���FF��С��4�������ǿյ�
				return 0xff;
			}
			else{
				temp += RECORDLEN;
				memset(buff, 0, RECORDLEN);
				flashread(temp, buff, RECORDLEN);
				if(RecRomCheckFF_O(buff, RECORDLEN) > (RECORDLEN-4)){
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
		temp = FLASH1_START_O;
		for(uit=0; uit<FLASH_SIZE; uit++){//320KΪ5��
			flasherase(ERASE64KBYTESECTOR, temp);
			temp+=0x10000;
			Delay(2);
			//delayxms(2);
		}
		memset(buff, 0xEE, 64);
		memcpy(buff, "BDBD�˿��Ѿ�����! ", 18);
		crc32 = GenerateCRC32(buff, 60);
		memcpy(buff+60, (unsigned char*)&crc32, 4);
		flashwrite(FLASH1_START_O, buff, 64);

		temp = FLASH1_START_O+RECORDLEN;
		memcpy(headp, (unsigned char*)&temp, 4);
		return temp;
	}
//���ҵ�ǰ��Ҫд��ĵ�ַ
	temp = FLASH1_START_O+64;
	memset(tempbuf, 0xff, RECORDLEN);
	memset(pbuf0, 0, RECORDLEN);
	addrF = 0;
	firstdar = 0;
	curdar = 0;
	enddar = ((LEN_EEPROM_O/RECORDLEN)-50); //FLASH_REC_MAX;
	while(1){//���ҵ�ǰָ��
		curdar = ((enddar-firstdar)/2)+firstdar;
		temp = curdar*RECORDLEN;
		temp += (FLASH1_START_O+64);

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
		if(RecRomCheckFF_O(buff, RECORDLEN) > (RECORDLEN-4)){
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
	enddar = current - (FLASH1_START_O+64);
	enddar /= RECORDLEN; //��ǰ������

	while(1){
		curdar = ((enddar-firstdar)/2)+firstdar;
		temp = curdar*RECORDLEN;
		temp += (FLASH1_START_O+64);
		
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
		if(RecRomCheck00_O(buff, RECORDLEN) < (RECORDLEN-2)){
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

unsigned int cal_space_O(void)
{
	int headp, curp;
	
	curp = Get_Record_point_O((unsigned char*)&headp, 0x5a);
	if (curp == -2)
		return 0;
	else
		return 0xff;
}

unsigned char libWriteRecord_O( unsigned char *buffer)
{
	unsigned char i=0;
	unsigned char buff[RECORD_LEN_O];
	unsigned int temp, itt=0;
	unsigned int RECORDLEN;
	 MSG_LOG("do %s\r\n",__FUNCTION__);
	RECORDLEN = getRECORD_LEEN_O();
	
	memset(buff, 0x00, RECORDLEN);
	memcpy(buff, buffer, RECORDLEN);
	
	temp = Get_Record_point_O(buff, 0);
	
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
#endif
			//			spiInit();
			//			delayxms(2);
			if(RecRomCheckFF_O(buff, RECORDLEN) <= (RECORDLEN-4)){//����ȫFF��д�˲��֣���ûд�ԡ���ַ�ۼ�
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
	display(2, 2, "Data Flash Error!", 1);
	memset(buffer, 0, 20);
	memcpy(buffer, "ADDR:", 5);
	BCD2Ascii((unsigned char *)&temp, buffer+5, 4);
	display(6,0,(char*)buffer, 0);
	led_on(LED_RED|LED_GREEN);
	for(;;)
		delayxms(50);
}




unsigned char FeRC_Dlelt_O(void)
{
	unsigned int  headp=0;//curp=0,
	unsigned int RECORDLEN=0;
	unsigned int addr;
	unsigned char disbuf[RECORD_LEN_O],disbuf_1[RECORD_LEN_O];
	RECORDLEN = getRECORD_LEEN_O();
	//curp = 
	Get_Record_point_O((unsigned char*)&headp, 0);
	addr=headp;
	if(addr < FLASH2_START1_O){//��ǰ����д0,��ʷ����д0
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
int GJRec_Send_O(void)
{

	unsigned int sum;
	unsigned int curp, headp;
	unsigned int RECORDLEN;
		unsigned char tmpChs[10];
	RECORDLEN = getRECORD_LEEN_O();
		curp = Get_Record_point_O((unsigned char*)&headp, 0);
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
				
				save_ODA_infor(FeRC_Dlelt, NULL);
		}
		}
		else {
			sum = 0;
			if (curp >= (FLASH1_START_O + FLASH_RCLR_O)) {//0x3E800�����¼��ַ�Ѿ��ﵽ��40000����64�ֽڣ��������ô����������¿�ʼ
				//clr_Record_point();
				lib_clr_Record_point_O();//ԭ���õ��� clr_Record_point();   ���������ѱ������ĵ�ַҲ�������
				
			}
			
		}
	
	return sum;
}


int save_ODA_infor(unsigned char mode, unsigned char *re_infor) {


//	unsigned char buff[512];
//	unsigned int addr = 0;
	unsigned int  headp=0;
	//,curp=0;
	unsigned int RECORDLEN=0;
	int re_inforLen = 0;
	//	int tmpI = 0;
	//	stTlv tlvCardStyle;

	int retCode = Ret_OK;
	if (mode != ODA_FeRC_Check) {
		MSG_LOG("save_repurse_infor(%02X):%d\n", mode, re_inforLen);
	}
	switch (mode)
	{
	case ODA_FeRC_Init://��ʼ��
		MSG_LOG("��ʼ��ODAĿ¼�ṹ\n ");
	lib_clr_Record_point_O();

		break;
	case ODA_FeRC_Write://д��¼
		MSG_LOG("дFLAHS ODA8583��¼:");
		re_inforLen = GET_INT16(re_infor + 2) + 4;
		MSG_LOG("ODA��¼:%d\n", re_inforLen);
		if (re_inforLen > 512) {
			MSG_LOG("ODA��¼�޷�д�볬��512B�ļ�¼:%d\n", re_inforLen);
			retCode = Ret_Err_Overflow;
			break;
		}
		libWriteRecord_O(re_infor);
		break;
	case ODA_FeRC_Dlelt://ɾ��
		MSG_LOG("ɾODA8583��¼\n ");
	
		if (FeRC_Dlelt_O()==1) {
		
			retCode = Ret_Err_Format;
			break;
		}

		break;
	case ODA_FeRC_Check://�鿴�Ƿ��м�¼
		//MSG_LOG("C");

		
		if (GJRec_Send_O()!=0) {
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
		RECORDLEN = getRECORD_LEEN_O();
//		curp =
	Get_Record_point_O((unsigned char*)&headp, 0);
		flashread(headp, re_infor, RECORDLEN);

		re_inforLen = GET_INT16(re_infor + 2) + 4;
		if (re_inforLen > 512) {
			MSG_LOG("ODA��¼������512 B�ļ�¼:%d\n", re_inforLen);
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
void showGPRStR_add(void)
{
#ifndef NO_MOBILE_24G_
	unsigned int ltemp1;
	unsigned int sendenda;
#endif
	unsigned int ltemp;
	int headp, curp;

	char buffer[50];
	cls();
	curp = Get_Record_point_O((unsigned char*)&headp, 0);
	ltemp = 0;
	ltemp = curp - headp;
	ltemp /= RECORD_LEN_O;
#ifndef NO_GJ_SERVISE
	if (tcpipPacket.packetStatus == RECORD_DEAL)
	{
		if (gchn_eng == 'E')
			sprintf(buffer, "ODA card:%d/1", ltemp);
		else
			sprintf(buffer, "ODAδ����¼:%d/1", ltemp);
	}
	else
	{
		if (gchn_eng == 'E')
			sprintf(buffer, "ODA card:%d/0", ltemp);
		else
			sprintf(buffer, "ODAδ����¼:%d/0", ltemp);
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

void add_init_sum(void)
{
	init_Q_pos_infor();//��
	init_timeout_infor(); //�������ʱ����
	init_delay_send();//��ʱ������
}
void add_init_sum_frist(void)
{
	lib_clr_Record_point_O();//oda
//	lib_clr_Record_point_add();//��������װ��
	//		lib_clr_Record_point_2(FLASH1_START_2,FLASH_size); //����
}