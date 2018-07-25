
/**************************************************
* 文件名 :CPUCARJTB.c
* 描述   :交通部CPU卡消费文件
* 建立   :15/11/2015 @zzc	
**************************************************/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

#include "inputmisc/PsamCard.h"

#include "inputmisc/GPIOCtrl.h"


#include "LightColor.h"
#include "GPRS.h"

#include "DemoMain.h"

#include "inputmisc/IcCardCtrlApi.h"

#include "CPUCARJTB.h"
#include "Main_City.h"
#include "SlzrTypeDef.h"
#include "PSAMLib.h"
#include "MYDES.h"

#include "ICCardLib.h"
#include "CPUCard.h"
#include "RecordFile.h"

#define _debug_JTB_


//888888888888888888888888
 stJTB1A	JTB_1A;//1A文件结构
 stJTB15	jtb15file;//15
 stJTB16	jtb16file;//16
 stJTB17	jtb17file;//17
 stJTB18	jtb18file;//18
 stJTB1E	JTB_1E;//1A文件结构
pboc_sam_init_purchaseJTB stuSamInitPurchaseJTB;
 
unsigned char dealtype;//扣费模式
unsigned char BUS_Flag;//车的状态 一共8位前三位分别表示  1是否发车2是否在车站3是否刷了发车卡
FeUnKnowREC FeuRec;	//


unsigned short WhiteLength;		////下载白名单的时记录条数用


//白名单 机构代码添加 取0x15文件下的发卡机构标识（01-08 HEX）的前8位与白名单比较
#define white_organization 5
const unsigned int WriteNUM[white_organization+1] = {
	0x01581270,0x09991001,0x01656900,0x01423050,0x00010001,0x00
};
#define Factory_NUM		"\x09\x99\x10\x01"
sam_pub SamPubInf_LL;

unsigned char CPUPsamInitJTB(void)
{
	unsigned char i;
	unsigned char ii=0;
	unsigned char buffer[256];
	
	psamJTB.Flag = 1;
	
	memset(psamJTB.CardNO, 0, 6);
#ifdef _debug_JTB_
	debugstring("---JTBPSAM init\r\n");
	ii = PSAM_YD;
	MSG_LOG("PSAMJTB:%d",ii);
#endif
	for(i=0;i<2;i++)
	{
		ii = SamColdReset(PSAM_YD, 9600, 3, buffer);//
		if(ii!=0)
			break;
	}
	if(i>=2){

		ii = SamColdReset(PSAM_YD, 38400, 3, buffer);//
		if(ii==0)
			return ST_ERROR;
	}	
#ifdef _debug_JTB_
	debugstring("CPUPSAM art:");
	debugdata(buffer, ii, 1);
#endif
	usleep(5000);
	if(Read_Binary(PSAM_YD, 0x16, 0, 6, buffer)==0) 
	{
#ifdef _debug_JTB_
		debugstring("CPURead_Binary error\r\n");
		debugdata(buffer, 8, 1);
#endif
		return ST_ERROR;
	}
#ifdef _debug_JTB_
	debugstring("CPUread 16 Binary:");
	debugdata(buffer,8,1);
#endif
	memcpy(psamJTB.CardNO,buffer,6);
	usleep(100000);
	
#ifdef _debug_JTB_
	debugstring("CPUSelect File \r\n");
#endif

	//当前 AID=ASCII【MOT.CPTSAM01】
	//A0 00 00 06 32 4D 4F 54 2E 43 50 53 41 4D 30 31

	memcpy(buffer, "\xA0\x00\x00\x06\x32\x4D\x4F\x54\x2E\x43\x50\x53\x41\x4D\x30\x31", 16);		//JTB电子钱包AID
#ifdef _debug_JTB_
	debugstring("***select AID1:");
	debugstring("SND_C:");
	debugdata(buffer, 16, 1);
#endif
	if(Select_FileF(PSAM_YD, buffer, 7)==0)
	{
#ifdef _debug_JTB_
		debugstring("CPU PSAM Select_File1 error\r\n");
#endif
		return ST_ERROR;
	}

	if(Read_Binary(PSAM_YD, 0x17, 0, 25, buffer)==0)		//读0x17文件25字节
	{
		debugstring("GJ_Read_Binary17 error\r\n");
		debugdata(buffer, 8, 1);
		return ST_ERROR;
	}
#ifdef _debug_JTB_
	debugstring("JTB _read 17 Binary:");
	debugdata(buffer,25,1);	//01000000000000000100000000000000012011122220161220
	debugstring("OK\r\n");
#endif
	memcpy((unsigned char*)&SamPubInf_LL, buffer, sizeof(sam_pub));	//第一个字节是密钥索引
	 
	psamJTB.Flag = 0;
	return ST_OK;
}
extern unsigned int ghisCur;//历史当前指针
extern int getCardtype(char *cardD, unsigned char type);
extern int getCartypeJTB(char *cardD, unsigned char type);
unsigned char Disp_historyJTB(unsigned char *data,unsigned int Index)
{
	unsigned char buffer[80];
	char btff[20];
	unsigned int temp;//,temp1;
	DEALRECODE_JTB historyCheck;

	 char  tembuf[22];
//	unsigned int addr;
//	int curp;
	memcpy((unsigned char*)&historyCheck,data,RECORD_JTB_LEN);
	if (memcmp(historyCheck.rTTTT,"\xAA\xAA\xAA\xAA",4)!=0)//加个判断，不知道为啥 有时候会乱
	{
		return ST_ERROR;
	}
	switch((historyCheck.rDealType))
	{
	case ID_REC_HJY_REC:
	case ID_REC_TOLL:
		cls();
		memset(buffer,0,20);
		temp=0;
		memcpy((unsigned char*)&temp,historyCheck.rDealMoney,3);
		sprintf((char*)buffer,"扣款:%d.%02d ",temp/100, temp%100);
		memset(btff, 0, 20);//加入卡类
		getCartypeJTB(btff, historyCheck.rCardType);
		strcat((char*)buffer, btff);
		display(2,0,(char*)buffer,0);
		memset(buffer,0,20);
		temp=0;
		memcpy((unsigned char*)&temp, historyCheck.rAfterMoney, 4);
			sprintf((char*)buffer, "余额:%d.%02d", temp/100, temp%100);
		display(4,0,(char*)buffer,0);
		
		dis_time_mini(6, (stcomtime*)&historyCheck.rDealTime);
		break;
	case ID_REC_MON:
		cls();
		memset(buffer,0,20);

		temp = 0;
		memcpy((unsigned char*)&temp,historyCheck.rDealMoney,3);
		if(temp == 0){
			getCartypeJTB((char*)buffer, historyCheck.rCardType);
			display(3,0,(char*)buffer, DIS_CENTER);
		}
		else{
			temp=0;
			memcpy((unsigned char*)&temp,historyCheck.rDealMoney,3);
				sprintf((char*)buffer, "扣次:%d ",temp);
			memset(btff, 0, 20);//加入卡类
			getCartypeJTB(btff, historyCheck.rCardType);
			strcat((char*)buffer, btff);
			display(2,0,(char*)buffer,0);

			memset(buffer,0,20);
			memcpy((unsigned char*)&temp, historyCheck.rAfterMoney, 4);
			sprintf((char*)buffer, "余次:%d", temp);
			display(4,0,(char*)buffer,0);
		}
		break;

	case ID_REC_CLERK:
		cls();
		display(3, 0, "站务员卡", DIS_CENTER);
		break;

	case ID_REC_ARVBUS:
		cls();
		display(3, 0, "到站记录", DIS_CENTER);
		break;
	case ID_REC_LINESET:
			  cls();
		display(3, 0, "线路卡", DIS_CENTER);
		break;
	case ID_REC_BLKLIST	:
		cls();
		display(3, 0, "黑卡记录", DIS_CENTER);
		break;
	default:
		return ST_ERROR;
	}
	if(historyCheck.rBusFlag&BUS_TES&&historyCheck.rDealType!=ID_REC_HJY_REC)//测试数据
		sprintf((char*)buffer, "TES_REC    ->%d ", Index+1);
	else if(historyCheck.rDealType==ID_REC_HJY_REC)
		sprintf((char*)buffer, "UN_Know    ->%d ", Index+1);
	else
		sprintf((char*)buffer, "->%d ", Index+1);
	miniDispstr(7, (21-strlen((char*)buffer)), (char*)buffer, 0);
	memset(buffer,0,20);
	temp=0;
	//memcpy((unsigned char*)&tembuf, historyCheck.rPublishNumb,8);
	memset(tembuf,0,22);
	BCD2Ascii(historyCheck.rPublishNumb,(unsigned char*)tembuf,10);
	//over_turn(8,(unsigned char*)&tembuf);	
//	sprintf((char*)buffer,"%16x",tembuf);
	strcpy((char*)&buffer,tembuf);
	miniDispstr(0,1,(char*)buffer,0);
	dis_time_mini(6, (stcomtime*)historyCheck.rDealTime);
	return ST_OK;

}

