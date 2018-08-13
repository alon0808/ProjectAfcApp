

/**************************************************
* 文件名 :add.c
* 描述   :
* 建立   :
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
extern unsigned char gchn_eng;//中英文标志
extern TCP_IP_PACKET1 tcpipPacket;
extern time_t BCDTime2Long(unsigned char*timee);
//********************************************************
//  再请款相关函数 BCDTime2Long
//
//
//******************************************************
qpboc_pos_infor Q_pos_infor;  //全局变量
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
		shuangmian = 0;       //超时切到脱机
	}
}

#define Q_T  3
unsigned char get_q_(unsigned char mode)
{
	int t1,t2;
   //判断是否需要再请款
 
	if(	switch_both==1 && Q_pos_infor.flge==0xad)
	{

		t1 = BCDTime2Long(&SysTime.year_h);
		t2= BCDTime2Long(Q_pos_infor.time_last);

		if (mode==1)  //只判断时间
		{
				MSG_LOG("==现在时间%d\r\n",t1);
				MSG_LOG("==上一笔时间%d\r\n",t2);
			if(t1-t2>=Q_T)
			{
				MSG_LOG("==现在时间%d\r\n",t1);
				MSG_LOG("==上一笔时间%d\r\n",t2);
				Q_pos_infor.flge=0;
			}
		}
		else  //时间跟卡号都判断
		{
			MSG_LOG("现在时间%d\r\n",t1);
			MSG_LOG("上一笔时间%d\r\n",t2);
            

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
		sprintf((char *)BLK_TEMP_1,"%06u",uitemp);//把十六进制转成ASCII的十进制
		memcpy(data,BLK_TEMP_1,len);
		
#ifdef _debug_

		debugstring((char *)BLK_TEMP_1);
		debugstring("\r\n");
	
#endif
	}
	else if (len==8)
	
	{
		sprintf((char *)BLK_TEMP_1,"%08u",uitemp);//把十六进制转成ASCII的十进制
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
//超时函数
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
	MSG_LOG("格式=%s\r\n",U_format);
	cls();
	display(0,2,"设置超时时间",DIS_CENTER);
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
			MSG_LOG("保存=%d\r\n",U_time);
			//	Ascii2BCD(buffer,(unsigned char *)&U_time,2);
			   saveMobileParameter(mode,(unsigned char *)&U_time);
			   display(3,3,"设置成功",DIS_CENTER);
			   delayxms(1500);
			   MSG_LOG("保存\r\n");
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
	//	MSG_LOG("获取失败，取默认\r\n");
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
		MSG_LOG("获取失败，取默认\r\n");
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
		MSG_LOG("获取失败，取默认\r\n");
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

qpboc_pos_infor timeout_infor;  //全局变量
void init_timeout_infor(void)
{
	memset((unsigned char *)&timeout_infor,0,sizeof(qpboc_pos_infor));
}

//切换标志
void Switch_sign(unsigned char mode)
{
// 	if(shuangmian==1)
// 	{
		timeout_infor.flge = mode;
//	}
}
extern void DisRetry2(void);
 extern unsigned char restore_flag;
unsigned char Judge_timeout(unsigned char mode,unsigned int time_out,qpboc_pos_infor *timeout_infor,unsigned char switch_over )//判断超时.switch_over：是否需要判断 =1,需要判断
{
	int t1,t2;
	qpboc_pos_infor  * U_timeout_infor;
//	unsigned char disbuff[10];
	//memcpy((unsigned char *)&U_timeout_infor,(unsigned char *)timeout_infor,sizeof(qpboc_pos_infor));
   
	U_timeout_infor=timeout_infor;
	//判断是否超时
 	if(	switch_over==1 && U_timeout_infor->flge==Switch_sign_OVER )
	{

		t1 = BCDTime2Long(&SysTime.year_h);
		t2= BCDTime2Long(U_timeout_infor->time_last);
		if (mode==1)  //只判断时间
		{

				if(switch_both==1)
				{
					MSG_LOG("==现在时间%d\r\n",t1);
					MSG_LOG("==上一笔时间%d\r\n",t2);
					DisRetry2();
					restore_flag=0;
				}
			


			if(t1-t2>time_out+3)
			{
			//	MSG_LOG("==现在时间%d\r\n",t1);
			//	MSG_LOG("=xx=上一笔时间%d\r\n",t2);
				U_timeout_infor->flge=0;
				restore_flag=3;
				return ST_OK;
			}
			else
			{
				return ST_ERROR;
				
			}
		}
		else if(mode==2) //时间跟卡号都判断
		{
			MSG_LOG("222现在时间%d\r\n",t1);
			MSG_LOG("22上一笔时间%d\r\n",t2);
            
			//暂时只判断有卡来就自动冲正，不做再请款
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
					MSG_LOG("不同卡\r\n");
					U_timeout_infor->flge=0;
					return ST_OK;
					
				}
				else
				{
					MSG_LOG("同卡\r\n");
					//	U_timeout_infor->flge=0;
					return ST_ERROR;
					
				}
			}
			else
			{
				MSG_LOG("单通道在线\r\n");
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
		memcpy(timeout_infor.c_serial_qpoc,(unsigned char*)&c_serial, 4);  //暂时不管nfc手机
//	}
}
void Judge_timeout_re(unsigned char mode,unsigned char switch_over )//mode=1,只比较时间，2  //在线才存在
{
	unsigned int time_out=0;
	if(timeout_infor.flge==Switch_sign_OVER )  //延时上送
	{
		
		
		time_out=get_over_time_re();
		MSG_LOG("time_out=%d\r\n",time_out);

			
		if(Judge_timeout(mode,time_out,&timeout_infor,1)==ST_OK) //switch_over 默认打开
		{
			
			write_re_flash();
			
		}
	}
}

/**************************************************
* 函数	 :	DELAY_SEND
* 描述   :  延时找任务，发送
* 建立   :
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
	//	MSG_LOG("延时找任务减1==%d==\r\n",delay_send.time);
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
//		MSG_LOG("刚刷完卡\r\n");
		return ST_ERROR;
	}
}
//ODA存储相关函数
///////////////////////////
unsigned int getRECORD_LEEN_O(void)
{
	return RECORD_LEN_O ;
}
//清除并格式当前存贮区
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
	memcpy(buff, "BDBD此块已经启用! ", 18);
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
//取记录指针
//返回需要写入的指针,参数返回头指针
//mode == 1 只返回是否满
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
	for(uit=0; uit<FLASH_SIZE; uit++){//取出标识，标识区的长度和记录长度一致
		flashread(temp, buff, 64);
		flashread(temp, tempbuf, 64);
		if(memcmp(buff, tempbuf, 64) == 0)
			break;
	}
	if(uit >= FLASH_SIZE){
		debugstring("标识读出错误:");
	}
	crc32 = GenerateCRC32(buff, 60);
	memcpy((unsigned char*)&uit, buff+60, 4);
	
// #ifdef _debug_
// 	debugstring("G Crc32:");
// 	debugdata((unsigned char *)&crc32, 4, 1);
// 	debugdata(buff, 64, 1);
// #endif

	if((crc32 == uit)&&(memcmp(buff, "BDBD此块已经启用! ", 18) == 0)){//已经启用
		if(mode == 0x5a){//只需要返回是否还有空间，查看最后地址就行
			temp = FLASH1_START_O+FLASH_MAX_O;//查看5100条记录是否是全FF,不是就认为是满的
			memset(buff, 0, RECORDLEN);
			flashread(temp, buff, RECORDLEN);
			if(RecRomCheckFF_O(buff, RECORDLEN) > (RECORDLEN-4)){//只要大于全部数据中不是FF的小于4个就算是空的
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
	else{//没有启用，擦除
#ifdef _debug_
		debugstring("擦除");
#endif
		temp = FLASH1_START_O;
		for(uit=0; uit<FLASH_SIZE; uit++){//320K为5块
			flasherase(ERASE64KBYTESECTOR, temp);
			temp+=0x10000;
			Delay(2);
			//delayxms(2);
		}
		memset(buff, 0xEE, 64);
		memcpy(buff, "BDBD此块已经启用! ", 18);
		crc32 = GenerateCRC32(buff, 60);
		memcpy(buff+60, (unsigned char*)&crc32, 4);
		flashwrite(FLASH1_START_O, buff, 64);

		temp = FLASH1_START_O+RECORDLEN;
		memcpy(headp, (unsigned char*)&temp, 4);
		return temp;
	}
//查找当前需要写入的地址
	temp = FLASH1_START_O+64;
	memset(tempbuf, 0xff, RECORDLEN);
	memset(pbuf0, 0, RECORDLEN);
	addrF = 0;
	firstdar = 0;
	curdar = 0;
	enddar = ((LEN_EEPROM_O/RECORDLEN)-50); //FLASH_REC_MAX;
	while(1){//先找当前指针
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
			if(firstdar==curdar)//已经找到
				break;
			else
			{
				enddar=curdar;
			}
		}
		else{
			if(enddar==curdar)//已经找到,没有空间了。需要采集
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
	current = temp; //这是当前指针
	
// #ifdef _debug_
// 	debugstring("----------------------\r\ntemp:");
// 	debugdata((unsigned char *)&temp, 4, 1);
// 	debugstring("----------------------\r\n");
// #endif
//	还需要找头指针
	addrF = 0;
	firstdar = 0;
	curdar = 0;
	enddar = current - (FLASH1_START_O+64);
	enddar /= RECORDLEN; //当前条数。

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
			if(firstdar==curdar)//已经找到
				break;
			else
			{
				enddar=curdar;
			}
		}
		else{
			if(enddar==curdar)//已经找到,没有空间了。需要采集
				break;
			else
			{
				firstdar=curdar+1;
			}
		}

	}
	addrF = temp;
	if(addrF == 0)
		addrF = 64;	//标识区长度64字节
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
	MSG_LOG("当前指针=%04x\r\n",temp);
	memcpy((unsigned char *)&itt, (unsigned char*)&buff, 4);
	MSG_LOG("头指针=%04x\r\n",itt);
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
		else{//写错，正区写全01，副区写实数据，并写到下一数据地址
#ifdef _debug_
			debugstring("write record ERROR!!!\r\n");
			debugdata((unsigned char*)&temp, 4, 1);
#endif
			//			spiInit();
			//			delayxms(2);
			if(RecRomCheckFF_O(buff, RECORDLEN) <= (RECORDLEN-4)){//不是全FF，写了部分，但没写对。地址累加
#ifdef _debug_
				debugstring("@@@@@@@@@@@@@@@@@@@@");
				debugdata(buff, RECORDLEN, 1);
#endif
				memset(buff, 0x01, RECORDLEN);
				flashwrite(temp, buff, RECORDLEN);
				temp += RECORDLEN;
				delayxms(2);
			}
			else{//还是全FF，没写进去，不累加地址
				if(i>0)
					i--;	//测试不停的写，如果写了后还是全FF.
				itt++;
				if(itt > 10){	//写10次不成功，就退出。
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
	if(addr < FLASH2_START1_O){//当前区才写0,历史区不写0
		memset(disbuf, 0, RECORDLEN);
		flashwrite(addr, disbuf, RECORDLEN);//把这条写成全0，
		
		delayxms(1);
		memset(disbuf_1,0xff,RECORDLEN);
		flashread(addr,disbuf_1,RECORDLEN);
		MSG_LOG("读出来==数据\r\n");
		BCD_LOG(disbuf_1,RECORDLEN,1);
		if (memcmp(disbuf,disbuf_1,RECORDLEN)!=0)
		{
			MSG_LOG("写失败\r\n");
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
		//如果记录缓冲中没有记录可发将发送E2PROM中没有发成功的记录
		if (curp > headp)
		{
			sum = (curp - headp) / RECORDLEN;
#ifdef _debug_
			debugstring("有记录!\r\n");
#endif		
						flashread(headp, tmpChs, 4);
			MSG_LOG("前面4个字节格式:");
			BCD_LOG(tmpChs, 4, 1);
			MSG_LOG("指针状态:");
			if (memcmp(tmpChs, "\x55\xAA", 2) != 0) {
				sum=0;
				
				save_ODA_infor(FeRC_Dlelt, NULL);
		}
		}
		else {
			sum = 0;
			if (curp >= (FLASH1_START_O + FLASH_RCLR_O)) {//0x3E800如果记录地址已经达到了40000条（64字节），则重置存贮区，重新开始
				//clr_Record_point();
				lib_clr_Record_point_O();//原来用的是 clr_Record_point();   这个函数会把备份区的地址也清掉。。
				
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
	case ODA_FeRC_Init://初始化
		MSG_LOG("初始化ODA目录结构\n ");
	lib_clr_Record_point_O();

		break;
	case ODA_FeRC_Write://写记录
		MSG_LOG("写FLAHS ODA8583记录:");
		re_inforLen = GET_INT16(re_infor + 2) + 4;
		MSG_LOG("ODA记录:%d\n", re_inforLen);
		if (re_inforLen > 512) {
			MSG_LOG("ODA记录无法写入超入512B的记录:%d\n", re_inforLen);
			retCode = Ret_Err_Overflow;
			break;
		}
		libWriteRecord_O(re_infor);
		break;
	case ODA_FeRC_Dlelt://删掉
		MSG_LOG("删ODA8583记录\n ");
	
		if (FeRC_Dlelt_O()==1) {
		
			retCode = Ret_Err_Format;
			break;
		}

		break;
	case ODA_FeRC_Check://查看是否有记录
		//MSG_LOG("C");

		
		if (GJRec_Send_O()!=0) {
					MSG_LOG("有记录\r\n");
			retCode = Ret_YES;
		}
		else
		{
			//MSG_LOG("查看是否有记录\r\n");
			retCode = Ret_NO;
			break;
		}
		break;
	case ODA_FeRC_READ://

		MSG_LOG("读记录\r\n");
		RECORDLEN = getRECORD_LEEN_O();
//		curp =
	Get_Record_point_O((unsigned char*)&headp, 0);
		flashread(headp, re_infor, RECORDLEN);

		re_inforLen = GET_INT16(re_infor + 2) + 4;
		if (re_inforLen > 512) {
			MSG_LOG("ODA记录读出超512 B的记录:%d\n", re_inforLen);
			retCode = Ret_Err_Overflow;
			break;
		}
		break;
	default:
			MSG_LOG("其它\r\n");
		break;
	}

	return retCode;
}
//未上传
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
			sprintf(buffer, "ODA未传记录:%d/1", ltemp);
	}
	else
	{
		if (gchn_eng == 'E')
			sprintf(buffer, "ODA card:%d/0", ltemp);
		else
			sprintf(buffer, "ODA未传记录:%d/0", ltemp);
	}
#else
	if (gchn_eng == 'E')
		sprintf(buffer, "IC card:%d/0", ltemp);
	else
		sprintf(buffer, "IC卡共:%d/0", ltemp);
#endif	
	display(0, 0, buffer, 0);

	// #ifdef qPBOC_BUS
	// 	curp = qPbocGet_Record_point((unsigned char*)&headp, 0);
	// 	ltemp=0;
	// 	ltemp=(curp - headp)/256;
	// 	sprintf(buffer,"银联卡:%d",ltemp);
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
	init_Q_pos_infor();//清
	init_timeout_infor(); //清冲正延时上送
	init_delay_send();//延时找任务
}
void add_init_sum_frist(void)
{
	lib_clr_Record_point_O();//oda
//	lib_clr_Record_point_add();//冲正，初装机
	//		lib_clr_Record_point_2(FLASH1_START_2,FLASH_size); //冲正
}