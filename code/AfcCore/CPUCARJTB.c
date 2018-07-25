
/**************************************************
* �ļ��� :CPUCARJTB.c
* ����   :��ͨ��CPU�������ļ�
* ����   :15/11/2015 @zzc	
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
 stJTB1A	JTB_1A;//1A�ļ��ṹ
 stJTB15	jtb15file;//15
 stJTB16	jtb16file;//16
 stJTB17	jtb17file;//17
 stJTB18	jtb18file;//18
 stJTB1E	JTB_1E;//1A�ļ��ṹ
pboc_sam_init_purchaseJTB stuSamInitPurchaseJTB;
 
unsigned char dealtype;//�۷�ģʽ
unsigned char BUS_Flag;//����״̬ һ��8λǰ��λ�ֱ��ʾ  1�Ƿ񷢳�2�Ƿ��ڳ�վ3�Ƿ�ˢ�˷�����
FeUnKnowREC FeuRec;	//


unsigned short WhiteLength;		////���ذ�������ʱ��¼������


//������ ����������� ȡ0x15�ļ��µķ���������ʶ��01-08 HEX����ǰ8λ��������Ƚ�
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

	//��ǰ AID=ASCII��MOT.CPTSAM01��
	//A0 00 00 06 32 4D 4F 54 2E 43 50 53 41 4D 30 31

	memcpy(buffer, "\xA0\x00\x00\x06\x32\x4D\x4F\x54\x2E\x43\x50\x53\x41\x4D\x30\x31", 16);		//JTB����Ǯ��AID
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

	if(Read_Binary(PSAM_YD, 0x17, 0, 25, buffer)==0)		//��0x17�ļ�25�ֽ�
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
	memcpy((unsigned char*)&SamPubInf_LL, buffer, sizeof(sam_pub));	//��һ���ֽ�����Կ����
	 
	psamJTB.Flag = 0;
	return ST_OK;
}
extern unsigned int ghisCur;//��ʷ��ǰָ��
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
	if (memcmp(historyCheck.rTTTT,"\xAA\xAA\xAA\xAA",4)!=0)//�Ӹ��жϣ���֪��Ϊɶ ��ʱ�����
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
		sprintf((char*)buffer,"�ۿ�:%d.%02d ",temp/100, temp%100);
		memset(btff, 0, 20);//���뿨��
		getCartypeJTB(btff, historyCheck.rCardType);
		strcat((char*)buffer, btff);
		display(2,0,(char*)buffer,0);
		memset(buffer,0,20);
		temp=0;
		memcpy((unsigned char*)&temp, historyCheck.rAfterMoney, 4);
			sprintf((char*)buffer, "���:%d.%02d", temp/100, temp%100);
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
				sprintf((char*)buffer, "�۴�:%d ",temp);
			memset(btff, 0, 20);//���뿨��
			getCartypeJTB(btff, historyCheck.rCardType);
			strcat((char*)buffer, btff);
			display(2,0,(char*)buffer,0);

			memset(buffer,0,20);
			memcpy((unsigned char*)&temp, historyCheck.rAfterMoney, 4);
			sprintf((char*)buffer, "���:%d", temp);
			display(4,0,(char*)buffer,0);
		}
		break;

	case ID_REC_CLERK:
		cls();
		display(3, 0, "վ��Ա��", DIS_CENTER);
		break;

	case ID_REC_ARVBUS:
		cls();
		display(3, 0, "��վ��¼", DIS_CENTER);
		break;
	case ID_REC_LINESET:
			  cls();
		display(3, 0, "��·��", DIS_CENTER);
		break;
	case ID_REC_BLKLIST	:
		cls();
		display(3, 0, "�ڿ���¼", DIS_CENTER);
		break;
	default:
		return ST_ERROR;
	}
	if(historyCheck.rBusFlag&BUS_TES&&historyCheck.rDealType!=ID_REC_HJY_REC)//��������
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

void ERR_DIS(const char *dis,unsigned char *dat)//��ʾ����
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
//HEXתBCD  Dec ��Ҫת��������  length ��Ҫת�����ֽ��� mode==1,�������ת��
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
extern unsigned char BUS_Flag;//����״̬
void BuildRecordeJTB(unsigned char delType, unsigned char *recBuf)
{
	DEALRECODE_JTB *rRecordDeal;
	unsigned int temp;
	//		unsigned char ttt;
	rRecordDeal = (DEALRECODE_JTB *)recBuf;
	memset(rRecordDeal->rPublishNumb,0,128);
	memcpy(rRecordDeal->rPublishNumb, gCardinfo.PublishBicker, 10);//����
	rRecordDeal->rCardType = jtb17file.card_catalog;//gCardinfo.card_catalog;				//����
	rRecordDeal->rCardCand = gCardinfo.gMCardCand;							//������
	memcpy(rRecordDeal->rSAMxulie,stuDebitPurchase.cTermSnq,4);		//PSAM������ˮ
	memcpy(rRecordDeal->rPSAMNO,psamJTB.CardNO,6);						//�ն˱���PSAM����
#ifdef _NEW_Device
	ASCII2ZBcod((char*)gDeviceParaTab.DeviceNo,rRecordDeal->rDeviceNo, 4);	//�������豸�� ����
#else
	Ascii2BCD(gDeviceParaTab.DeviceNo,rRecordDeal->rDeviceNo,8);
#endif
	
	rRecordDeal->rDealType = delType;								//��������
	memcpy(rRecordDeal->rPiaoJia,gDeviceParaTab.busPrice,2);			//Ʊ��
	memcpy(rRecordDeal->rMoneyDealSum,stuInitPurchaseRet.cSnq,2);	//Ǯ�����״���
	memcpy(rRecordDeal->rDealMoney, (unsigned char *)&s_sum1, 3);	//���׽��
	memcpy(rRecordDeal->rAfterMoney, (unsigned char*)&a_sum1, 4);	//�������
	memcpy(rRecordDeal->rDealTime,(unsigned char*)&SysTime,7);		//����ʱ��
	memcpy(rRecordDeal->rCPUTAC,stuDebitPurchase.cMac,4);			//TAC
	memcpy(rRecordDeal->rDriverNo, (unsigned char*)&gBuInfo.DriverNO, 4);//˾������

	memcpy(rRecordDeal->rLineNo, gDeviceParaTab.LineNo, 3);				//��·��

	memcpy(rRecordDeal->rFactorFlg,jtb15file.cPublisherSign,8);		//��������15
	memcpy(rRecordDeal->rCityCode,jtb17file.Citycode,2);			//���д���15
	if(delType==ID_REC_HJY_REC)
		rRecordDeal->rHUIJIAOYI=1;										//�ҽ��ױ�ʶ15
	else
		rRecordDeal->rHUIJIAOYI=0;
	
	//
#ifdef _debug_JTB_
	BUS_Flag|=BUS_TES;//���Լ�¼
#else
	BUS_Flag&=BUS_CL_TES;
#endif
	rRecordDeal->rBusFlag=BUS_Flag;
	//����14
	temp = SOFT_VER_TIME_LOG;
	memcpy(rRecordDeal->rProVer, (unsigned char*)&temp, 2);			//����汾
	memcpy(rRecordDeal->rTTTT,"\xAA\xAA\xAA\xAA",4);				//�������� ������ĸ�AA
	
#ifdef _debug_JTB_
	debugstring("��֯��¼\r\n");
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
// ��������64�ֽڳ���д128��,mode=0;����128��  mode=1,����128
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
	if((mode==1)&&(memcmp(rec+124,"\xAA\xAA\xAA\xAA",4)!=0))//����128��
	{
		MSG_LOG("������");
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
		else{//д������дȫ0������дʵ���ݣ���д����һ���ݵ�ַ
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
	display(2, 2, "E2PROMд����,��Ҫά��!", 1);
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

	JTB_1ER.eDealType=mode;		//�³���
	memcpy(JTB_1ER.eDeviceNum+2,psamJTB.CardNO,6);
	JTB_1ER.eTracode=02;		//��ҵ����
	memcpy(JTB_1ER.eLineNUm,gDeviceParaTab.LineNo,2);
	memcpy((unsigned char*)&itm,(unsigned char*)&s_sum1,4);
	over_turn(4,(unsigned char*)&itm);
	memcpy(JTB_1ER.eDmoney,(unsigned char*)&itm,4);//���׽��
	memcpy((unsigned char*)&itm,(unsigned char*)&a_sum1,4);
	over_turn(4,(unsigned char*)&itm);
	memcpy(JTB_1ER.eYuE,(unsigned char*)&itm,4);//���
	memcpy(JTB_1ER.eDealTime,(unsigned char*)&SysTime,7);
	memcpy(JTB_1ER.eCityco,jtb17file.Citycode,2);
	memcpy(JTB_1ER.eFactoryco,jtb15file.cPublisherSign,8);

	
	itm=5;
	over_turn(4,(unsigned char*)&itm);
	memcpy(JTB_1ER.eErrPriceMax,(unsigned char*)&itm,4);				//������ѽ��HEX(���)
	//������ѽ��
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
	memcpy(sndbuf, "\x00\xA4\x04\x00\x08\xA0\x00\x00\x06\x32\x01\x01\x05", i);		//JTB����Ǯ��AID
	//memcpy(sndbuf, "\x02\x00\xA4\x00\x00\x02\x3F\x01",8);//�д˿���Ҫ���ļ���������ȡ����ʱ��Ҫ����һ�¡�
//	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf, 13);
	if(ret == 0)
		return ST_ERROR;

	memcpy(sndbuf, "\02\x00\x84\x00\x00\x08", 6); i = 6;
	deal2or3(sndbuf);	
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_JTB_
	debugstring("�����:");
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
	if(getMiKey() != ST_OK)	//��֤������֤��
		return 8;
	//----------����KEY----------
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
//	Mac(CPUPKEY, Rader, snddat, 5, CPUinitdat, revbuf);//1BE5F163F50843699E120D74FAF08E76 ����
	//CountMac(CPUPKEY, Rader, snddat, 5,revbuf);
#else//$ NO_CPU_PSAM_  ��ˮ��

//	memcpy(sndbuf,stuPubInfZaoZuang.cPublisherSign,8);
	memcpy(sndbuf,gCardinfo.PublishBicker+2,8);//���ź�8�ֽ�
	i =8;
	memcpy(sndbuf+i,gCardinfo.FactorFlg,8);//������������
	i+=8;
	//memcpy(sndbuf+i,gCardinfo.CityCode,2);i+=2;
	//memcpy(sndbuf+i,"\xFF\x00\x00\x00\x00\x00",6);i+=6;
		
#ifdef _debug_JTB_
	debugstring("PSAM_MAC��ʼ��ָ��:");
	debugdata((unsigned char *)&i,4,1);
	debugdata(sndbuf,i,1);
#endif
	if (CpuCardSetMac2(PSAM_GJ,revbuf,sndbuf,i) == ST_ERROR){	//801A450110
		return ST_ERROR;
	}

	memcpy(sndbuf,Rader,8);i=8;//��Կ
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
	debugstring("����:");
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
//	memcpy(sndbuf, "\x02\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01", 15); // ѡ��3F01
	memcpy(sndbuf, "\x00\xA4\x04\x00\x08\xA0\x00\x00\x06\x32\x01\x01\x05", i);		//JTB����Ǯ��AID
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
//mode==1  д�����磬mode==2ɾ�������¼  3 ����Ƿ��м�¼��Ҫת��      �������δ���  �ϵ�û��¼�����
void Set_UnknowREC(unsigned char mode,unsigned char *data){

	return;
// 	//FeUnKnowREC buffer;
// 	unsigned int CRC;
// 	unsigned char i;
// 
// 	if(mode==FeRC_Write){
// 		memcpy(FeuRec.falg,"��Ч",4);
// 		memcpy(FeuRec.RCE,data,RECORD_JTB_LEN);
// 		CRC=__cpuCrc32(FeuRec.falg,sizeof(FeUnKnowREC)-4);
// 		memcpy(FeuRec.CRC,(unsigned char*)&CRC,4);
// 		sysfewrite(BIT_UNKNOW_RECORD,sizeof(FeUnKnowREC),(unsigned char*)&FeuRec);
// 		MSG_LOG("д�����¼\r\n ");
// 		BCD_LOG(FeuRec.falg,sizeof(FeUnKnowREC),1);
// 	}
// 	else if (mode==FeRC_Dlelt)//ɾ��
// 	{
// 		
// 		memset((unsigned char*)&FeuRec,0,sizeof(FeUnKnowREC));
// 		CRC=__cpuCrc32(FeuRec.falg,sizeof(FeUnKnowREC)-4);
// 		memcpy(FeuRec.CRC,(unsigned char*)&CRC,4);
// 		sysfewrite(BIT_UNKNOW_RECORD,sizeof(FeUnKnowREC),(unsigned char*)&FeuRec);
// 		MSG_LOG("ɾ�������¼\r\n ");
// 		BCD_LOG(FeuRec.falg,sizeof(FeUnKnowREC),1);
// 	}else if (mode==FeRC_Check)//�ж��Ƿ��м�¼��Ҫת��
// 	{
// 		
// 		for(i=0;i<5;i++)
// 		{
// 			sysferead(BIT_UNKNOW_RECORD,sizeof(FeUnKnowREC),(unsigned char*)&FeuRec);
// 			CRC=__cpuCrc32(FeuRec.falg,sizeof(FeUnKnowREC)-4);
// 			if(memcmp((unsigned char*)&CRC,FeuRec.CRC,4)==0)//��ȷ
// 				break;
// 		}
// 
// 		MSG_LOG("FeuREC:\r\n");
// 		BCD_LOG((unsigned char*)&i,1,1);
// 		BCD_LOG(FeuRec.falg,136,1);
// 
// 		if(memcmp(FeuRec.falg,"��Ч",4)==0){//�м�¼
// 			WriRecorJTB(FeuRec.RCE,0);		//д����¼
// 		//ת�������
// 		memset((unsigned char*)&FeuRec,0,sizeof(FeUnKnowREC));
// 		CRC=__cpuCrc32(FeuRec.falg,sizeof(FeUnKnowREC)-4);
// 		memcpy(FeuRec.CRC,(unsigned char*)&CRC,4);
// 		sysfewrite(BIT_UNKNOW_RECORD,sizeof(FeUnKnowREC),(unsigned char*)&FeuRec);
// 		MSG_LOG("��¼ת��\r\n");
// 		return;
// 		}
// 		MSG_LOG("û�м�¼\r\n");
// 	}
		

}




//��ˢȡTAC��ں��� 4 MAC1+4 tac
unsigned char GET_TRANSACTION_PROVE(unsigned char*outdata,unsigned char mode)
{
	unsigned char i;
	unsigned char sndbuf[100];
	unsigned int ret;
	unsigned char revbuf[128];
	unsigned int kk=0;
	unsigned short ii;
	
	memset(outdata, 0, 8);//���ص�TAC��
	
	i=5;
	memcpy(sndbuf,"\x00\xB2\x01\xC4\x00",i);//18�ļ�
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_
	debugstring("RCV_C18:");
	debugdata(revbuf, ret, 1);
#endif
	
	
	memcpy(jtb18file.rEPsearl,revbuf+1,sizeof(jtb18file));
	
	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0){
		MSG_LOG("18�ļ���\r\n");
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
	
	if(memcmp(jtb18file.rPSAMnum,psamJTB.CardNO,6)!=0)//����ͬһ���ն�
	{
		MSG_LOG("�ն˲�һ��\r\n");
#ifndef _debug_
		return ST_ERROR;
#endif
	}
	if(memcmp((unsigned char*)&SysTime,jtb18file.rPursTime,7)>=0)
		kk=DataTimeDec((unsigned char*)&SysTime,jtb18file.rPursTime);
	else
	{
		MSG_LOG("ʱ�����\r\n");
#ifndef _debug_
		return ST_ERROR;
#endif
	}
	BCD_LOG((unsigned char*)&SysTime,7,1);
	BCD_LOG(jtb18file.rPursTime,7,1);
	MSG_LOG("��ȡ %d����\r\n",kk);
	if(kk<120)//��������
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
			MSG_LOG("��ȡʧ����+++++++++\r\n");
			beep(2,100,100);
			return ST_ERROR;
		}
		memcpy(outdata, revbuf+1, 8);//���ص�TAC��
		MSG_LOG("TAC��ȡ�ɹ� \r\n");
		BCD_LOG(outdata,8,1);
		return ST_OK;
	}
	MSG_LOG("ʱ�䲻����\r\n");
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
		display(0,0,"����:",1);
		display(3,0,"��ͨPSAM����!",0);

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
		MessageBox(1,"��������");
		beep(2,50,50);
		gBuInfo.g24GDisFlash=5;
		gBuInfo.restore_flag=0;
		return ST_ERROR;
	}
	over_turn(4, (unsigned char*)&value);
	a_sum1 = value;		//���


	if(cardSound == 0xaa){
		if(card_ser!=gCardinfo.c_serial){
			Set_UnknowREC(FeRC_Check,revbuf);//��ˢ����Ŀ�������,Ҫ����һ�����ļ�¼��������
			card_ser = 0;
			cardSound = 0;
		}
	}
	if(cardSound == 0xaa){//��Ҫ��ˢ
		if(value < a_sumR){//�ϴ��Ѿ��۳ɹ�
// 			memcpy(sndbuf, "\x02\x80\x5A\x00\x06\x02", 6);//memcpy(sndbuf, "\x02\x80\x5A\x00\x05\x02", 6);
// 				sndbuf[4]=stuSamInitPurchaseJTB.cType;
// 			memcpy((unsigned char*)&ii, stuInitPurchaseRet.cSnq, 2);
// 			over_turn(2, (unsigned char*)&ii);
// 			ii += 1;
// 			over_turn(2, (unsigned char*)&ii);
// 			memcpy(sndbuf+6, (unsigned char*)&ii, 2);
// 			sndbuf[9] = 8;
// #ifdef _debug_CPU_
// 			debugstring("����:");
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
// 			if(revLen == 0)//���س�ʱ,����
// 				return ST_ERROR;
// 			if((memcmp(revbuf+revLen-2, "\x90\x00", 2)!=0) && (memcmp(revbuf+revLen-2, "\x6C\x08", 2)!=0))
// 				goto CPUMAC2deal;//ȡ���ɹ�02 69 85 
// 			memcpy(TACC, revbuf+revLen-6, 4);//���ص�TAC��
// 			memcpy(sndbuf, revbuf+revLen-10, 4);//��MAC2��
			
			if(GET_TRANSACTION_PROVE(revbuf, 9) == ST_ERROR){
				if(gCardinfo.CardType == LocakCard)
					goto CPUMAC2deal;				//����Ǳ��ؼ�¼����Ҫ�ϴ��ġ�
				else
					goto CPUdealEnd;	//����д��¼��ע��TAC��������д��¼��
			}
			
			memcpy(sndbuf, revbuf, 4);//��MAC2��
			memcpy(TACC, revbuf+4, 4);//���ص�TAC��
			
			//�����������Ʊ����M1����ȥ��һ�Σ���Щ���������Ѿ����ˣ�Ϊ�˷�ֹд��¼�����⣬����ȡֵ
			//��ʵǰ����ˢ���ƺ��ˣ���ôû�б�ҪҪ��

			s_sum1 = get_s_sum1(0);	
			
			goto CPUMAC2deal0;
		}	

	}
	a_sumR = value;		//��֮ǰ���
	//8050 0102 0B||��Կ��ʶ�� [1]��0x01��||���׽��[4](0x00000001)||�ն˻����[6]
	//8050 0302 0B  ��������
	i=5;
	memcpy(sndbuf, "\x80\x50\x03\x02\x0B", i);//11 INITIALIZE FOR PURCHASE����
	sndbuf[i++] = SamPubInf_LL.kid;	//��Կ��
//	debugdata((unsigned char*)&SamPubInf_LL,25,1);
//

	s_sum1 = get_s_sum1(0);//ȡ�����ѽ��

	
	MSG_LOG("���ѽ��:");
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
	//	ERR_DIS("��ʼ������",revbuf);
		return ST_ERROR;
	}
	else{
		if(memcmp(revbuf+ret-2, "\x94\x01", 2)==0){
			if(cool == 4)//��Ʊ��ˢ����������ʾ����
				return 4;
			else
				return 3;			//����
		}
		else if(memcmp(revbuf+ret-2, "\x90\x00", 2)!=0){
			ERR_DIS("��ʼ������ʧ��",revbuf+ret-2);
			return 5;
		}
	}	
	memcpy((unsigned char*)&stuInitPurchaseRet, revbuf+1, sizeof(stuInitPurchaseRet));
#ifdef _debug_JTB_
	debugstring("8050ָ���:::\r\n");
	debugdata((unsigned char*)&stuInitPurchaseRet, sizeof(stuInitPurchaseRet), 1);
	debugstring("�����::");
	debugdata(stuInitPurchaseRet.cBalance,4,1);
	debugstring("��Կ�汾���㷨��ʶ:");
	debugdata(&stuInitPurchaseRet.cKeyVer,2,1);
#endif
		if(GetDateTime())
			return ST_ERROR;
	memcpy(stuSamInitPurchaseJTB.cDateTime,(unsigned char*)&SysTime,7);
	memcpy(stuSamInitPurchaseJTB.cRand,stuInitPurchaseRet.cRand,4);
	memcpy(stuSamInitPurchaseJTB.cSnq,stuInitPurchaseRet.cSnq,2);
	memcpy(stuSamInitPurchaseJTB.cMoney,(unsigned char*)&uitemp,4);
	stuSamInitPurchaseJTB.cType=0x09;//��������
	stuSamInitPurchaseJTB.cKeyVer=stuInitPurchaseRet.cKeyVer;
	stuSamInitPurchaseJTB.cKeyEn=stuInitPurchaseRet.cKeyEn;

	memcpy(stuSamInitPurchaseJTB.cPublisherSign,gCardinfo.PublishBicker+2,8);//���ź�8�ֽ�
	memcpy(stuSamInitPurchaseJTB.cKeyFactor,gCardinfo.FactorFlg,8);//������������
	
	memcpy((unsigned char*)&a_sum1, stuInitPurchaseRet.cBalance, 4);//���
	over_turn(4, (unsigned char*)&a_sum1);
	
#ifdef _debug_JTB_
	MSG_LOG("a_sum:%d.%dԪ\r\n",a_sum1/100,a_sum1%100);
#endif
	gCardinfo.gucSoudnorm=0;
	if((a_sum1>=s_sum1)&&(a_sum1<500)){
		gCardinfo.gucSoudnorm = 1;
	}

 #ifdef _debug_JTB_
	debugstring("JTB MAC1����ָ��:");
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
	debugstring("8070ָ���:::\r\n");
	debugdata(revbuf, 9, 1);
#endif	
		memcpy(temrev,revbuf,8);//�ն����к�+MAC
		memcpy(stuDebitPurchase.cTermSnq,revbuf,4);//�ն˽�����ˮ
		memcpy(stuDebitPurchase.cDateTime,stuSamInitPurchaseJTB.cDateTime,7);
#ifdef _debug_JTB_
		debugstring("stuDebitPurchase\r\n");
		debugdata(stuDebitPurchase.cTermSnq,15,1);
#endif


		//����0X1E�ļ�80DC 00 F0 
		memcpy(sndbuf,"\x80\xDC\x00\xF0\x30",5);
		//�ۿ�
		i=5;
		Build_1E_Rec(sndbuf+i,dealtype);//��֯1A�ļ�
		i+=48;
		ret = RfPassthrough(revbuf, 2, sndbuf, i);
		
#ifdef _debug_JTB_
		debugstring("����1E:");
		debugdata(sndbuf, i, 1);
		debugdata(revbuf, 10, 1);
#endif
		if(ret==0){
		//	ERR_DIS("����1Eʧ��",revbuf);
			return ST_ERROR;
		}
		if(memcmp(revbuf+ret-2, "\x90\x00", 2)!=0){
			ERR_DIS("����1E�ļ�ʧ��",revbuf+ret-2);
			 return ST_ERROR;
		}
		BuildRecordeJTB(ID_REC_HJY_REC, revbuf);//�����ɼ�¼
		Set_UnknowREC(FeRC_Write,revbuf);//д������
		//*****************************
		i=5;
		memcpy(sndbuf, "\x80\x54\x01\x00\x0F", i);//11 INITIALIZE FOR PURCHASE����
		memcpy(sndbuf+i,temrev,4);//�ն˽������к�
		i+=4;
		memcpy(sndbuf+i,stuSamInitPurchaseJTB.cDateTime,7);
		i+=7;
		memcpy(sndbuf+i,temrev+4,4);//MAC1
		i+=4;	
		ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_JTB_
		debugstring("�۷�!:");
		debugdata(sndbuf, i, 1);
		debugdata(revbuf, ret, 1);
#endif
		if(ret==0){
			cardSound = 0xaa;//������Ҫ��ˢ��
			card_ser = gCardinfo.c_serial;
			audio(Audio_TRY_AGAIN);
			return 7;
		}
		if(memcmp(revbuf+ret-2, "\x93\x02", 2)==0)
			return 9;//�Ƿ���
		if(memcmp(revbuf+ret-2, "\x90\x00", 2)!=0){
			return 7;
		}			 
		memcpy(sndbuf,revbuf+1+4,4);//MAC2
	//	memcpy(stuDebitPurchase.cMac,revbuf+1,4);//���׵�MAC
		memcpy(TACC,revbuf+1,4);
		a_sum1-=s_sum1;//�ۿ�Ҫ��������
CPUMAC2deal0:
#ifdef _debug_JTB_
	debugstring("У��MAC2::\r\n");
#endif

		if(CPUMAC2(PSAM_YD,revbuf, sndbuf, 4)==ST_ERROR)//У��MAC2
			ERR_DIS("MAC2У�����",revbuf);

CPUMAC2deal:
		memcpy(gCardinfo.PbocTac, TACC, 4);	// TAC��
		BuildRecordeJTB(ID_REC_TOLL, revbuf);
		WriRecorJTB(revbuf,0);


// 	memcpy(jsb_recbuff.TAC,gCardinfo.PbocTac,4);
	
#ifdef _debug_JTB_
	debugstring("����MAC2:");
	debugdata(revbuf, 10, 1);
#endif

CPUdealEnd:	
	card_ser = 0;
	cardSound = 0;
//	display(2,1,"���ѳɹ�",DIS_CENTER);
	money_msg(ID_REC_TOLL, a_sum1, s_sum1, 0);
//	Sound_cityCMD(0);
	audio(Audio_DONG);
	Set_UnknowREC(FeRC_Dlelt,revbuf);//ɾ������ļ�¼
//if(gCardinfo.card_catalog!=1)//��ͨ��
//	save_h_month();
//	Save_delayCardinfo(mode);

	return ST_OK;//���ѳɹ�
}
//���Ұ�����
unsigned char IsWriteNUM(unsigned int Write)
{
	unsigned char i;

//		MSG_LOG("���Ұ�����\r\n");
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
	memcpy((unsigned char*)&NowTime, (unsigned char *)&SysTime, 4);//��ǰ����
	over_turn(4, (unsigned char*)&NowTime);
	memcpy((unsigned char*)&start_time, jtb15file.cIssueDate, 4);//��������
	over_turn(4, (unsigned char*)&start_time);
	memcpy((unsigned char*)&end_time, jtb15file.cValidDate, 4);//��Ч����
	over_turn(4, (unsigned char*)&end_time);
	memcpy(gCardinfo.FactorFlg,jtb15file.cPublisherSign,8);
	memset(disbuf,0xff,2);
	if(jtb15file.cStarFlag!=0x01){//0 δ���ã�1 ���� 2 ͣ��
		disbuf[0]=0;
		disbuf[1]=jtb15file.cStarFlag;
		ERR_DIS("���ñ�ʶ����",disbuf);
		return ST_ERROR;//δ����
	}
	uitemp=BCD2int((unsigned char*)&start_time,4);
	//�ж�����ʱ��
	if(memcmp((unsigned char*)&start_time,"\x00\x00\x00\x00",4)==0||memcmp((unsigned char*)&start_time,"\xFF\xFF\xFF\xFF",4)==0||memcmp((unsigned char*)&end_time,"\x00\x00\x00\x00",4)==0||memcmp((unsigned char*)&end_time,"\xFF\xFF\xFF\xFF",4)==0)
	{	memcpy(disbuf,(unsigned char*)&start_time,2);
		ERR_DIS("���ڸ�ʽ����",disbuf);
		 return ST_ERROR;
	}
	//�������������ж�
	if(((uitemp%4==0)&&(uitemp%100!=0))||(uitemp%400==0))  
	{
		//����
		if(memcmp((unsigned char*)&start_time,"\x29",1)>0&&memcmp((unsigned char*)&start_time+1,"\x02",1)==0){
			//MessageBox(1,"�������ڸ�ʽ����");
			memcpy(disbuf,(unsigned char*)&start_time,2);
			MSG_LOG("����");
			ERR_DIS("�������ڸ�ʽ����",disbuf);
		//	return ST_ERROR;
		}
	}
	else{
		//ƽ��
		if(memcmp((unsigned char*)&start_time,"\x28",1)>0&&memcmp((unsigned char*)&start_time+1,"\x02",1)==0){
			MSG_LOG("ƽ��");
			MessageBox(1,"�������ڸ�ʽ����");
		//	return ST_ERROR;
		}
		
	}
	//���������ж�
	uitemp=BCD2int((unsigned char*)&end_time,4);
	//��Ч����
	if(((uitemp%4==0)&&(uitemp%100!=0))||(uitemp%400==0))  
	{
		//����
		if(memcmp((unsigned char*)&end_time,"\x29",1)>0&&memcmp((unsigned char*)&end_time+1,"\x02",1)==0){
			MessageBox(1,"��Ч���ڸ�ʽ����");
		//	return ST_ERROR;
		}
	}
	else{
		//ƽ��
		if(memcmp((unsigned char*)&end_time,"\x28",1)>0&&memcmp((unsigned char*)&end_time+1,"\x02",1)==0){
			
			MessageBox(1,"��Ч���ڸ�ʽ����");
		//	return ST_ERROR;
		}
		
	}

	//�ж��Ƿ����
	if((start_time>NowTime)||(NowTime > end_time)){//����ʱ���ж� ����ҲҪͨ��

		return CARD_NO_TIME;//���ڻ�ĩ����
	}
//�жϸ��˻����ñ�ʶ
	if (jtb15file.cAppType!=02)//���˻�Ӧ�ñ�ʶ��
	{
			disbuf[1]=jtb15file.cAppType;
			disbuf[0]=0x00;
			ERR_DIS("���˻���ʶ��",disbuf);
		return ST_ERROR;//δ����
	}

	if(memcmp(gCardinfo.FactorFlg,"\x01\x58\x12\x70",4)==0)//���ؿ�
		gCardinfo.CardType=LocakCard;//���ؿ� 
	else
		gCardinfo.CardType=YiDiCard;//��ؿ�
#if 0
 	if(memcmp(gCardinfo.FactorFlg,"\x01\x58\x12\x70",4)!=0)//���ؿ�
		return NO_WriteNUM;//���ڰ�������

  //		CardType=LocakCard;//���ؿ� 
//  	else
//  		CardType=YiDiCard;//��ؿ�

	//CardType=YiDiCard;//��ؿ�

	//�жϿ��Ļ��������Ƿ��ڰ�������
	memcpy((unsigned char*)&temp,gCardinfo.FactorFlg,4);//ȡ�������������ǰ�ĸ��ֽ�
	over_turn(4,(unsigned char*)&temp);
	if(IsWriteNUM(temp))
			return NO_WriteNUM;//���ڰ�������

//	if(IsInWhileList(gCardinfo.FactorFlg)!=ST_ERROR)//��
//		return NO_WriteNUM;//���ڰ�������

#endif


	return ST_OK;
}

unsigned char IF_1E_OK(unsigned char *Data)
{
//	unsigned short i;
	unsigned char disbuf[2];
//	unsigned int temp;

	//�ж�1E
	if(JTB_1E.eDealType>10){
		disbuf[0]=JTB_1E.eDealType;
		disbuf[1]=0;
		ERR_DIS("��1E�������ʹ�",disbuf);
		return ST_ERROR;
		}

	
		dealtype=ON_Nomal;//ȥ����
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
					debugstring("����---\r\n");
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
					debugstring("��ǰ---\r\n");
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
//0 ����   1 ��
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
//00��ء�01����
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
	//������ʱȡ��  �ٶȻؿ�ܶ�
// 	memcpy(sndbuf,"\xe0\x50",2);
// 	ret = RfPassthrough(revbuf, 2, sndbuf, 2);
// 	mode2or3 = 2;
// 	delayxms(2);

	gCardinfo.gMCardCand= CARDSTYLE_JTB;
#if 0  //2016��11��15��09:03:17 cai
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
	memcpy(sndbuf, "\x00\xA4\x04\x00\x08\xA0\x00\x00\x06\x32\x01\x01\x05", i);		//JTB����Ǯ��AID
#ifdef _debug_JTB_
	debugstring("***select AID1:");
	debugstring("SND_C:");
	debugdata(sndbuf, i, 1);
#endif
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_JTB_//ѡ��Ӧ�÷��ص���������Դ
	debugstring("RCV_C:");
	debugdata(revbuf, ret, 1);
#endif
	if(ret == 0){
		//MessageBox(1,"ѡ��AIDʧ��");
	//	beep(2,50,50);
		gBuInfo.g24GDisFlash=5;
		gBuInfo.restore_flag=0;
		return ST_ERROR;
	}
	
	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0)
	{
		MSG_LOG("�Ҳ���Ӧ��----\r\n");
		//ERR_DIS("ѡ��AIDʧ��",revbuf+ret-2);
		return ST_ERROR;
	}

	if(memcmp(revbuf+ret-2, "\x6A\x81", 2) == 0)
		return CARD_STOPWORK;
	if(memcmp(revbuf+ret-2, "\x93\x03", 2) == 0)
		return APP_LOCK;

	i=5;
	memcpy(sndbuf,"\x00\xB0\x95\x00\x1E",i);//����Ӧ����Ϣ�ļ� 30�ֽ�
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
	//	ERR_DIS("��1A�ļ�ʧ��",revbuf);
		return ST_ERROR;
	}
	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0)
	{
		ERR_DIS("��15�ļ�ʧ��",revbuf+ret-2);
		return ST_ERROR;
	}
	//	jtb15file = (stJTB15 *)(revbuf+1);
	memcpy((unsigned char*)&jtb15file,revbuf+1,30);
	memcpy(gCardinfo.PublishBicker, jtb15file.cApp_sn,10);
	memcpy((unsigned char*)&uitemp, (unsigned char *)&SysTime, 4);//��ǰ����
	over_turn(4, (unsigned char*)&uitemp);
	memcpy((unsigned char*)&start_time, jtb15file.cIssueDate, 4);//��������
	over_turn(4, (unsigned char*)&start_time);
	memcpy((unsigned char*)&end_time, jtb15file.cValidDate, 4);//��Ч����
	over_turn(4, (unsigned char*)&end_time);
	memcpy(gCardinfo.FactorFlg,jtb15file.cPublisherSign,8);
	
#ifdef _debug_JTB_
	debugstring("����:");
	debugdata(gCardinfo.PublishBicker,10,1);
	debugstring("����������ʶ:");
	debugdata(gCardinfo.FactorFlg,8,1);
	debugstring("���ñ�ʶ:");
	debugdata((unsigned char *)&jtb15file.cStarFlag,1,1);
	MSG_LOG("��������::%08X\r\n",start_time);
	MSG_LOG("��ǰ����::%08X\r\n",uitemp);
	MSG_LOG("��Ч����::%08X\r\n",end_time);
#endif
	  ret=IF_15_OK(revbuf+1);
	  if(ret!=ST_OK)
		  return ret;
//

#if 0//������ͨ ��ʱû�к�����
	if(IsInBlackList(stuPubInfZaoZuang.cPublisherSign)){
#ifdef _debug_JTB_
		debugstring("lock card:-----\r\n");
#endif
		return CARD_BLACK_PRO;
		
	}
#endif//


// #if 0  //��Ҫ���� 2016��9��10��07:00:21
// 	i=5;
// 	memcpy(sndbuf,"\x00\xB0\x96\x00\x37",i);//�ֿ��˻�����Ϣ 55�ֽ�
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
// 	//	ERR_DIS("��16�ļ�ʧ��",revbuf);
// 		return ST_ERROR;
// 	}
// 	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0)
// 	{
// 		ERR_DIS("��16�ļ�ʧ��",revbuf+ret-2);
// 		MSG_LOG("16�ļ�������");
// 		return ST_ERROR;
// 	}
// //	jtb16file = (stJTB16 *)(revbuf+1);
// 	memcpy((unsigned char*)&jtb16file,revbuf+1,55);
// #ifdef _debug_JTB_
// 	debugstring("������ʶ:");
// 	debugdata((unsigned char*)&jtb16file.cCardType,1,1);
// 	debugstring("����ְ����ʶ:");
// 	debugdata((unsigned char*)&jtb16file.cStuffcode,1,1);
// 	debugstring("֤������:");
// 	debugdata(jtb16file.cName,20,1);
// 	debugstring("֤����:");
// 	debugdata(jtb16file.cCredent,32,1);
// 	debugstring("֤������:");
// 	debugdata((unsigned char*)&jtb16file.cCretype,1,1);
// 	
// #endif
// 
// #endif //if 0
	//88888888888888888888888888888888888
	i=5;
	memcpy(sndbuf,"\x00\xB0\x97\x00\x3C",i);//������Ϣ�ļ�
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
	//	ERR_DIS("��17�ļ�ʧ��",revbuf);
		return ST_ERROR;
	}
	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0)
	{
		ERR_DIS("ERRO 17",revbuf+ret-2);
		return ST_ERROR;
	}
	memcpy((unsigned char*)&jtb17file,revbuf+1,60);
#ifdef _debug_JTB_
	debugstring("���ʴ���:");
	debugdata(jtb17file.intercode,4,1);
	debugstring("ʡ������:");
	debugdata(jtb17file.provcode,2,1);
	debugstring("���д���:");
	debugdata(jtb17file.Citycode,2,1);
	debugstring("��ͨ����:");
	debugdata(jtb17file.car_type,2,1);//Ҫ�Ի�ͨ�����ж�
	debugstring("������:");
	debugdata((unsigned char*)&jtb17file.card_catalog,1,1);
	debugstring("Ԥ��:");
	debugdata(jtb17file.reser,49,1);

#endif

#ifdef CARD_JTB
	gCardinfo.card_catalog=CARD_JTB;//����
#else
	gCardinfo.card_catalog=jtb17file.card_catalog;//����
#endif
	i=5;
	memcpy(sndbuf,"\x00\xB2\x01\xF4\x00",i);//1E�ļ�
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
		//	ERR_DIS("��1E�ļ�ʧ��",revbuf);
		return ST_ERROR;
	}
	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0){
		
		ERR_DIS("��1E�ļ�ʧ��",revbuf+ret-2);
		MSG_LOG("�Ҳ���Ӧ��----\r\n");
		return ST_ERROR;
	}
	memcpy((unsigned char*)&JTB_1E,revbuf+1,48);//1A�ļ�����

    //��ͨ����������
if(gCardinfo.CardType==YiDiCard)//��ؿ���Ҫ�ж�
{
	if(memcmp(jtb17file.car_type,"\x00\x00",2)==0){//�ж��Ƿ��ǻ�ͨ��, �������Ѳ��ж�
		
		ERR_DIS("���ǻ�ͨ��",jtb17file.car_type);
			//	MSG_LOG("���ؿ�\r\n");
		
					return ST_ERROR;
	}
	if (memcmp(jtb17file.car_type,"\x00\x01",2)==0)//��ͨ��
	{
			
	if(!(jtb17file.card_catalog!=0||jtb17file.card_catalog<5||jtb17file.card_catalog==0xFF)){
		ERR_DIS("���ִ�",jtb17file.car_type);
				return ST_ERROR;
		}
	}
}		 
//�жϻ�ͨ����
	//�ж��Żݿ���
	 //
//������ͨ�ļ����� 

//�ж� һƱ��
	ret=IF_1E_OK((unsigned char*)&JTB_1E);
	if(ret!=ST_OK)
		return ret;

	
	ret=gCardinfo.card_catalog;
	if((gDeviceParaTab.rate[ret]==204)||(gDeviceParaTab.rate[ret]==104)||(gDeviceParaTab.rate[ret]==0xFF)||(gDeviceParaTab.rate[ret]==0))//��ֹ
	{

		return CARD_forbid;	
	}
	return CARD_MONEY;
}

#endif//#ifdef BUS_PRO_JTB
