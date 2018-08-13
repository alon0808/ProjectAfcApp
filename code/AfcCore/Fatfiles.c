
//USB���ܺ���
#include "string.h"
#include "stdio.h"
#include "..\include\include.h"
#include "szct.h"
#include "..\fathead/ff.h"

#include "Fatfiles.h"

#ifdef _FASH_FATs_SYS_

extern stcomtime SysTime;
extern pFistVary_1 pFistVary;
extern unsigned char Qboboc_flag;

extern void dis_messgebox(char *istr, int code, unsigned char row);
extern unsigned char keywait(unsigned char CKey, unsigned int dly50xms);
extern unsigned char GetDateTime(void);
int SaveFAT_hisRecInfor(unsigned char mode);
extern unsigned char mf_mount(char* path, unsigned char mt);



//�õ�����ʣ������
//drv:���̱��("0:"/"1:")
//total:������	 ����λKB��
//free:ʣ������	 ����λKB��
//����ֵ:0,����.����,�������
// u8 exf_getfree(u8 *drv,u32 *total,u32 *free)
// {
// 	FATFS *fs1;
// 	u8 res;
//     u32 fre_clust=0, fre_sect=0, tot_sect=0;
//     //�õ�������Ϣ�����д�����
//     res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
//     if(res==0)
// 	{											   
// 		tot_sect=(fs1->n_fatent-2)*fs1->csize;	//�õ���������
// 		fre_sect=fre_clust*fs1->csize;			//�õ�����������	   
// #if _MAX_SS!=512				  				//������С����512�ֽ�,��ת��Ϊ512�ֽ�
// 		tot_sect*=fs1->ssize/512;
// 		fre_sect*=fs1->ssize/512;
// #endif	  
// 		*total=tot_sect>>1;	//��λΪKB
// 		*free=fre_sect>>1;	//��λΪKB 
// 	}
// 	return res;
// }		   

//�õ�����ʣ������
//drv:���̱��("0:"/"1:")
//total:������	 ����λKB��
//free:ʣ������	 ����λKB��
//����ֵ:0,����.����,�������
extern unsigned char exf_getfree(char *drv,unsigned int *total,unsigned int *free);

unsigned char get_flahFatfree( unsigned int *total, unsigned int*free )
{
	// unsigned int total,free;
	char testbuf[50];
	char res;
	f_chdrive("1:");
	res=exf_getfree("1:",total,free);
	if(res==0)
	{
		MSG_LOG("total:%d ", *total);
		sprintf(testbuf,"total=%dMB\r\n",*total>>10);
		//debugstring(testbuf);
		memset(testbuf,0,50);
		MSG_LOG("free:%d ", *free);
		sprintf(testbuf,"free=%dMB\r\n",*free>>10);
		MSG_LOG(testbuf);
	} 
	else
	{
		printf("Flash Fat Error:%d\r\n", res);
	}
	return res;
}

FRESULT format_disk_RW(void)
{
	//	FATFS fs;
	//	FATFS *fls = &fs;
	FRESULT res;
	char path[20];
	//	res = f_mount(0,&fs);
	// 	if (res != FR_OK)
	// 	{
	// 		printf("\r\n�����ļ�ϵͳʧ��,�������: %u",res);
	// 		return;
	// 	}	
	display(12, 0, "���ڸ�ʽ������,���Ժ�...", 0);
	
	res = f_mkfs("1:", 1, 512);	//��ʽ��

	if(res != FR_OK){
		sprintf((char*)irda_rxBuf, "��ʽ��ʧ��,�������: %u",res);
		display(14, 0, (char*)irda_rxBuf, 0);
		return res;
	}
	strcpy(path, "curdat");
	res = f_mkdir(path);	//��curdatĿ¼
	if(res != FR_OK){
		sprintf((char*)irda_rxBuf, "��curdat����,����: %u",res);
		display(14, 0, (char*)irda_rxBuf, 0);
		return res;
	}
	display(14, 0, "��ʽ���ɹ�...", DIS_CENTER);
	return FR_OK;
	//	f_mount(0,NULL);
}


//����ִ��һ��
void checkFlashDisFAT(void)
{
	unsigned int ato, afree;
	unsigned char res;

	res = mf_mount("1:", 0);//0,&gfatfs);
	if (res != FR_OK)
	{
		printf("\r\n�����ļ�ϵͳʧ��,�������: %u",res);
		return;
	}
	
	res = get_flahFatfree(&ato, &afree);
	if ((res == FR_NO_FILESYSTEM)||(res == FR_INVALID_PARAMETER)){//û���ļ�ϵͳ����Ҫ��ʽ��
		res = get_flahFatfree(&ato, &afree);
		if((res == FR_NO_FILESYSTEM)||(res == FR_INVALID_PARAMETER)){
	// 		display(0,0,"û���ļ�ϵͳ", DIS_CENTER);
	// 		display(3, 0, "��Ҫ��ʽ��,��ȷ��!", DIS_CENTER);
// 			while(uck != KEY_ENT){
// 				uck = keywait(KEY_ENT, 2);
// 				if(uck != KEY_ENT){
// 					display(8, 0, "���ļ�ϵͳ���������г���", 0);
// 				}
// 			}

			res = format_disk_RW();
			if (res == FR_OK)
			{
				display(3, 0, "    ��ʽ�����!    ", DIS_CENTER);
				beep(1, 100, 100);
				res = get_flahFatfree(&ato, &afree);
				keywait(0, 20);
			}
			else
			{
				while(1){
					delayxms(50);
					getkey(1);
				}
			}
		}
	}
}



FIL rfile_Wrecord;
//unsigned int rfile_Wrecord_point=0;//��¼ָ�� ��4K/256=16 ��Ҫ�ر��ļ�������

unsigned char save_bit_tmprec_point(unsigned int *haddr)
{
	unsigned char rbuf[16];
	unsigned int i=0;

	MSG_LOG("sava point:%d\r\n",*haddr);
	memcpy(rbuf,haddr,4);
	i = __cpuCrc32(rbuf, BIT_TMP_RECORD_POINT_LEN-4);
	memcpy(rbuf+4,&i,4);
	sysfewrite(BIT_TMP_RECORD_POINT, BIT_TMP_RECORD_POINT_LEN, rbuf);

	return ST_OK;

}
unsigned char get_bit_tmprec_point(unsigned int *haddr)
{
	unsigned char rbuf[16];
// 	unsigned char buf[RECORD_LEN+32];
	unsigned int i, t, j;//,addr=0;
	for(j=0; j<3; j++){
		feread(BIT_TMP_RECORD_POINT, BIT_TMP_RECORD_POINT_LEN, rbuf);
		i = __cpuCrc32(rbuf, BIT_TMP_RECORD_POINT_LEN-4);
		memcpy((unsigned char*)&t, rbuf+4, 4);
		if(i == t){
			MSG_LOG("getTMP_RECORD_POINT OK!\r\n");
			BCD_LOG(rbuf, BIT_TMP_RECORD_POINT_LEN, 1);
			memcpy((unsigned char*)&i, rbuf, 4);
			*haddr = i;
			return ST_OK;
		}
	}


	MSG_LOG("getTMP_RECORD_POINT ERROR!\r\n");
	BCD_LOG(rbuf,BIT_TMP_RECORD_POINT_LEN, 1);

	
	return ST_ERROR;
}
int close_rfile_Wrecord(void)
{
	MSG_LOG("do %s\r\n",__FUNCTION__);
	f_close(&rfile_Wrecord);
	return 0;
}