void ERR_DIS(const char *dis,unsigned char *dat)//显示错误
{
	unsigned short distem;
	char disbuf[20];
	MessageBox(1,dis);
	gBuInfo.g24GDisFlash=3;
	gBuInfo.restore_flag=0;
	memcpy((unsigned char*)&distem,dat,2);
	over_turn(2,(unsigned char*)&distem);
	sprintf(disbuf,"%04x",distem);
	miniDispstr(6,0,disbuf,0);
}
//HEX转BCD  Dec 需要转换的数据  length 需要转出的字节数 mode==1,大端数据转换
int DectoBCD(unsigned int Dec, unsigned char *Bcd, int length,unsigned char mode)  //
{  //
	    int i;  
	     int temp;  
		 if(mode==1)
			over_turn(4,(unsigned char*)&Dec);

		    for(i=length-1; i>=0; i--)  
		     {  
				temp = Dec%100;  
				Bcd[i] = ((temp/10)<<4) + ((temp%10) & 0x0F);  
				Dec /= 100;  
			}  
			
			return 0;  
}  

#ifdef BUS_PRO_JTB
extern unsigned char BUS_Flag;//车的状态
void BuildRecordeJTB(unsigned char delType, unsigned char *recBuf)
{
	DEALRECODE_JTB *rRecordDeal;
	unsigned int temp;
	//		unsigned char ttt;
	rRecordDeal = (DEALRECODE_JTB *)recBuf;
	memset(rRecordDeal->rPublishNumb,0,128);
	memcpy(rRecordDeal->rPublishNumb, gCardinfo.PublishBicker, 10);//卡号
	rRecordDeal->rCardType = jtb17file.card_catalog;//gCardinfo.card_catalog;				//卡类
	rRecordDeal->rCardCand = gCardinfo.gMCardCand;							//卡类型
	memcpy(rRecordDeal->rSAMxulie,stuDebitPurchase.cTermSnq,4);		//PSAM交易流水
	memcpy(rRecordDeal->rPSAMNO,psamJTB.CardNO,6);						//终端编码PSAM卡号
#ifdef _NEW_Device
	ASCII2ZBcod((char*)gDeviceParaTab.DeviceNo,rRecordDeal->rDeviceNo, 4);	//机器的设备号 车号
#else
	Ascii2BCD(gDeviceParaTab.DeviceNo,rRecordDeal->rDeviceNo,8);
#endif
	
	rRecordDeal->rDealType = delType;								//交易类型
	memcpy(rRecordDeal->rPiaoJia,gDeviceParaTab.busPrice,2);			//票价
	memcpy(rRecordDeal->rMoneyDealSum,stuInitPurchaseRet.cSnq,2);	//钱包交易次数
	memcpy(rRecordDeal->rDealMoney, (unsigned char *)&s_sum1, 3);	//交易金额
	memcpy(rRecordDeal->rAfterMoney, (unsigned char*)&a_sum1, 4);	//交易余额
	memcpy(rRecordDeal->rDealTime,(unsigned char*)&SysTime,7);		//交易时间
	memcpy(rRecordDeal->rCPUTAC,stuDebitPurchase.cMac,4);			//TAC
	memcpy(rRecordDeal->rDriverNo, (unsigned char*)&gBuInfo.DriverNO, 4);//司机卡号

	memcpy(rRecordDeal->rLineNo, gDeviceParaTab.LineNo, 3);				//线路号

	memcpy(rRecordDeal->rFactorFlg,jtb15file.cPublisherSign,8);		//机构代码15
	memcpy(rRecordDeal->rCityCode,jtb17file.Citycode,2);			//城市代码15
	if(delType==ID_REC_HJY_REC)
		rRecordDeal->rHUIJIAOYI=1;										//灰交易标识15
	else
		rRecordDeal->rHUIJIAOYI=0;
	
	//
#ifdef _debug_JTB_
	BUS_Flag|=BUS_TES;//测试记录
#else
	BUS_Flag&=BUS_CL_TES;
#endif
	rRecordDeal->rBusFlag=BUS_Flag;
	//保留14
	temp = SOFT_VER_TIME_LOG;
	memcpy(rRecordDeal->rProVer, (unsigned char*)&temp, 2);			//程序版本
	memcpy(rRecordDeal->rTTTT,"\xAA\xAA\xAA\xAA",4);				//正常数据 必须加四个AA
	
#ifdef _debug_JTB_
	debugstring("组织记录\r\n");
	debugdata(rRecordDeal->rPublishNumb,29,1);
	debugdata(rRecordDeal->rMoneyDealSum,40,1);
	debugdata(rRecordDeal->rDealTimeOLD,39,1);
	
#endif
	if((delType == ID_REC_TOLL)||(delType == ID_REC_MON)){
		addStatMoney(delType, s_sum1);
	
	}
	//	memcpy(recBuf, (unsigned char*)&rRecordDeal, 128);
#ifdef _debug_JTB_
	debugstring("BLD REC:");
	debugdata((unsigned char*)&rRecordDeal,128,1);
#endif
	
}
// unsigned char WriRecorJTB(unsigned char *rec)
// {
// 	unsigned char ret;
// 
// 	//	WriRecorJTB(buffer);
// #ifdef _debug_JTB_
// 	debugstring("write REC:");
// 	debugdata(rec, RECORD_JTB_LEN, 1);
// #endif
// 	ret = libWriteRecord(BIT_COMSUME_FILE, rec, 0);
// 	return ret;
// }
// 邯郸兼容64字节长度写128，,mode=0;正常128，  mode=1,补够128
unsigned char WriRecorJTB(unsigned char *rec,unsigned char mode)
{
	unsigned char i=0;
	unsigned char buff[RECORD_JTB_LEN+1];
	unsigned char buffer[RECORD_JTB_LEN+1];
	//	unsigned char *tempbuf;
	unsigned int temp;
	//	unsigned char len;
	
	memset(buff, 0x00, RECORD_JTB_LEN);
	memcpy(buffer, rec, RECORD_JTB_LEN);
	if((mode==1)&&(memcmp(rec+124,"\xAA\xAA\xAA\xAA",4)!=0))//补够128的
	{
		MSG_LOG("补长度");
		//memcpy(buffer,rec,64);
		memset(buffer+64,0,60);
		memset(buffer+64+60,0xBB,4);
	}
	

	temp = Get_Record_point(buff, 0);
	

#ifdef _debug_JTB_
	debugstring("-------------------\r\ncur:");
	debugdata((unsigned char*)&temp, 4, 1);
	FR_flashread(temp, buff, RECORD_JTB_LEN);
	debugdata(buff, RECORD_JTB_LEN, 1);
#endif
	
	for(i=0; i<3; i++) 
	{
		
		FR_flashwrite(temp, buffer, RECORD_JTB_LEN);
#ifdef _debug_JTB_
		debugstring("WriteRecord : \r\n");		
		debugdata(buffer,RECORD_JTB_LEN,1);
#endif
		
		FR_flashread(temp, buff, RECORD_JTB_LEN);
#ifdef _debug_JTB_
		debugstring("WriteRecordCheck ulofset2: \r\n");
		debugdata(buff, RECORD_JTB_LEN, 1);
#endif

		if(memcmp(buffer, buff, RECORD_JTB_LEN) == 0) 
		{
			writeBackRec(buffer,RECORD_JTB_LEN, 0);
			ADD_Record_point(0, RECORD_JTB_LEN);
			MSG_LOG("WRITRC_OK!");
			return ST_OK;
		}
		else{//写错，正区写全0，副区写实数据，并写到下一数据地址
#ifdef _debug_JTB_
			debugstring("write record ERROR!!!\r\n");
#endif
			memset(buff, 0x01, RECORD_JTB_LEN);
			FR_flashwrite(temp, buff, RECORD_JTB_LEN);
			temp += RECORD_JTB_LEN;
			ADD_Record_point(0, RECORD_JTB_LEN);
			
		}
	}
	cls();
	
	writeBackRec(buffer, RECORD_JTB_LEN, 0);
	//	writeBackRec(buffer+128, RECORD_LEN, 0);
	display(2, 2, "E2PROM写出错,需要维修!", 1);
	memset(buffer, 0, 20);
	memcpy(buffer, "ADDR:", 5);
	BCD2Ascii((unsigned char *)&temp, buffer+5, 4);
	display(6,0,(char*)buffer, 0);
	
	led_on(LED_RED|LED_GREEN);
	for(;;)
		usleep(50000);
		
	return ST_OK;
}