int open_rfile_Wrecord(void)
{
	FRESULT res;
#ifdef _debug_
	FILINFO finf;
#endif
	//	FIL rfile;
	int length;
	// 		float f1;
	DIR tdir;
	Rec_Information *hisp;
	unsigned char buf[RECORD_LEN+32];
	char fileName[50];
//	unsigned int addr=0,i=0;
	
	//	LPRECORD record;
	
	
	//  	unsigned int ato=0, afree=0;
	
	MSG_LOG("do %s\r\n",__FUNCTION__);
	
	
	
	res = f_opendir(&tdir, "1:/curdat");
	if(res != FR_OK){
		dis_messgebox("��Ŀ¼��", res, 12);
		res = f_mkdir("1:/curdat");
		if(res != FR_OK){
			dis_messgebox("����Ŀ¼��", res, 12);
			return -1;
		}
	}
	
	// 	res = get_flahFatfree(&ato, &afree);
	
	
	memset(fileName, 0, sizeof(fileName));
	// 	if(mode == 1)
	// 		strcpy(fileName, "1:/curdat/cur");	//��ǰ�ļ�
	// 	else
	strcpy(fileName, "1:/curdat/his");	//��ʷ�ļ�
	BCD2Ascii((unsigned char*)&SysTime.month, (unsigned char*)fileName+strlen(fileName), 2);//�ļ���Ϊhis+�·�+���ڡ���5��9��Ϊ��his0509 �ɱ���1��
	MSG_LOG(" open RecordFat FileName%s\r\n", fileName);
	//û�������ļ����Լ�����
	res = f_open(&rfile_Wrecord, fileName, FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
		dis_messgebox("������¼�ļ���", res, 12);
		return -2;
	}
	MSG_LOG("cell data 2\r\n");
	
	if(_Fread(buf,DATALEN,&rfile_Wrecord) < 0){	//����ǰ��14���ֽڣ�����¼�ǲ��ǵ����
		dis_messgebox("����¼�ļ���", res, 12);
		return -2;
	}
	
	//	BCD_LOG(buf, DATALEN, 1);
	
	Ascii2BCD(buf, buf+20, 14);
	if(memcmp(buf+20, (unsigned char*)&SysTime, 2) != 0){//���ļ��ڵ����ݲ��ǽ���
		f_close(&rfile_Wrecord);
		MSG_LOG("�������ļ�:%s\r\n", fileName);
		res = f_open(&rfile_Wrecord, fileName, FA_READ|FA_WRITE|FA_CREATE_ALWAYS);//���ǽ������ļ�
		if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
		{
			dis_messgebox("�����¼�¼�ļ���", res, 12);
			return -2;
		}
		hisp = (Rec_Information *)buf;
		memset(buf, ' ', sizeof(buf));
		BCD2Ascii((unsigned char*)&SysTime, (unsigned char*)hisp->yy, 7);//д�뵱ǰ����ʱ��
		memcpy(hisp->device_no, pFistVary.DeviceNo,7);
		length = 0;
		memcpy((unsigned char*)&length,pFistVary.fileSno,2);//�ļ���
		sprintf((char*)hisp->package_serial_no,"%05u",length);//�ɼ������к�	(ASCII)
		length+=1;
		memcpy(pFistVary.fileSno,(unsigned char*)&length,2);//�ļ����кż�1
		sysfewrite(BIT_GET_FILESNO,2,(unsigned char*)&length);
		
		sprintf(hisp->RecLength, "%05d", DATALEN);//������¼����
		
		_Fwrite(buf, DATALEN, &rfile_Wrecord);

		f_close(&rfile_Wrecord);
		//û�������ļ����Լ�����
		res = f_open(&rfile_Wrecord, fileName, FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
		if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
		{
			dis_messgebox("������¼�ļ���", res, 12);
			return -2;
		}
		MSG_LOG("cell data 3\r\n");

	}

#ifdef _debug_

	res = f_stat(fileName, &finf);
	if(res != FR_OK){
		//f_close(&rfile);
		dis_messgebox("��¼�ļ���Ϣ��", res, 12);
		return -3;
	}
	length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
	MSG_LOG("stat len:%d,%lld\r\n", length, finf.fsize);
	MSG_LOG("Rindex len:%d\r\n", length);

#endif


	return 0;
	
}

int write_rfile_Wrecord(unsigned char *buf)
{

	char fileName[50];

//	FILINFO finf;
// 	FIL rfile;
	int length;
	float f1;


	MSG_LOG("do %s\r\n",__FUNCTION__);


	memset(fileName, 0, sizeof(fileName));
	strcpy(fileName, "1:/curdat/his");	//��ʷ�ļ�
	BCD2Ascii((unsigned char*)&SysTime.month, (unsigned char*)fileName+strlen(fileName), 2);//�ļ���Ϊhis+�·�+���ڡ���5��9��Ϊ��his0509 �ɱ���1��


// 	res = f_stat(fileName, &finf);
// 	if(res != FR_OK){
// 		dis_messgebox("��¼�ļ���Ϣ��", res, 12);
// 		return -3;
// 	}
// 	length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
// 	MSG_LOG("stat len:%d,%lld\r\n", length, finf.fsize);
	//�½� �ļ��Ͳ���Ҫȡ���ȶ�λ�ˡ�û�ر��ļ�ǰ����f_statȡ�����ǲ��Եģ�û�а�����д������ݳ���
	
	f1 = f_size(&rfile_Wrecord);//ȡ�����ĳ��Ȳ��ԡ���������	
 	MSG_LOG("f_size length:%6.0f\r\n", f1);

	
	length = f1;
	MSG_LOG("Rindex len:%d\r\n", length);
	f_lseek(&rfile_Wrecord, length);	//��λ���ļ������
	
	if(_Fwrite(buf, DATALEN, &rfile_Wrecord) != DATALEN){
		close_rfile_Wrecord();
		open_rfile_Wrecord();
//		dis_messgebox("д��¼�ļ���", res, 12);
		return -4;		
	}

	MSG_LOG("w FAT ok--\r\n");

	return 0;
}
//���������ȷ��ÿ���пռ俪������ capacity_M ������ֵ
int check_free_flash(unsigned char  capacity_M)
{
	unsigned char getbuff[32];
	unsigned char hisfilename[12];//1231 ����his ����
	unsigned int i=0;

	unsigned char buf[RECORD_LEN+32];
	char fileName[50];
	FIL rfile;

	unsigned int ato=0, afree=0;
	unsigned char res;
	
	hisfilename[0] = 0x12;
	hisfilename[1] = 0x31;		//

	MSG_LOG("do %s\r\n",__FUNCTION__);
	
//	cls();

//	display(6, 0, "����ļ�...���Ժ�",0);

	
	GetDateTime();
	res = get_flahFatfree(&ato, &afree);
	if ( afree < capacity_M*1024)//
	{
		
		MSG_LOG("<%dM ����\r\n",capacity_M);


			
			//��ȥ�����1.1��ʼ
			for (i=0;i< 365;i++)
			{
				clr_wdt();
				res = get_flahFatfree(&ato, &afree);
				
				if (afree > capacity_M*1024) 
				{
					MSG_LOG("�ռ��㹻%dm\r\n",capacity_M);
					return ST_OK;
				}
				memcpy(getbuff,&SysTime.year_h,2);
				memcpy(getbuff+2,hisfilename,2);
				memcpy(getbuff+4,"\x01\x59\x59",3);
// 				MSG_LOG("time:");
// 				BCD_LOG(getbuff,7,1);
				TimeAdd(getbuff+20,getbuff,86400);	//һ��
				memcpy(hisfilename,getbuff+20+2,2);//ym
// 				MSG_LOG("time:");
// 				BCD_LOG(getbuff+20,7,1);
// 				MSG_LOG("his:%02x%02x\r\n",hisfilename[0],hisfilename[1]);
				
				
				memset(fileName,0,sizeof(fileName));
				strcpy(fileName, "1:/curdat/his");	//��ʷ�ļ�
				BCD2Ascii(hisfilename,(unsigned char *)(fileName+strlen(fileName)),2);
				MSG_LOG("I:%d,path:%s\r\n",i,fileName);
				
				//���ļ�����
				res = f_open(&rfile, fileName, FA_OPEN_EXISTING | FA_READ);
				if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
				{
					continue;
				}
				else
				{
					
					
					if(_Fread(buf,DATALEN,&rfile) < 0){	//����ǰ��14���ֽڣ�����¼�ǲ��ǵ����
						dis_messgebox("����¼�ļ���", res, 12);
						f_close(&rfile);
						return -2;
					}
					
					MSG_LOG("����ʷ����:");
					BCD_LOG(buf, 30, 1);
					
					Ascii2BCD(buf, buf+20, 14);
					if(memcmp(buf+20, (unsigned char*)&SysTime, 2) != 0){//���ļ��ڵ����ݲ��ǽ���
						f_close(&rfile);
						MSG_LOG("ȥ��ɾ��:%s\r\n", fileName);
						f_unlink(fileName);


						
					}
					
					f_close(&rfile);
				}
			}
			
	

			hisfilename[0] = 0x12;
			hisfilename[1] = 0x31;		//


			//�ҽ��� 1.1 ��ʼ
			for (i=0;i< 365;i++)
			{
				clr_wdt();

				res = get_flahFatfree(&ato, &afree);
				
				if (afree > capacity_M*1024) 
				{
					MSG_LOG("�ռ��㹻%dm\r\n",capacity_M);
					return ST_OK;
				}
				memcpy(getbuff,&SysTime.year_h,2);
				memcpy(getbuff+2,hisfilename,2);
				memcpy(getbuff+4,"\x01\x59\x59",3);
				TimeAdd(getbuff+20,getbuff,86400);	//һ��
				memcpy(hisfilename,getbuff+20+2,4);//ym
				
				
				memset(fileName,0,sizeof(fileName));
				strcpy(fileName, "1:/curdat/his");	//��ʷ�ļ�
				BCD2Ascii(hisfilename,(unsigned char *)(fileName+strlen(fileName)),2);
				MSG_LOG("I:%d,path:%s\r\n",i,fileName);
				
				//���ļ�����
				res = f_open(&rfile, fileName, FA_OPEN_EXISTING | FA_READ);
				if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
				{
					continue;
				}
				else
				{
					
					f_close(&rfile);
					MSG_LOG("����ɾ��:%s\r\n", fileName);
					f_unlink(fileName);
						
						
				}
			}	
	}else{
		MSG_LOG("free�����㹻:%dM =%dBK\r\n",afree/1024,afree);
	}





	return ST_OK;



}

//������ʱ����ļ�¼4K ����д��his�ļ��� ,˳��OPEN ���flash ��ָ��
int check_tmp_record(void)
{
	unsigned char buf[RECORD_LEN+32];
	unsigned int addr=0,i=0;

	MSG_LOG("do %s\r\n",__FUNCTION__);


	if (open_rfile_Wrecord() != 0)
	{
		return -1;
	}


	addr = FLASH_TEMP_RECORD;
	for (i=0;i<MASK4K;i+=RECORD_LEN)
	{
		flashread(addr+i,buf,DATALEN);

		MSG_LOG("�鿴��¼:%d\r\n",i);
		BCD_LOG(buf,DATALEN,1);
		memset(buf+RECORD_LEN,0xFF,10);
		if (memcmp(buf,buf+RECORD_LEN, 10 ) == 0)
		{
			break;	//û����
		}

		if (memcmp(buf,"\x02\x41",2 ) != 0)
		{
			continue;
		}

		//�϶��Ǽ�¼��
		MSG_LOG("��д��¼:%d",i);

		// 		f1 = f_size(&rfile_Wrecord);//ȡ�����ĳ��Ȳ��ԡ���������
		// 		MSG_LOG("f_size length:%6.0f\r\n", f1);
		
		write_rfile_Wrecord(buf);

		


		
	}


	close_rfile_Wrecord();
	
	if (open_rfile_Wrecord() != 0)
	{
		return -1;
	}

	addr = 0;
	save_bit_tmprec_point(&addr);
	addr = FLASH_TEMP_RECORD;
	flasherase(ERASE4KBYTESECTOR,addr);


	MSG_LOG("open OK--\r\n");


	return 0;

}


//���ļ���д��¼���ļ���¼λ��0:/curdatĿ¼�¡�д��¼�ļ���ʱ�䣺172ms��������265ms
//ֻ��Ҫд����ʷ�ļ���31���ļ�(�ļ���Ϊhis0101-his1231)���µ�һ�긲�Ǿɵġ���ǰ�ļ�ģʽ����.��һ��д���ļ�����������ʱ�䣬������ǵ�����򸲸ǣ��ǵ�������д��
//buffer:Ҫд��ļ�¼ָ��
//mode:д�뷽ʽ��0=д��ʷ�ļ���1=д��ǰ�ļ�,����curdat�ļ����¡�
int WriteRecordFAT_0(unsigned char *wbuf, unsigned char mode)
{
	FRESULT res;
	FILINFO finf;
	FIL rfile;
	int length;
// 		float f1;
	DIR tdir;
	Rec_Information *hisp;
	unsigned char buf[RECORD_LEN+32];
	char fileName[50];

	set_timer0(2000, 2);
	MSG_LOG("do %s\r\n",__FUNCTION__);

	res = f_opendir(&tdir, "1:/curdat");
	if(res != FR_OK){
		dis_messgebox("��Ŀ¼��", res, 12);
		res = f_mkdir("1:/curdat");
		if(res != FR_OK){
			dis_messgebox("����Ŀ¼��", res, 12);
			return -1;
		}
	}

	memset(fileName, 0, sizeof(fileName));
	if(mode == 1)
		strcpy(fileName, "1:/curdat/cur");	//��ǰ�ļ�
	else
		strcpy(fileName, "1:/curdat/his");	//��ʷ�ļ�
	BCD2Ascii((unsigned char*)&SysTime.month, (unsigned char*)fileName+strlen(fileName), 2);//�ļ���Ϊhis+�·�+���ڡ���5��9��Ϊ��his0509 �ɱ���1��
 	MSG_LOG("Write RecordFat FileName%s\r\n", fileName);
	//û�������ļ����Լ�����
	res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
		dis_messgebox("������¼�ļ���", res, 12);
		return -2;
	}
	MSG_LOG("cell data 2\r\n");

	if(_Fread(buf,DATALEN,&rfile) < 0){	//����ǰ��14���ֽڣ�����¼�ǲ��ǵ����
		dis_messgebox("����¼�ļ���", res, 12);
		return -2;
	}

//	BCD_LOG(buf, DATALEN, 1);
	
	Ascii2BCD(buf, buf+20, 14);
	if(memcmp(buf+20, (unsigned char*)&SysTime, 2) != 0){//���ļ��ڵ����ݲ��ǽ���
		f_close(&rfile);
		MSG_LOG("�������ļ�:%s\r\n", fileName);
		res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_CREATE_ALWAYS);//���ǽ������ļ�
		if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
		{
			dis_messgebox("�����¼�¼�ļ���", res, 12);
			return -2;
		}
		hisp = (Rec_Information *)buf;
		memset(buf, ' ', sizeof(buf));
		BCD2Ascii((unsigned char*)&SysTime, (unsigned char*)hisp->yy, 7);//д�뵱ǰ����ʱ��
		memcpy(hisp->device_no, pFistVary.DeviceNo,7);
		length = 0;
		memcpy((unsigned char*)&length,pFistVary.fileSno,2);//�ļ���
		sprintf((char*)hisp->package_serial_no,"%05u",length);//�ɼ������к�	(ASCII)
		length+=1;
		memcpy(pFistVary.fileSno,(unsigned char*)&length,2);//�ļ����кż�1
		sysfewrite(BIT_GET_FILESNO,2,(unsigned char*)&length);

		sprintf(hisp->RecLength, "%05d", DATALEN);//������¼����
		
		_Fwrite(buf, DATALEN, &rfile);
	}
	else{//�½� �ļ��Ͳ���Ҫȡ���ȶ�λ�ˡ�û�ر��ļ�ǰ����f_statȡ�����ǲ��Եģ�û�а�����д������ݳ���
// 		f1 = f_size(&rfile);//ȡ�����ĳ��Ȳ��ԡ���������
// 		MSG_LOG("f_size length:%6.0f\r\n", f1);

		res = f_stat(fileName, &finf);
		if(res != FR_OK){
			f_close(&rfile);
			dis_messgebox("��¼�ļ���Ϣ��", res, 12);
			return -3;
		}
		length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
		MSG_LOG("stat len:%d,%lld\r\n", length, finf.fsize);
		
		MSG_LOG("Rindex len:%d\r\n", length);
		f_lseek(&rfile, length);	//��λ���ļ������
	}

	MSG_LOG("WFAT:");
	BCD_LOG(wbuf, DATALEN,1);
	if(_Fwrite(wbuf, DATALEN, &rfile) != DATALEN){
		f_close(&rfile);
		dis_messgebox("д��¼�ļ���", res, 12);
		return -4;		
	}


	f_close(&rfile);

	length = get_timer0(2);
	MSG_LOG("write FAT delay:%dms\r\n", (2000-length));//����д��¼ʱ��೤

	return 0;
	
}


//mode:д�뷽ʽ��0=д��ʷ�ļ���1=д��ǰ�ļ�,����curdat�ļ����¡�
int WriteRecordFAT(unsigned char *wbuf, unsigned char mode)
{

//	unsigned char buf[RECORD_LEN+32];
	unsigned int addr=0;//,i=0;
	unsigned int rfile_Wrecord_point=0;//��¼ָ�� ��4K/256=16 ��Ҫ�ر��ļ�������

#ifdef _debug_
	int length=0;
	char fileName[64];
#endif
	addr = FLASH_TEMP_RECORD;

	MSG_LOG("do %s\r\n",__FUNCTION__);
	
	set_timer0(2000, 2);


	if (get_bit_tmprec_point(&rfile_Wrecord_point) == ST_ERROR)
	{
		//��֪��ʲôԭ����ˣ��ر��ļ�����д
		close_rfile_Wrecord();
		check_tmp_record();
		get_bit_tmprec_point(&rfile_Wrecord_point);
	}


	if (rfile_Wrecord_point >= 32)
	{
		MSG_LOG("��:%d\r\n",rfile_Wrecord_point);
		//debugstring("����--\r\n");
		addr = FLASH_TEMP_RECORD;
		flasherase(ERASE4KBYTESECTOR,addr);
		rfile_Wrecord_point = 0;
		save_bit_tmprec_point(&rfile_Wrecord_point);

		//���¹رմ��ļ�
		close_rfile_Wrecord();
		open_rfile_Wrecord();

	}

	MSG_LOG("��дtmp FLASH off:%d\r\n",rfile_Wrecord_point);
	flashwrite(addr+rfile_Wrecord_point*RECORD_LEN,wbuf, RECORD_LEN);
	rfile_Wrecord_point++;
	save_bit_tmprec_point(&rfile_Wrecord_point);

	//��д�ļ�
	write_rfile_Wrecord(wbuf);




#ifdef _debug_
	length = get_timer0(2);
	MSG_LOG("write FAT delay:%dms\r\n", (2000-length));//����д��¼ʱ��೤
	memset(fileName,0,sizeof(fileName));
	sprintf(fileName,"write FAT:%dMS",(2000-length));
	//	debugstring(fileName);
	miniDispstr(0,0,(const char *)fileName,0);
#endif


	return 0;

}