void Build_1E_Rec(unsigned char *Rvb,unsigned char mode){
//	unsigned char tembuff[20];
	unsigned int itm;
	stJTB1E JTB_1ER;//
	memset((unsigned char*)&JTB_1ER,0,48);

	JTB_1ER.eDealType=mode;		//下车车
	memcpy(JTB_1ER.eDeviceNum+2,psamJTB.CardNO,6);
	JTB_1ER.eTracode=02;		//行业代码
	memcpy(JTB_1ER.eLineNUm,gDeviceParaTab.LineNo,2);
	memcpy((unsigned char*)&itm,(unsigned char*)&s_sum1,4);
	over_turn(4,(unsigned char*)&itm);
	memcpy(JTB_1ER.eDmoney,(unsigned char*)&itm,4);//交易金额
	memcpy((unsigned char*)&itm,(unsigned char*)&a_sum1,4);
	over_turn(4,(unsigned char*)&itm);
	memcpy(JTB_1ER.eYuE,(unsigned char*)&itm,4);//余额
	memcpy(JTB_1ER.eDealTime,(unsigned char*)&SysTime,7);
	memcpy(JTB_1ER.eCityco,jtb17file.Citycode,2);
	memcpy(JTB_1ER.eFactoryco,jtb15file.cPublisherSign,8);

	
	itm=5;
	over_turn(4,(unsigned char*)&itm);
	memcpy(JTB_1ER.eErrPriceMax,(unsigned char*)&itm,4);				//最大消费金额HEX(大端)
	//最大消费金额
	memcpy(Rvb,(unsigned char*)&JTB_1ER,48);
#ifdef _debug_JTB_
	debugstring("Build_1E\r\n");
	debugdata(Rvb,48,1);
#endif
}


//extern void Mac(unsigned char *Key,unsigned char *pInData1,unsigned char *pInData2,int data2len,unsigned char *initData, unsigned char *mac);
unsigned char  operat_Black(void)
{
	
	unsigned char i;
	unsigned char ret;
	unsigned char revbuf[256];
	unsigned char sndbuf[56];
	unsigned char Rader[10];
#ifdef NO_CPU_PSAM_
	unsigned char CPUPKEY[16];
	unsigned char snddat[64];
#endif

//	unsigned char CPUinitdat[8];

	i=13;
	memcpy(sndbuf, "\x00\xA4\x04\x00\x08\xA0\x00\x00\x06\x32\x01\x01\x05", i);		//JTB电子钱包AID
	//memcpy(sndbuf, "\x02\x00\xA4\x00\x00\x02\x3F\x01",8);//有此卡需要用文件名，所以取不到时需要处理一下。
//	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf, 13);
	if(ret == 0)
		return ST_ERROR;

	memcpy(sndbuf, "\02\x00\x84\x00\x00\x08", 6); i = 6;
	deal2or3(sndbuf);	
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_JTB_
	debugstring("随机数:");
	debugdata(revbuf, ret, 1);
#endif
	if(ret == 0)
		return 4;
	else{
		if(memcmp(revbuf+1, "\x94\x01", 2)==0)
			return 3;
		else if(memcmp(revbuf+ret-2, "\x90\x00", 2)!=0)
			return 5;
	}
	memset(Rader,0,10);
	memcpy(Rader,revbuf+1,4);
#ifdef NO_CPU_PSAM_
	if(getMiKey() != ST_OK)	//验证卡的认证码
		return 8;
	//----------计算KEY----------
	memcpy(CPUPKEY, gCardinfo.MoneyKEYA, 6);
	memcpy(CPUPKEY+6, gCardinfo.RecodeKEYA, 6);
	memcpy(CPUPKEY+12, gCardinfo.MonthKEYA, 4);

	memcpy(snddat,"\x84\x1E\x00\x00\x04",5);
#ifdef _debug_JTB_
	debugstring("sndat1:\r\n");
	debugdata(sndbuf,20,1);
	debugstring("sndat2:\r\n");
	debugdata(snddat,16,1);
	debugstring("key:\r\n");
	debugdata(CPUPKEY,16,1);
	debugstring("CPUinitdat:\r\n");
//	debugdata(CPUinitdat,8,1);
#endif
	memset(revbuf,0,20);
//	Mac(CPUPKEY, Rader, snddat, 5, CPUinitdat, revbuf);//1BE5F163F50843699E120D74FAF08E76 消费
	//CountMac(CPUPKEY, Rader, snddat, 5,revbuf);
#else//$ NO_CPU_PSAM_  修水用

//	memcpy(sndbuf,stuPubInfZaoZuang.cPublisherSign,8);
	memcpy(sndbuf,gCardinfo.PublishBicker+2,8);//卡号后8字节
	i =8;
	memcpy(sndbuf+i,gCardinfo.FactorFlg,8);//发卡机构代码
	i+=8;
	//memcpy(sndbuf+i,gCardinfo.CityCode,2);i+=2;
	//memcpy(sndbuf+i,"\xFF\x00\x00\x00\x00\x00",6);i+=6;
		
#ifdef _debug_JTB_
	debugstring("PSAM_MAC初始化指令:");
	debugdata((unsigned char *)&i,4,1);
	debugdata(sndbuf,i,1);
#endif
	if (CpuCardSetMac2(PSAM_GJ,revbuf,sndbuf,i) == ST_ERROR){	//801A450110
		return ST_ERROR;
	}

	memcpy(sndbuf,Rader,8);i=8;//密钥
	memcpy(sndbuf+i,"\x84\x1E\x00\x00\x04\x80\x00\x00",12);i +=8;
	
	if(CpuBlackMac(PSAM_GJ,revbuf,sndbuf,i)==ST_ERROR)			//80FA050010
		return ST_ERROR;
#endif
#ifdef _debug_JTB_
	debugstring("MAC1:");
	debugdata(revbuf,10,1);
#endif	
	//if(CpuBlackMac(gPSAMin,revbuf,sndbuf,i)==ST_ERROR)
	//	return ST_ERROR;
	//CountMAC();

	memcpy(sndbuf, "\02\x84\x1e\x00\x00\x04", 6); i = 6;
	memcpy(sndbuf+i,revbuf,4);i+=4;//MAC
	deal2or3(sndbuf);
	
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_JTB_
	debugstring("锁定:");
	debugdata(&ret,1,1);
	debugdata(revbuf, ret, 1);
#endif
	if(ret == 0)
		return 4;
	else{
		if(memcmp(revbuf+1, "\x94\x01", 2)==0)
			return 3;
		else if(memcmp(revbuf+ret-2, "\x90\x00", 2)!=0)
			return 5;
	}
//	memcpy(sndbuf, "\x02\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01", 15); // 选择3F01
	memcpy(sndbuf, "\x00\xA4\x04\x00\x08\xA0\x00\x00\x06\x32\x01\x01\x05", i);		//JTB电子钱包AID
//	deal2or3(sndbuf);
#ifdef _debug_JTB_
	debugstring("select AID1:");
	debugdata(sndbuf, 15, 1);
#endif
	ret = RfPassthrough(revbuf, 2, sndbuf, 13);
#ifdef _debug_JTB_
	debugdata(revbuf, ret, 1);
#endif
	if(ret == 0)
		return ST_ERROR;
	BuildRecorde(ID_REC_BLKLIST,revbuf);
	WriRecorJTB(revbuf,0);
//	WriteRecord(revbuf,0);
	return ST_OK;
}
//mode==1  写进铁电，mode==2删掉铁电记录  3 检查是否有记录需要转正      解决消费未完成  断电没记录的情况
void Set_UnknowREC(unsigned char mode,unsigned char *data){

	return;
// 	//FeUnKnowREC buffer;
// 	unsigned int CRC;
// 	unsigned char i;
// 
// 	if(mode==FeRC_Write){
// 		memcpy(FeuRec.falg,"有效",4);
// 		memcpy(FeuRec.RCE,data,RECORD_JTB_LEN);
// 		CRC=__cpuCrc32(FeuRec.falg,sizeof(FeUnKnowREC)-4);
// 		memcpy(FeuRec.CRC,(unsigned char*)&CRC,4);
// 		sysfewrite(BIT_UNKNOW_RECORD,sizeof(FeUnKnowREC),(unsigned char*)&FeuRec);
// 		MSG_LOG("写铁电记录\r\n ");
// 		BCD_LOG(FeuRec.falg,sizeof(FeUnKnowREC),1);
// 	}
// 	else if (mode==FeRC_Dlelt)//删掉
// 	{
// 		
// 		memset((unsigned char*)&FeuRec,0,sizeof(FeUnKnowREC));
// 		CRC=__cpuCrc32(FeuRec.falg,sizeof(FeUnKnowREC)-4);
// 		memcpy(FeuRec.CRC,(unsigned char*)&CRC,4);
// 		sysfewrite(BIT_UNKNOW_RECORD,sizeof(FeUnKnowREC),(unsigned char*)&FeuRec);
// 		MSG_LOG("删掉铁电记录\r\n ");
// 		BCD_LOG(FeuRec.falg,sizeof(FeUnKnowREC),1);
// 	}else if (mode==FeRC_Check)//判断是否有记录需要转正
// 	{
// 		
// 		for(i=0;i<5;i++)
// 		{
// 			sysferead(BIT_UNKNOW_RECORD,sizeof(FeUnKnowREC),(unsigned char*)&FeuRec);
// 			CRC=__cpuCrc32(FeuRec.falg,sizeof(FeUnKnowREC)-4);
// 			if(memcmp((unsigned char*)&CRC,FeuRec.CRC,4)==0)//正确
// 				break;
// 		}
// 
// 		MSG_LOG("FeuREC:\r\n");
// 		BCD_LOG((unsigned char*)&i,1,1);
// 		BCD_LOG(FeuRec.falg,136,1);
// 
// 		if(memcmp(FeuRec.falg,"有效",4)==0){//有记录
// 			WriRecorJTB(FeuRec.RCE,0);		//写进记录
// 		//转正后清掉
// 		memset((unsigned char*)&FeuRec,0,sizeof(FeUnKnowREC));
// 		CRC=__cpuCrc32(FeuRec.falg,sizeof(FeUnKnowREC)-4);
// 		memcpy(FeuRec.CRC,(unsigned char*)&CRC,4);
// 		sysfewrite(BIT_UNKNOW_RECORD,sizeof(FeUnKnowREC),(unsigned char*)&FeuRec);
// 		MSG_LOG("记录转正\r\n");
// 		return;
// 		}
// 		MSG_LOG("没有记录\r\n");
// 	}
		

}