/*
//���ļ���д��¼���ļ���¼λ��0:/curdatĿ¼�¡�д��¼�ļ���ʱ�䣺172ms��������265ms
//ֻ��Ҫд����ʷ�ļ���31���ļ�(�ļ���Ϊhis0101-his1231)���µ�һ�긲�Ǿɵġ���ǰ�ļ�ģʽ����.��һ��д���ļ�����������ʱ�䣬������ǵ�����򸲸ǣ��ǵ�������д��
//buffer:Ҫд��ļ�¼ָ��
//mode:д�뷽ʽ��0=д��ʷ�ļ���1=д��ǰ�ļ�,����curdat�ļ����¡�
int WriteRecordFAT(unsigned char *wbuf, unsigned char mode)
{
	FRESULT res;
	FILINFO finf;
	FIL rfile;
	int length;
// 		float f1;
	DIR tdir;
	Rec_Information *hisp;
	unsigned char buf[RECORD_LEN+32];
	char fileName[50];

//  	unsigned int ato=0, afree=0;

	set_timer0(2000, 2);


	res = f_opendir(&tdir, "1:/curdat");
	if(res != FR_OK){
		dis_messgebox("��Ŀ¼��", res, 12);
		res = f_mkdir("1:/curdat");
		if(res != FR_OK){
			dis_messgebox("����Ŀ¼��", res, 12);
			return -1;
		}
	}

// 	res = get_flahFatfree(&ato, &afree);


	memset(fileName, 0, sizeof(fileName));
	if(mode == 1)
		strcpy(fileName, "1:/curdat/cur");	//��ǰ�ļ�
	else
		strcpy(fileName, "1:/curdat/his");	//��ʷ�ļ�
	BCD2Ascii((unsigned char*)&SysTime.month, (unsigned char*)fileName+strlen(fileName), 2);//�ļ���Ϊhis+�·�+���ڡ���5��9��Ϊ��his0509 �ɱ���1��
 	MSG_LOG("Write RecordFat FileName%s\r\n", fileName);
	//û�������ļ����Լ�����
	res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
		dis_messgebox("������¼�ļ���", res, 12);
		return -2;
	}
	MSG_LOG("cell data 2\r\n");

	if(_Fread(buf,DATALEN,&rfile) < 0){	//����ǰ��14���ֽڣ�����¼�ǲ��ǵ����
		dis_messgebox("����¼�ļ���", res, 12);
		return -2;
	}

//	BCD_LOG(buf, DATALEN, 1);
	
	Ascii2BCD(buf, buf+20, 14);
	if(memcmp(buf+20, (unsigned char*)&SysTime, 2) != 0){//���ļ��ڵ����ݲ��ǽ���
		f_close(&rfile);
		MSG_LOG("�������ļ�:%s\r\n", fileName);
		res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_CREATE_ALWAYS);//���ǽ������ļ�
		if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
		{
			dis_messgebox("�����¼�¼�ļ���", res, 12);
			return -2;
		}
		hisp = (Rec_Information *)buf;
		memset(buf, ' ', sizeof(buf));
		BCD2Ascii((unsigned char*)&SysTime, (unsigned char*)hisp->yy, 7);//д�뵱ǰ����ʱ��
		memcpy(hisp->device_no, pFistVary.DeviceNo,7);
		length = 0;
		memcpy((unsigned char*)&length,pFistVary.fileSno,2);//�ļ���
		sprintf((char*)hisp->package_serial_no,"%05u",length);//�ɼ������к�	(ASCII)
		length+=1;
		memcpy(pFistVary.fileSno,(unsigned char*)&length,2);//�ļ����кż�1
		sysfewrite(BIT_GET_FILESNO,2,(unsigned char*)&length);

		sprintf(hisp->RecLength, "%05d", DATALEN);//������¼����
		
		_Fwrite(buf, DATALEN, &rfile);
	}
	else{//�½� �ļ��Ͳ���Ҫȡ���ȶ�λ�ˡ�û�ر��ļ�ǰ����f_statȡ�����ǲ��Եģ�û�а�����д������ݳ���
// 		f1 = f_size(&rfile_Wrecord);//ȡ�����ĳ��Ȳ��ԡ���������
// 		MSG_LOG("f_size length:%6.0f\r\n", f1);

		res = f_stat(fileName, &finf);
		if(res != FR_OK){
			f_close(&rfile);
			dis_messgebox("��¼�ļ���Ϣ��", res, 12);
			return -3;
		}
		length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
		MSG_LOG("stat len:%d,%lld\r\n", length, finf.fsize);
		
		MSG_LOG("Rindex len:%d\r\n", length);
		f_lseek(&rfile, length);	//��λ���ļ������
	}




	length = get_timer0(2);
	MSG_LOG("write FAT delay1:%dms\r\n", (2000-length));//����д��¼ʱ��೤

	MSG_LOG("WFAT:");
	BCD_LOG(wbuf, DATALEN,1);
	if(_Fwrite(wbuf, DATALEN, &rfile) != DATALEN){
		f_close(&rfile);
		dis_messgebox("д��¼�ļ���", res, 12);
		return -4;		
	}

	length = get_timer0(2);
	MSG_LOG("write FAT delay2:%dms\r\n", (2000-length));//����д��¼ʱ��೤
// #ifdef _debug_
// 	memset(fileName,0,sizeof(fileName));
// 	sprintf(fileName,"write FAT:%dMS",(2000-ato));
	//	debugstring(fileName);
// 	miniDispstr(0,0,(const char *)fileName,0);
// #endif



	f_close(&rfile);



 
	length = get_timer0(2);
	MSG_LOG("write FAT delay:%dms\r\n", (2000-length));//����д��¼ʱ��೤
#ifdef _debug_
	memset(fileName,0,sizeof(fileName));
	sprintf(fileName,"write FAT:%dMS",(2000-length));
//	debugstring(fileName);
	miniDispstr(0,0,(const char *)fileName,0);
#endif


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _debug_
	
	memset(fileName,0,sizeof(fileName));
	strcpy(fileName,"1:/curdat/time.txt");
	//û�������ļ����Լ�����
	res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
		dis_messgebox("������¼�ļ���", res, 12);
		return -2;
	}
	res = f_stat(fileName, &finf);
	if(res != FR_OK){
		f_close(&rfile);
		dis_messgebox("��¼�ļ���Ϣ��", res, 12);
		return -3;
	}
	length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
	MSG_LOG("stat len:%d,%lld\r\n", length, finf.fsize);
	
	MSG_LOG("time.txt len:%d\r\n", length);
	f_lseek(&rfile, length);	//��λ���ļ������

	length = get_timer0(2);

	memset(fileName,0,sizeof(fileName));
	sprintf(fileName,"write FAT:%dMS\r\n",(2000-length));

	if(_Fwrite(fileName, strlen(fileName), &rfile) == -1){
		f_close(&rfile);
	}
	
	f_close(&rfile);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

	return 0;
	
}
*/



//�����û����Ҫ�ɼ��ļ�¼
// =0 ����Ϣ���󣬲�����
// 1 Ҫ������Ϣ
int Check_Send_FAT_Rcord(stFAT_hisRec *hisinfor)
{
	unsigned char getbuff[32];
	stFAT_hisRec hismsg;
	unsigned int scrc32=0;
	
	sysferead(BIT_REORD_HIS,BIT_REORD_HIS_LEN,(unsigned char *)&hismsg);
//	BCD_LOG((unsigned char *)&hismsg,BIT_REORD_HIS_LEN,1);
	



	
	memcpy(getbuff,hismsg.startdate,4);
	memcpy(getbuff+4,"\x01\x59\x59",3);
	if(time_invalid(getbuff) != 1){
		return 0;
	}
	
	memcpy(getbuff,hismsg.enddate,4);
	memcpy(getbuff+4,"\x01\x59\x59",3);
	if(time_invalid(getbuff) != 1){
		return 0;
	}

	memcpy(getbuff,hismsg.sfilename,4);
	memcpy(getbuff+4,"\x01\x59\x59",3);
	if(time_invalid(getbuff) != 1){
		return 0;
	}

	
	scrc32 = __cpuCrc32((unsigned char *)&hismsg, BIT_REORD_HIS_LEN-4);
	
	if (scrc32 != hismsg.scrc32)
	{
		return 0;
	}

	memcpy(hisinfor,&hismsg,BIT_REORD_HIS_LEN);
	
	return 1;
}
//�ҵ�ĳ���ļ���д������Ϣ
//1 �м�¼�ļ��� �������ļ��ܳ�
// 0 ��
int Check_hisfile(stFAT_hisRec *hisinfor)
{
	
	FRESULT res;
	FILINFO finf;
	FIL rfile;
	int length;
	int i=0;//,ret;
	unsigned char getbuff[32];
	stFAT_hisRec hismsg;
	char fileName[50];
	
	while(1) 
	{
		
		i = Check_Send_FAT_Rcord(&hismsg);
		//MSG_LOG("i=%d\r\n",i);
		if (i ==0)
		{
			return 0;//û�ҵ���̨Ҫ�ɼ�����Ϣ
		}
		
		memset(fileName,0,sizeof(fileName));
		strcpy(fileName, "1:/curdat/his");	//��ʷ�ļ�
		BCD2Ascii(hismsg.sfilename+2,(unsigned char *)(fileName+strlen(fileName)),2);
		MSG_LOG("path:%s\r\n",fileName);
		
		//���ļ�����
		res = f_open(&rfile, fileName, FA_OPEN_EXISTING | FA_READ);
		if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
		{
			SaveFAT_hisRecInfor(0);		//	�޳�һ���ļ����Ҳ������޳� Ȼ������
			
		}else{		
			break;		//�и��ļ��ɹ���
		}
		
		if (memcmp(hismsg.sfilename,hismsg.enddate,4) == 0)//û���ļ����˳���
		{
			return 0;
		}
		
	} 
	
	
	//�ɹ��򿪣�д��Ϣ
	
	res = f_stat(fileName, &finf);
	if(res != FR_OK){
		f_close(&rfile);
		return 0;
	}

	length = hismsg.offset;
	if (hismsg.offset == 0)
	{
		MSG_LOG("����ͷ128--\r\n");
		
		length += DATALEN;	//
		
	}

	MSG_LOG("���ļ�:%s\r\n",fileName);
	MSG_LOG("off:%d,flen:%lld\r\n", length, finf.fsize);
	
	f_lseek(&rfile, length);	//��λ���ļ������
	if(_Fread(getbuff, 8, &rfile) != 8){
		f_close(&rfile);
		return 0;		
	}
	if(memcmp(getbuff, "��������", 8) == 0)//
	{
		debugstring("�ü�¼Ϊ���п���¼\r\n");
		Qboboc_flag = 1;//�������м�¼����ͨ��¼
	}
	else
	{
		Qboboc_flag=0;
		debugstring("�ü�¼Ϊ��ͨ����¼\r\n");
	}

// 	length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
// 	MSG_LOG("find len:%d\r\n", length);
	
	hismsg.filelen = finf.fsize;
	
	hismsg.scrc32  = __cpuCrc32((unsigned char *)&hismsg, BIT_REORD_HIS_LEN-4);
	sysfewrite(BIT_REORD_HIS,BIT_REORD_HIS_LEN,(unsigned char *)&hismsg);
	
	
	memcpy(hisinfor,&hismsg,BIT_REORD_HIS_LEN);
	
	
    /*close file */  
    f_close(&rfile);  
	
	
	return 1;
	
	
	
}