//冲刷取TAC借口函数 4 MAC1+4 tac
unsigned char GET_TRANSACTION_PROVE(unsigned char*outdata,unsigned char mode)
{
	unsigned char i;
	unsigned char sndbuf[100];
	unsigned int ret;
	unsigned char revbuf[128];
	unsigned int kk=0;
	unsigned short ii;
	
	memset(outdata, 0, 8);//返回的TAC码
	
	i=5;
	memcpy(sndbuf,"\x00\xB2\x01\xC4\x00",i);//18文件
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_
	debugstring("RCV_C18:");
	debugdata(revbuf, ret, 1);
#endif
	
	
	memcpy(jtb18file.rEPsearl,revbuf+1,sizeof(jtb18file));
	
	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0){
		MSG_LOG("18文件错\r\n");
#ifdef _debug_
		beep(3,100,100);
#endif
		memcpy((unsigned char*)&ii, stuInitPurchaseRet.cSnq, 2);
		over_turn(2, (unsigned char*)&ii);
		kk += 1;
		over_turn(2, (unsigned char*)&ii);
		memcpy(jtb18file.rEPsearl, (unsigned char*)&ii, 2);//8
		memcpy(jtb18file.rPSAMnum, psamJTB.CardNO,6);
		memcpy(jtb18file.rPursTime,stuSamInitPurchaseJTB.cDateTime,7);
		//	return ST_ERROR;
	}
	
	if(memcmp(jtb18file.rPSAMnum,psamJTB.CardNO,6)!=0)//不是同一个终端
	{
		MSG_LOG("终端部一致\r\n");
#ifndef _debug_
		return ST_ERROR;
#endif
	}
	if(memcmp((unsigned char*)&SysTime,jtb18file.rPursTime,7)>=0)
		kk=DataTimeDec((unsigned char*)&SysTime,jtb18file.rPursTime);
	else
	{
		MSG_LOG("时间出错\r\n");
#ifndef _debug_
		return ST_ERROR;
#endif
	}
	BCD_LOG((unsigned char*)&SysTime,7,1);
	BCD_LOG(jtb18file.rPursTime,7,1);
	MSG_LOG("获取 %d秒内\r\n",kk);
	if(kk<120)//两分钟内
	{
		if(mode==9)
			memcpy(sndbuf, "\x80\x5A\x00\x09\x02",5);
		else
			memcpy(sndbuf, "\x80\x5A\x00\x06\x02",5);//memcpy(sndbuf, "\x02\x80\x5A\x00\x05\x02", 6);
		i=5;
		memcpy(sndbuf+i, jtb18file.rEPsearl, 2);
		i+=2;
		ret = RfPassthrough(revbuf, 2, sndbuf, i);
		if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0){
			MSG_LOG("获取失败了+++++++++\r\n");
			beep(2,100,100);
			return ST_ERROR;
		}
		memcpy(outdata, revbuf+1, 8);//返回的TAC码
		MSG_LOG("TAC获取成功 \r\n");
		BCD_LOG(outdata,8,1);
		return ST_OK;
	}
	MSG_LOG("时间不对了\r\n");
	beep(2,100,100);
	return ST_ERROR;	
}


unsigned char CPUDealCard_JTB(unsigned char mode, unsigned char cool)
{

	unsigned char i;
	unsigned char sndbuf[250];
	unsigned char revbuf[200];
	unsigned char TACC[4]={0};
	//	unsigned char PSAMDsno[4];
	unsigned int ret;
	unsigned int value;
	unsigned int uitemp=0;
	unsigned char temrev[10];

	if(psamJTB.Flag){
		cls();
		display(0,0,"错误:",1);
		display(3,0,"交通PSAM错误!",0);

		gBuInfo.restore_flag = 3;
		sleep(1);
		return ST_ERROR;
	}
			
#ifdef _debug_JTB_
	debugstring("CPUDealCard====JTB====\r\n");
#endif
	gCardinfo.gMCardCand= CARDSTYLE_JTB;
	if(gBuInfo.stop_flag==1) 
		return ST_ERROR;

	if (CPU_Read_Money((unsigned char*)&value, 3) != 0){
		MessageBox(1,"读余额出错");
		beep(2,50,50);
		gBuInfo.g24GDisFlash=5;
		gBuInfo.restore_flag=0;
		return ST_ERROR;
	}
	over_turn(4, (unsigned char*)&value);
	a_sum1 = value;		//余额


	if(cardSound == 0xaa){
		if(card_ser!=gCardinfo.c_serial){
			Set_UnknowREC(FeRC_Check,revbuf);//重刷被别的卡覆盖了,要把上一个卡的记录保存下来
			card_ser = 0;
			cardSound = 0;
		}
	}
	if(cardSound == 0xaa){//需要重刷
		if(value < a_sumR){//上次已经扣成功
// 			memcpy(sndbuf, "\x02\x80\x5A\x00\x06\x02", 6);//memcpy(sndbuf, "\x02\x80\x5A\x00\x05\x02", 6);
// 				sndbuf[4]=stuSamInitPurchaseJTB.cType;
// 			memcpy((unsigned char*)&ii, stuInitPurchaseRet.cSnq, 2);
// 			over_turn(2, (unsigned char*)&ii);
// 			ii += 1;
// 			over_turn(2, (unsigned char*)&ii);
// 			memcpy(sndbuf+6, (unsigned char*)&ii, 2);
// 			sndbuf[9] = 8;
// #ifdef _debug_CPU_
// 			debugstring("防拔:");
// 			debugdata(stuInitPurchaseRet.cSnq, 2, 1);
// 			debugdata(sndbuf, 10, 1);
// #endif
// 			deal2or3(sndbuf);
// 			revLen = RfPassthrough(revbuf, 2, sndbuf, 10);
// 			if(revLen == 0)
// 				return ST_ERROR;
// #ifdef _debug_CPU_
// 			debugdata(revbuf, revLen, 1);//02 00 0C 00 00 ED F2 97 9B 6E 94 37 BC 90 00 F2 03
// #endif
// 
// 			if(revLen == 0)//返回超时,错误
// 				return ST_ERROR;
// 			if((memcmp(revbuf+revLen-2, "\x90\x00", 2)!=0) && (memcmp(revbuf+revLen-2, "\x6C\x08", 2)!=0))
// 				goto CPUMAC2deal;//取不成功02 69 85 
// 			memcpy(TACC, revbuf+revLen-6, 4);//返回的TAC码
// 			memcpy(sndbuf, revbuf+revLen-10, 4);//用MAC2码
			
			if(GET_TRANSACTION_PROVE(revbuf, 9) == ST_ERROR){
				if(gCardinfo.CardType == LocakCard)
					goto CPUMAC2deal;				//如果是本地记录还是要上传的。
				else
					goto CPUdealEnd;	//跳过写记录，注意TAC错误跳过写记录。
			}
			
			memcpy(sndbuf, revbuf, 4);//用MAC2码
			memcpy(TACC, revbuf+4, 4);//返回的TAC码
			
			//如果是其他月票或者M1卡进去了一次，这些变量可能已经变了，为了防止写记录有问题，重新取值
			//其实前端重刷控制好了，这么没有必要要了

			s_sum1 = get_s_sum1(0);	
			
			goto CPUMAC2deal0;
		}	

	}
	a_sumR = value;		//扣之前余额
	//8050 0102 0B||密钥标识符 [1]（0x01）||交易金额[4](0x00000001)||终端机编号[6]
	//8050 0302 0B  复合消费
	i=5;
	memcpy(sndbuf, "\x80\x50\x03\x02\x0B", i);//11 INITIALIZE FOR PURCHASE命令
	sndbuf[i++] = SamPubInf_LL.kid;	//密钥索
//	debugdata((unsigned char*)&SamPubInf_LL,25,1);
//

	s_sum1 = get_s_sum1(0);//取得消费金额

	
	MSG_LOG("消费金额:");
	BCD_LOG((unsigned char*)&s_sum1,4,1);

	if(s_sum1 > 2000)
		return 22;
	uitemp = s_sum1;
	over_turn(4, (unsigned char*)&uitemp);
	memcpy(sndbuf+i, (unsigned char*)&uitemp, 4); i+=4;
	memcpy(sndbuf+i, psamJTB.CardNO, 6); i += 6;
#ifdef _debug_JTB_
	debugstring("*** init for purchase:");
	debugstring("SND_C:");
	debugdata(sndbuf, i, 1);
#endif
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_JTB_
	debugstring("RCV_C:");
	debugdata(revbuf, ret, 1);
#endif
	if(ret == 0){
	//	ERR_DIS("初始化出错",revbuf);
		return ST_ERROR;
	}
	else{
		if(memcmp(revbuf+ret-2, "\x94\x01", 2)==0){
			if(cool == 4)//月票连刷进来，不提示余额不足
				return 4;
			else
				return 3;			//余额不足
		}
		else if(memcmp(revbuf+ret-2, "\x90\x00", 2)!=0){
			ERR_DIS("初始化出错失败",revbuf+ret-2);
			return 5;
		}
	}	
	memcpy((unsigned char*)&stuInitPurchaseRet, revbuf+1, sizeof(stuInitPurchaseRet));
#ifdef _debug_JTB_
	debugstring("8050指令返回:::\r\n");
	debugdata((unsigned char*)&stuInitPurchaseRet, sizeof(stuInitPurchaseRet), 1);
	debugstring("卡余额::");
	debugdata(stuInitPurchaseRet.cBalance,4,1);
	debugstring("密钥版本和算法标识:");
	debugdata(&stuInitPurchaseRet.cKeyVer,2,1);
#endif
		if(GetDateTime())
			return ST_ERROR;
	memcpy(stuSamInitPurchaseJTB.cDateTime,(unsigned char*)&SysTime,7);
	memcpy(stuSamInitPurchaseJTB.cRand,stuInitPurchaseRet.cRand,4);
	memcpy(stuSamInitPurchaseJTB.cSnq,stuInitPurchaseRet.cSnq,2);
	memcpy(stuSamInitPurchaseJTB.cMoney,(unsigned char*)&uitemp,4);
	stuSamInitPurchaseJTB.cType=0x09;//复合消费
	stuSamInitPurchaseJTB.cKeyVer=stuInitPurchaseRet.cKeyVer;
	stuSamInitPurchaseJTB.cKeyEn=stuInitPurchaseRet.cKeyEn;

	memcpy(stuSamInitPurchaseJTB.cPublisherSign,gCardinfo.PublishBicker+2,8);//卡号后8字节
	memcpy(stuSamInitPurchaseJTB.cKeyFactor,gCardinfo.FactorFlg,8);//发卡机构代码
	
	memcpy((unsigned char*)&a_sum1, stuInitPurchaseRet.cBalance, 4);//余额
	over_turn(4, (unsigned char*)&a_sum1);
	
#ifdef _debug_JTB_
	MSG_LOG("a_sum:%d.%d元\r\n",a_sum1/100,a_sum1%100);
#endif
	gCardinfo.gucSoudnorm=0;
	if((a_sum1>=s_sum1)&&(a_sum1<500)){
		gCardinfo.gucSoudnorm = 1;
	}

 #ifdef _debug_JTB_
	debugstring("JTB MAC1计算指令:");
	debugdata(gCardinfo.PublishBicker,10,1);
	debugdata(gCardinfo.FactorFlg,8,1);
	debugdata((unsigned char*)&stuSamInitPurchaseJTB,sizeof(stuSamInitPurchaseJTB), 1);//+12
 #endif	
	if(CpuCardSetMac(PSAM_YD,revbuf, (unsigned char*)&stuSamInitPurchaseJTB,0x24) == ST_ERROR)
	{
		CPUPsamInitJTB();
		return 6;
	}
	
#ifdef _debug_JTB_
	debugstring("MAC OK!:");
	debugstring("8070指令返回:::\r\n");
	debugdata(revbuf, 9, 1);
#endif	
		memcpy(temrev,revbuf,8);//终端序列号+MAC
		memcpy(stuDebitPurchase.cTermSnq,revbuf,4);//终端交易流水
		memcpy(stuDebitPurchase.cDateTime,stuSamInitPurchaseJTB.cDateTime,7);
#ifdef _debug_JTB_
		debugstring("stuDebitPurchase\r\n");
		debugdata(stuDebitPurchase.cTermSnq,15,1);
#endif


		//更新0X1E文件80DC 00 F0 
		memcpy(sndbuf,"\x80\xDC\x00\xF0\x30",5);
		//扣款
		i=5;
		Build_1E_Rec(sndbuf+i,dealtype);//组织1A文件
		i+=48;
		ret = RfPassthrough(revbuf, 2, sndbuf, i);
		
#ifdef _debug_JTB_
		debugstring("更新1E:");
		debugdata(sndbuf, i, 1);
		debugdata(revbuf, 10, 1);
#endif
		if(ret==0){
		//	ERR_DIS("更新1E失败",revbuf);
			return ST_ERROR;
		}
		if(memcmp(revbuf+ret-2, "\x90\x00", 2)!=0){
			ERR_DIS("更新1E文件失败",revbuf+ret-2);
			 return ST_ERROR;
		}
		BuildRecordeJTB(ID_REC_HJY_REC, revbuf);//先生成记录
		Set_UnknowREC(FeRC_Write,revbuf);//写进铁电
		//*****************************
		i=5;
		memcpy(sndbuf, "\x80\x54\x01\x00\x0F", i);//11 INITIALIZE FOR PURCHASE命令
		memcpy(sndbuf+i,temrev,4);//终端交易序列号
		i+=4;
		memcpy(sndbuf+i,stuSamInitPurchaseJTB.cDateTime,7);
		i+=7;
		memcpy(sndbuf+i,temrev+4,4);//MAC1
		i+=4;	
		ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_JTB_
		debugstring("扣费!:");
		debugdata(sndbuf, i, 1);
		debugdata(revbuf, ret, 1);
#endif
		if(ret==0){
			cardSound = 0xaa;//出错，需要重刷卡
			card_ser = gCardinfo.c_serial;
			audio(Audio_TRY_AGAIN);
			return 7;
		}
		if(memcmp(revbuf+ret-2, "\x93\x02", 2)==0)
			return 9;//非法卡
		if(memcmp(revbuf+ret-2, "\x90\x00", 2)!=0){
			return 7;
		}			 
		memcpy(sndbuf,revbuf+1+4,4);//MAC2
	//	memcpy(stuDebitPurchase.cMac,revbuf+1,4);//交易的MAC
		memcpy(TACC,revbuf+1,4);
		a_sum1-=s_sum1;//扣款要放在这里
CPUMAC2deal0:
#ifdef _debug_JTB_
	debugstring("校验MAC2::\r\n");
#endif

		if(CPUMAC2(PSAM_YD,revbuf, sndbuf, 4)==ST_ERROR)//校验MAC2
			ERR_DIS("MAC2校验错误",revbuf);

CPUMAC2deal:
		memcpy(gCardinfo.PbocTac, TACC, 4);	// TAC码
		BuildRecordeJTB(ID_REC_TOLL, revbuf);
		WriRecorJTB(revbuf,0);


// 	memcpy(jsb_recbuff.TAC,gCardinfo.PbocTac,4);
	
#ifdef _debug_JTB_
	debugstring("计算MAC2:");
	debugdata(revbuf, 10, 1);
#endif

CPUdealEnd:	
	card_ser = 0;
	cardSound = 0;
//	display(2,1,"消费成功",DIS_CENTER);
	money_msg(ID_REC_TOLL, a_sum1, s_sum1, 0);
//	Sound_cityCMD(0);
	audio(Audio_DONG);
	Set_UnknowREC(FeRC_Dlelt,revbuf);//删掉铁电的记录
//if(gCardinfo.card_catalog!=1)//普通卡
//	save_h_month();
//	Save_delayCardinfo(mode);

	return ST_OK;//消费成功
}
//查找白名单
unsigned char IsWriteNUM(unsigned int Write)
{
	unsigned char i;

//		MSG_LOG("查找白名单\r\n");
	for(i=0; i<white_organization; i++){
//		BCD_LOG((unsigned char*)&Write,4,1);
//		BCD_LOG((unsigned char*)&WriteNUM[i],4,1);
		if(Write == WriteNUM[i])
			return ST_OK;
		}
	return ST_ERROR;
}
unsigned char IF_15_OK(unsigned char *Data){

//	unsigned int ret=0;
	unsigned int uitemp=0;
	unsigned int NowTime;
	unsigned int start_time=0,end_time=0;
	
//	unsigned char i=0;
	//	unsigned int value;	
//	unsigned char sndbuf[256];
//	unsigned char revbuf[150];
//	unsigned int temp;
	unsigned char disbuf[2];
	memcpy(gCardinfo.PublishBicker, jtb15file.cApp_sn,10);
	memcpy((unsigned char*)&NowTime, (unsigned char *)&SysTime, 4);//当前日期
	over_turn(4, (unsigned char*)&NowTime);
	memcpy((unsigned char*)&start_time, jtb15file.cIssueDate, 4);//启用日期
	over_turn(4, (unsigned char*)&start_time);
	memcpy((unsigned char*)&end_time, jtb15file.cValidDate, 4);//有效日期
	over_turn(4, (unsigned char*)&end_time);
	memcpy(gCardinfo.FactorFlg,jtb15file.cPublisherSign,8);
	memset(disbuf,0xff,2);
	if(jtb15file.cStarFlag!=0x01){//0 未启用，1 启用 2 停用
		disbuf[0]=0;
		disbuf[1]=jtb15file.cStarFlag;
		ERR_DIS("启用标识不对",disbuf);
		return ST_ERROR;//未启用
	}
	uitemp=BCD2int((unsigned char*)&start_time,4);
	//判断启用时间
	if(memcmp((unsigned char*)&start_time,"\x00\x00\x00\x00",4)==0||memcmp((unsigned char*)&start_time,"\xFF\xFF\xFF\xFF",4)==0||memcmp((unsigned char*)&end_time,"\x00\x00\x00\x00",4)==0||memcmp((unsigned char*)&end_time,"\xFF\xFF\xFF\xFF",4)==0)
	{	memcpy(disbuf,(unsigned char*)&start_time,2);
		ERR_DIS("日期格式不对",disbuf);
		 return ST_ERROR;
	}
	//启用日期日期判断
	if(((uitemp%4==0)&&(uitemp%100!=0))||(uitemp%400==0))  
	{
		//闰年
		if(memcmp((unsigned char*)&start_time,"\x29",1)>0&&memcmp((unsigned char*)&start_time+1,"\x02",1)==0){
			//MessageBox(1,"启用日期格式不对");
			memcpy(disbuf,(unsigned char*)&start_time,2);
			MSG_LOG("闰年");
			ERR_DIS("启用日期格式不对",disbuf);
		//	return ST_ERROR;
		}
	}
	else{
		//平年
		if(memcmp((unsigned char*)&start_time,"\x28",1)>0&&memcmp((unsigned char*)&start_time+1,"\x02",1)==0){
			MSG_LOG("平年");
			MessageBox(1,"启用日期格式不对");
		//	return ST_ERROR;
		}
		
	}
	//结束日期判断
	uitemp=BCD2int((unsigned char*)&end_time,4);
	//有效日期
	if(((uitemp%4==0)&&(uitemp%100!=0))||(uitemp%400==0))  
	{
		//闰年
		if(memcmp((unsigned char*)&end_time,"\x29",1)>0&&memcmp((unsigned char*)&end_time+1,"\x02",1)==0){
			MessageBox(1,"有效日期格式不对");
		//	return ST_ERROR;
		}
	}
	else{
		//平年
		if(memcmp((unsigned char*)&end_time,"\x28",1)>0&&memcmp((unsigned char*)&end_time+1,"\x02",1)==0){
			
			MessageBox(1,"有效日期格式不对");
		//	return ST_ERROR;
		}
		
	}

	//判断是否过期
	if((start_time>NowTime)||(NowTime > end_time)){//启用时间判断 当天也要通过

		return CARD_NO_TIME;//过期或末到期
	}
//判断个人化启用标识
	if (jtb15file.cAppType!=02)//个人化应用标识错
	{
			disbuf[1]=jtb15file.cAppType;
			disbuf[0]=0x00;
			ERR_DIS("个人化标识错",disbuf);
		return ST_ERROR;//未启用
	}

	if(memcmp(gCardinfo.FactorFlg,"\x01\x58\x12\x70",4)==0)//本地卡
		gCardinfo.CardType=LocakCard;//本地卡 
	else
		gCardinfo.CardType=YiDiCard;//异地卡
#if 0
 	if(memcmp(gCardinfo.FactorFlg,"\x01\x58\x12\x70",4)!=0)//本地卡
		return NO_WriteNUM;//不在白名单内

  //		CardType=LocakCard;//本地卡 
//  	else
//  		CardType=YiDiCard;//异地卡

	//CardType=YiDiCard;//异地卡

	//判断卡的机构代码是否在白名单内
	memcpy((unsigned char*)&temp,gCardinfo.FactorFlg,4);//取发卡机构代码的前四个字节
	over_turn(4,(unsigned char*)&temp);
	if(IsWriteNUM(temp))
			return NO_WriteNUM;//不在白名单内

//	if(IsInWhileList(gCardinfo.FactorFlg)!=ST_ERROR)//有
//		return NO_WriteNUM;//不在白名单内

#endif


	return ST_OK;
}