//����һ���Ժ���ʷ��¼��ϢҪ���ӣ����޳��ѷ�������ļ� ֱ�������� ��������ʱ��д�ɷǷ�
//0  ����ʼ�ļ�
//1  ��ƫ����
int SaveFAT_hisRecInfor(unsigned char mode)
{
// 	unsigned int startdata=0,enddata=0;
// 	unsigned short ustemp=0,ustemp2=0;
	
	unsigned char getbuff[32];
	stFAT_hisRec hismsg;
//	unsigned int scrc32=0;
	
	sysferead(BIT_REORD_HIS,BIT_REORD_HIS_LEN,(unsigned char *)&hismsg);
//	BCD_LOG((unsigned char *)&hismsg,BIT_REORD_HIS_LEN,1);

	switch(mode)
	{
	case 0:
 		MSG_LOG("remove one file---\r\n");
		memcpy(getbuff+20,hismsg.sfilename,4);
		memcpy(getbuff+20+4,"\x01\x59\x59",3);
		TimeAdd(getbuff,getbuff+20,86400);	//һ��
		memcpy(hismsg.sfilename,getbuff,4);
		hismsg.offset = 0;
		hismsg.filelen = 0;


		if (memcmp(hismsg.sfilename,hismsg.enddate,4) == 0)
		{
			MSG_LOG("collect END2--\r\n");
			memset(&hismsg.sfilename,0,4);		//�ƳɷǷ�
			sysfewrite(BIT_REORD_HIS,BIT_REORD_HIS_LEN,(unsigned char *)&hismsg);
			return 0;	//û�м�¼��
		}
		break;
	case 1:

// 		MSG_LOG("off add---\r\n");
// 		MSG_LOG("off:%d,allen:%d\r\n",hismsg.offset,hismsg.filelen);
		if(Qboboc_flag==1)
			hismsg.offset += 256;
		else
		hismsg.offset += DATALEN;
		  Qboboc_flag=0;//�����п���¼��־λ

		if (hismsg.offset >= hismsg.filelen)
		{
			
			MSG_LOG("remove one file-2--\r\n");

			memcpy(getbuff+20,hismsg.sfilename,4);
			memcpy(getbuff+20+4,"\x01\x59\x59",3);
			TimeAdd(getbuff,getbuff+20,86400);	//һ��
			memcpy(hismsg.sfilename,getbuff,4);

			hismsg.offset = 0;
			hismsg.filelen = 0;

			if (memcmp(hismsg.sfilename,hismsg.enddate,4) == 0)
			{
				MSG_LOG("collect END--\r\n");
				memset(&hismsg.sfilename,0,4);		//�ƳɷǷ�
				sysfewrite(BIT_REORD_HIS,BIT_REORD_HIS_LEN,(unsigned char *)&hismsg);
				return 0;	//û�м�¼�� 
			}

		}

		break;
	default:
		return -1;
	}


	
#ifdef _debug_
	MSG_LOG("hisdata:");
	BCD_LOG(hismsg.startdate,8,1);
	BCD_LOG(hismsg.sfilename,4,1);
	MSG_LOG("off:%d,flen:%d\r\n",hismsg.offset,hismsg.filelen);
#endif

	hismsg.scrc32 = __cpuCrc32((unsigned char *)&hismsg, BIT_REORD_HIS_LEN-4);
	
	sysfewrite(BIT_REORD_HIS,BIT_REORD_HIS_LEN,(unsigned char *)&hismsg);



	return 0;
	
}


//�����û����Ҫ���ɼ��ļ�¼
// <0  �д���
// == 0 �޼�¼
// 1 ��Ҫ���͵ļ�¼��������Ϣ
int Read_oneHis(unsigned char *recrord)
{
	FRESULT res;
	FILINFO finf;
	FIL rfile;
	int length;
	int i=0;//,ret;
//	unsigned char getbuff[32];
	unsigned char disbuf[256];
	stFAT_hisRec hismsg;
	char fileName[50];


	i = Check_Send_FAT_Rcord(&hismsg);
	if (i != 1 )
	{
		return 0;//û�ҵ���̨Ҫ�ɼ�����Ϣ		
	}

	MSG_LOG("find1--\r\n");


	if(Check_hisfile(&hismsg)  != 1)
	{
		return 0;	//û�ҵ��б�
	}

	MSG_LOG("find2--\r\n");



	memset(fileName,0,sizeof(fileName));
	strcpy(fileName, "1:/curdat/his");	//��ʷ�ļ�
	BCD2Ascii(hismsg.sfilename+2,(unsigned char *)(fileName+strlen(fileName)),2);





//	MSG_LOG("path:%s\r\n",fileName);
	//���ļ�����
	res = f_open(&rfile, fileName, FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
		return 0;
	}

	res = f_stat(fileName, &finf);
	if(res != FR_OK){
		f_close(&rfile);
		return -2;
	}
	if (hismsg.offset == 0)
	{
		MSG_LOG("����ͷ128--\r\n");
		
		hismsg.offset += DATALEN;	//

	}

	if (hismsg.offset >= hismsg.filelen)
	{
		return 0;	// �����Ǹ��ռ�¼ �����ϲ����ܣ�����
	}
	

	length = hismsg.offset;
	MSG_LOG("���ļ�:%s\r\n",fileName);
	MSG_LOG("off:%d,flen:%lld\r\n", length, finf.fsize);

	f_lseek(&rfile, length);	//��λ���ļ������
	if(_Fread(disbuf, DATALEN, &rfile) != DATALEN){
	
		f_close(&rfile);
		return 0;		
	}
	
	
	if(Qboboc_flag==1)
	{
		memcpy(recrord,disbuf+8,120);
		length+=DATALEN;
		f_lseek(&rfile, length);	//��λ���ļ������
		if(_Fread(recrord+120, 21, &rfile) != 21){
			f_close(&rfile);
			return 0;		
		}
		debugstring("���п����ݣ�");
		debugdata(recrord,141,1);
	}
	else
		memcpy(recrord,disbuf,DATALEN);
	MSG_LOG("Rec FAT:");
	BCD_LOG(recrord,DATALEN,1);
	
	f_close(&rfile);
	
	return 1;
}