unsigned char IF_1E_OK(unsigned char *Data)
{
//	unsigned short i;
	unsigned char disbuf[2];
//	unsigned int temp;

	//判断1E
	if(JTB_1E.eDealType>10){
		disbuf[0]=JTB_1E.eDealType;
		disbuf[1]=0;
		ERR_DIS("读1E交易类型错",disbuf);
		return ST_ERROR;
		}

	
		dealtype=ON_Nomal;//去消费
		return ST_OK;
}

unsigned int IsWhiteBlock(unsigned char *c_card_no )
{
/*	unsigned int i_start=0;
	unsigned int i_end;
	unsigned int i_cur, addr;
	unsigned char i;
	unsigned char temp[WHITE_SNO_LEN];
	unsigned int invalid_point;
	unsigned char buffer[WHITE_SNO_LEN];
#ifdef _debug_white
	char disbuff[64];
#endif
	
	memcpy(temp,c_card_no,WHITE_SNO_LEN);
	
	i_end = WhiteLength -1;
	
#ifdef _debug_white
	debugstring("White==  current Card NO.:");
	debugdata(temp,WHITE_SNO_LEN,1);
	debugstring("\r\n");
	debugdata((unsigned char *)&i_end,4,1);
#endif 
	
	while(1)
	{
		i_cur=i_start+(i_end-i_start)/WHITE_SNO_LEN;
		invalid_point=i_cur*WHITE_SNO_LEN;			//
	
		addr = FLASH_WHITE_START + 6 +invalid_point;//
		
		
#ifdef _debug_white
// 		debugstring("invalid_point:");
// 		debugdata((unsigned char *)&invalid_point, 4, 1);
		memset(disbuff,0,sizeof(disbuff));
		sprintf(disbuff,"start:%d\r\n",i_start);
		debugstring(disbuff);
		memset(disbuff,0,sizeof(disbuff));
		sprintf(disbuff,"i_cur:%d\r\n",i_cur);
		debugstring(disbuff);
		memset(disbuff,0,sizeof(disbuff));
		sprintf(disbuff,"i_end:%d\r\n",i_end);
		debugstring(disbuff);
#endif
		flashread(addr, buffer, WHITE_SNO_LEN);
#ifdef _debug_white
		debugstring("read:");
		debugdata(buffer,WHITE_SNO_LEN,1);
#endif
		for(i=0;i<WHITE_SNO_LEN;i++)
		{
			if(temp[i]==buffer[i]){
#ifdef _debug_white
				debugstring("Y");
#endif
				continue;
			}
			else if(temp[i]>buffer[i])

			{
				if(i_end==i_cur)
					return 0xffffffff;
				else
				{
					i_start=i_cur+1;
#ifdef _debug_white
					debugstring("往后---\r\n");
#endif

					break;
				}
			}
			else
			{
				if(i_start==i_cur)
					return 0xffffffff;
				else
				{
#ifdef _debug_white
					debugstring("往前---\r\n");
#endif

					i_end=i_cur;
					break;
				}
			}
		}
		if(i==WHITE_SNO_LEN){
			return addr;
		}
	}
	*/
	return (FLASH_WHITE_END+1);
}
//0 不在   1 在
unsigned int IsInWhileList(unsigned char *c_card_no)
{
//	unsigned char i;
	unsigned int addr;

#ifdef _debug_JTB_
	debugstring("city_code::");
	debugdata(c_card_no,2,1);
	debugstring("WhiteLength:");
	debugdata((unsigned char *)&WhiteLength,2,1);
#endif
	
	if (memcmp(c_card_no,Factory_NUM,WHITE_SNO_LEN) == 0)
	{
		return 1;
	}else{
		if (WhiteLength == 0)
			return 0;

		addr = IsWhiteBlock(c_card_no);
#ifdef _debug_white
		debugstring("addr:::");
		debugdata((unsigned char *)&addr,4,1);
#endif
		if(addr < FLASH_WHITE_END)
			return 1;
	}	
	
	return 0;
}
//00异地》01本地
unsigned char CPUcardType_JTB(unsigned char mode)
{
	unsigned int ret=0;
	unsigned int uitemp=0;
	unsigned int start_time=0,end_time=0;

	unsigned char i=0;
//	unsigned int value;	
	unsigned char sndbuf[256];
	unsigned char revbuf[150];

	MSG_LOG("Cpu_JTB----\r\n");
	//以下暂时取消  速度回快很多
// 	memcpy(sndbuf,"\xe0\x50",2);
// 	ret = RfPassthrough(revbuf, 2, sndbuf, 2);
// 	mode2or3 = 2;
// 	delayxms(2);

	gCardinfo.gMCardCand= CARDSTYLE_JTB;
#if 0  //2016年11月15日09:03:17 cai
	i=19;
	//00A404000E325041592E5359532E4444463031
    memcpy(sndbuf,"\x00\xA4\x04\x00\x0E\x32\x50\x41\x59\x2E\x53\x59\x53\x2E\x44\x44\x46\x30\x31",i);//2PAY.SYS.DDF01
#ifdef _debug_JTB_
	debugstring("***select PPSE:2PAY.SYS.DDF01\r\n");
	debugstring("SND_C:");
	debugdata(sndbuf, i, 1);
#endif
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_JTB_
	debugstring("RCV_C:");
	debugdata(revbuf, ret, 1);
#endif

#endif //#if 0
//	if(ret == 0)
//		return ST_ERROR;

	i=13;
	memcpy(sndbuf, "\x00\xA4\x04\x00\x08\xA0\x00\x00\x06\x32\x01\x01\x05", i);		//JTB电子钱包AID
#ifdef _debug_JTB_
	debugstring("***select AID1:");
	debugstring("SND_C:");
	debugdata(sndbuf, i, 1);
#endif
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_JTB_//选择应用返回的特殊数据源
	debugstring("RCV_C:");
	debugdata(revbuf, ret, 1);
#endif
	if(ret == 0){
		//MessageBox(1,"选择AID失败");
	//	beep(2,50,50);
		gBuInfo.g24GDisFlash=5;
		gBuInfo.restore_flag=0;
		return ST_ERROR;
	}
	
	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0)
	{
		MSG_LOG("找不到应用----\r\n");
		//ERR_DIS("选择AID失败",revbuf+ret-2);
		return ST_ERROR;
	}

	if(memcmp(revbuf+ret-2, "\x6A\x81", 2) == 0)
		return CARD_STOPWORK;
	if(memcmp(revbuf+ret-2, "\x93\x03", 2) == 0)
		return APP_LOCK;

	i=5;
	memcpy(sndbuf,"\x00\xB0\x95\x00\x1E",i);//公共应用信息文件 30字节
#ifdef _debug_JTB_
	debugstring("***READ 95:\r\n");
	debugstring("SND_C:");
	debugdata(sndbuf, i, 1);
#endif
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_JTB_
	debugstring("RCV_C_15:");
	debugdata(revbuf, ret, 1);
#endif

	if(ret == 0){
	//	ERR_DIS("读1A文件失败",revbuf);
		return ST_ERROR;
	}
	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0)
	{
		ERR_DIS("读15文件失败",revbuf+ret-2);
		return ST_ERROR;
	}
	//	jtb15file = (stJTB15 *)(revbuf+1);
	memcpy((unsigned char*)&jtb15file,revbuf+1,30);
	memcpy(gCardinfo.PublishBicker, jtb15file.cApp_sn,10);
	memcpy((unsigned char*)&uitemp, (unsigned char *)&SysTime, 4);//当前日期
	over_turn(4, (unsigned char*)&uitemp);
	memcpy((unsigned char*)&start_time, jtb15file.cIssueDate, 4);//启用日期
	over_turn(4, (unsigned char*)&start_time);
	memcpy((unsigned char*)&end_time, jtb15file.cValidDate, 4);//有效日期
	over_turn(4, (unsigned char*)&end_time);
	memcpy(gCardinfo.FactorFlg,jtb15file.cPublisherSign,8);
	