/*
int add_download_file(char *fileName, unsigned char *dfiledata, unsigned int datlen)
{
	FRESULT res;
	FILINFO finf;
	FIL rfile;
	int length;
	float f1;
	DIR tdir;
	Rec_Information *hisp;
	unsigned char buf[RECORD_LEN+32];
	//char fileName[50];

	res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
		dis_messgebox("������¼�ļ���", res, 12);
		return -2;
	}

	//�½� �ļ��Ͳ���Ҫȡ���ȶ�λ�ˡ�û�ر��ļ�ǰ����f_statȡ�����ǲ��Եģ�û�а�����д������ݳ���
	f1 = f_size(&rfile);//ȡ�����ĳ��Ȳ��ԡ���������
	MSG_LOG("f_size length:%6.0f\r\n", f1);
	
	res = f_stat(fileName, &finf);
	if(res != FR_OK){
		f_close(&rfile);
		dis_messgebox("��¼�ļ���Ϣ��", res, 12);
		return -3;
	}

	length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
	MSG_LOG("stat len:%d,%lld\r\n", length, finf.fsize);
	
	MSG_LOG("Rindex len:%d\r\n", length);
	MSG_LOG("д�ļ�:--\r\n");
	f_lseek(&rfile, length);	//��λ���ļ������
	if(_Fwrite(dfiledata, datlen, &rfile) != datlen){
		f_close(&rfile);
		dis_messgebox("д��¼�ļ���", res, 12);
		return -4;		
	}


	f_close(&rfile);

	return 0;

}


int Great_download_file(char *fileNameTmp)
{

	FRESULT res;
//	FILINFO finf;
	FIL rfile;
//	int length;
	float f1;
	DIR tdir;
//	Rec_Information *hisp;
//	unsigned char buf[RECORD_LEN+32];
	char fileName[50];
	
//	set_timer0(2000, 2);
	
	res = f_opendir(&tdir, "1:/download");
	if(res != FR_OK){
		dis_messgebox("��Ŀ¼��", res, 12);
		res = f_mkdir("1:/download");
		if(res != FR_OK){
			dis_messgebox("����Ŀ¼��", res, 12);
			return -1;
		}
	}
	
	memset(fileName, 0, sizeof(fileName));

	strcpy(fileName, "1:/download/");	//��ʷ�ļ�
	strcat(fileName  ( char *)fileNameTmp);//�ļ���
	MSG_LOG("Write RecordFat FileName%s\r\n", fileName);

	MSG_LOG("���ǽ������ļ�:%s\r\n", fileName);
	res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_CREATE_ALWAYS);//���ǽ������ļ�
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
		dis_messgebox("�����¼�¼�ļ���", res, 12);
		return -2;
	}

	f_close(&rfile);
	
	
	return 0;
	
}
*/



int cp_file(char *old_path,char *new_path)
{
	FRESULT res;
	FIL rfile;
//	char filename[20];
	unsigned char buff[1025];
// 	unsigned char buf1[50];
	FILINFO finf;
	FIL rf1;
	unsigned int length, nowTime, count;//;, i;
//	char *p1, *p2;
	



	//�������

	res=f_open(&rf1, old_path, FA_READ);
	if(res!=FR_OK)
	{
		return -4;
	}
	
	res = f_stat((char*)old_path, &finf);
	if(res != FR_OK){
		return -1;
	}
	length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
	MSG_LOG("file len:%d,%lld\r\n", length, finf.fsize);


	MSG_LOG("�������ļ�:%s\r\n", new_path);
	res = f_open(&rfile, new_path, FA_READ|FA_WRITE|FA_CREATE_ALWAYS);//���ǽ������ļ�
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
// 		dis_messgebox("�����¼�¼�ļ���", res, 12);
		return -2;
	}


	f_lseek(&rf1,0);
	count = length;

	while(1)
	{
		clr_wdt();
		

		if(count > 512){
			res=f_read(&rf1,buff,512,&nowTime);
			if(res!=FR_OK)
			{
				dis_messgebox("�����ļ���ȡ����", length, 12);
				f_close(&rf1);
				return -4;
			}

//			debugdata(buff, nowTime, 1);
// 			flashwrite(addr,buff,512);

			if(_Fwrite(buff,512,&rfile)!= 512)
			{
				f_close(&rfile);
				return -3;
			}

			count -= 512;
// 			addr += 512;
		}
		else{
			res=f_read(&rf1,buff,count,&nowTime);
			if(res!=FR_OK)
			{
// 				dis_messgebox("�����ļ���ȡ����", length, 12);
				f_close(&rf1);
				return -4;
			}
			
			//			debugdata(buff, nowTime, 1);
//			flashwrite(addr,buff,count);
			
			if(_Fwrite(buff,count,&rfile)!= count)
			{
				f_close(&rfile);
				return -3;
			}


			count = 0;
// 			addr += count;
		}

// 		sprintf((char*)buf1, "%d%%", ((length-count)*100)/length);
// 		display(8, 0, (char*)buf1, DIS_CENTER);

		if(count == 0)
		{
			break;
		}	
	}



	res = f_stat((char*)old_path, &finf);
	if(res != FR_OK){
		return -1;
	}
	length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
	MSG_LOG("new file len:%d,%lld\r\n", length, finf.fsize);

	
	f_close(&rf1);
	f_close(&rfile);



	return 0;

}

#endif