#ifdef _debug_JTB_
	debugstring("卡号:");
	debugdata(gCardinfo.PublishBicker,10,1);
	debugstring("发卡机构标识:");
	debugdata(gCardinfo.FactorFlg,8,1);
	debugstring("启用标识:");
	debugdata((unsigned char *)&jtb15file.cStarFlag,1,1);
	MSG_LOG("启用日期::%08X\r\n",start_time);
	MSG_LOG("当前日期::%08X\r\n",uitemp);
	MSG_LOG("有效日期::%08X\r\n",end_time);
#endif
	  ret=IF_15_OK(revbuf+1);
	  if(ret!=ST_OK)
		  return ret;
//

#if 0//互联互通 暂时没有黑名单
	if(IsInBlackList(stuPubInfZaoZuang.cPublisherSign)){
#ifdef _debug_JTB_
		debugstring("lock card:-----\r\n");
#endif
		return CARD_BLACK_PRO;
		
	}
#endif//


// #if 0  //不要读了 2016年9月10日07:00:21
// 	i=5;
// 	memcpy(sndbuf,"\x00\xB0\x96\x00\x37",i);//持卡人基本信息 55字节
// #ifdef _debug_JTB_
// 	debugstring("***READ 16:\r\n");
// 	debugstring("SND_C:");
// 	debugdata(sndbuf, i, 1);
// #endif
// 	ret = RfPassthrough(revbuf, 2, sndbuf, i);
// #ifdef _debug_JTB_
// 	debugstring("RCV_C16:");
// 	debugdata(revbuf, ret, 1);
// #endif	
// 	if(ret == 0){
// 	//	ERR_DIS("读16文件失败",revbuf);
// 		return ST_ERROR;
// 	}
// 	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0)
// 	{
// 		ERR_DIS("读16文件失败",revbuf+ret-2);
// 		MSG_LOG("16文件读出错");
// 		return ST_ERROR;
// 	}
// //	jtb16file = (stJTB16 *)(revbuf+1);
// 	memcpy((unsigned char*)&jtb16file,revbuf+1,55);
// #ifdef _debug_JTB_
// 	debugstring("卡类别标识:");
// 	debugdata((unsigned char*)&jtb16file.cCardType,1,1);
// 	debugstring("本行职工标识:");
// 	debugdata((unsigned char*)&jtb16file.cStuffcode,1,1);
// 	debugstring("证件名字:");
// 	debugdata(jtb16file.cName,20,1);
// 	debugstring("证件号:");
// 	debugdata(jtb16file.cCredent,32,1);
// 	debugstring("证件类型:");
// 	debugdata((unsigned char*)&jtb16file.cCretype,1,1);
// 	
// #endif
// 
// #endif //if 0
	//88888888888888888888888888888888888
	i=5;
	memcpy(sndbuf,"\x00\xB0\x97\x00\x3C",i);//管理信息文件
#ifdef _debug_JTB_
	debugstring("***READ 17:\r\n");
	debugstring("SND_C:");
	debugdata(sndbuf, i, 1);
#endif
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_JTB_
	debugstring("RCV_C17:");
	debugdata(revbuf, ret, 1);
#endif
	
	if(ret == 0){
	//	ERR_DIS("读17文件失败",revbuf);
		return ST_ERROR;
	}
	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0)
	{
		ERR_DIS("ERRO 17",revbuf+ret-2);
		return ST_ERROR;
	}
	memcpy((unsigned char*)&jtb17file,revbuf+1,60);
#ifdef _debug_JTB_
	debugstring("国际代码:");
	debugdata(jtb17file.intercode,4,1);
	debugstring("省级代码:");
	debugdata(jtb17file.provcode,2,1);
	debugstring("城市代码:");
	debugdata(jtb17file.Citycode,2,1);
	debugstring("互通卡钟:");
	debugdata(jtb17file.car_type,2,1);//要对互通卡钟判断
	debugstring("卡类型:");
	debugdata((unsigned char*)&jtb17file.card_catalog,1,1);
	debugstring("预留:");
	debugdata(jtb17file.reser,49,1);

#endif

#ifdef CARD_JTB
	gCardinfo.card_catalog=CARD_JTB;//卡类
#else
	gCardinfo.card_catalog=jtb17file.card_catalog;//卡类
#endif
	i=5;
	memcpy(sndbuf,"\x00\xB2\x01\xF4\x00",i);//1E文件
#ifdef _debug_JTB_
	debugstring("***READ 1E:\r\n");
	debugstring("SND_C:");
	debugdata(sndbuf, i, 1);
#endif
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_JTB_
	debugstring("RCV_C1E:");
	debugdata(revbuf, ret, 1);
#endif
	if(ret == 0){
		//	ERR_DIS("读1E文件失败",revbuf);
		return ST_ERROR;
	}
	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0){
		
		ERR_DIS("读1E文件失败",revbuf+ret-2);
		MSG_LOG("找不到应用----\r\n");
		return ST_ERROR;
	}
	memcpy((unsigned char*)&JTB_1E,revbuf+1,48);//1A文件数据

    //互通卡消费流程
if(gCardinfo.CardType==YiDiCard)//异地卡需要判断
{
	if(memcmp(jtb17file.car_type,"\x00\x00",2)==0){//判断是否是互通卡, 本地消费不判断
		
		ERR_DIS("不是互通卡",jtb17file.car_type);
			//	MSG_LOG("本地卡\r\n");
		
					return ST_ERROR;
	}
	if (memcmp(jtb17file.car_type,"\x00\x01",2)==0)//互通卡
	{
			
	if(!(jtb17file.card_catalog!=0||jtb17file.card_catalog<5||jtb17file.card_catalog==0xFF)){
		ERR_DIS("卡种错",jtb17file.car_type);
				return ST_ERROR;
		}
	}
}		 
//判断互通卡钟
	//判断优惠卡种
	 //
//互联互通文件消费 

//判断 一票制
	ret=IF_1E_OK((unsigned char*)&JTB_1E);
	if(ret!=ST_OK)
		return ret;

	
	ret=gCardinfo.card_catalog;
	if((gDeviceParaTab.rate[ret]==204)||(gDeviceParaTab.rate[ret]==104)||(gDeviceParaTab.rate[ret]==0xFF)||(gDeviceParaTab.rate[ret]==0))//禁止
	{

		return CARD_forbid;	
	}
	return CARD_MONEY;
}

#endif//#ifdef BUS_PRO_JTB
