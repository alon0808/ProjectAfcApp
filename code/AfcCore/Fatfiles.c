
//USB功能函数
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



//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量	 （单位KB）
//free:剩余容量	 （单位KB）
//返回值:0,正常.其他,错误代码
// u8 exf_getfree(u8 *drv,u32 *total,u32 *free)
// {
// 	FATFS *fs1;
// 	u8 res;
//     u32 fre_clust=0, fre_sect=0, tot_sect=0;
//     //得到磁盘信息及空闲簇数量
//     res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
//     if(res==0)
// 	{											   
// 		tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
// 		fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	   
// #if _MAX_SS!=512				  				//扇区大小不是512字节,则转换为512字节
// 		tot_sect*=fs1->ssize/512;
// 		fre_sect*=fs1->ssize/512;
// #endif	  
// 		*total=tot_sect>>1;	//单位为KB
// 		*free=fre_sect>>1;	//单位为KB 
// 	}
// 	return res;
// }		   

//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量	 （单位KB）
//free:剩余容量	 （单位KB）
//返回值:0,正常.其他,错误代码
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
	// 		printf("\r\n挂载文件系统失败,错误代码: %u",res);
	// 		return;
	// 	}	
	display(12, 0, "正在格式化磁盘,请稍候...", 0);
	
	res = f_mkfs("1:", 1, 512);	//格式化

	if(res != FR_OK){
		sprintf((char*)irda_rxBuf, "格式化失败,错误代码: %u",res);
		display(14, 0, (char*)irda_rxBuf, 0);
		return res;
	}
	strcpy(path, "curdat");
	res = f_mkdir(path);	//建curdat目录
	if(res != FR_OK){
		sprintf((char*)irda_rxBuf, "建curdat错误,代码: %u",res);
		display(14, 0, (char*)irda_rxBuf, 0);
		return res;
	}
	display(14, 0, "格式化成功...", DIS_CENTER);
	return FR_OK;
	//	f_mount(0,NULL);
}


//开机执行一次
void checkFlashDisFAT(void)
{
	unsigned int ato, afree;
	unsigned char res;

	res = mf_mount("1:", 0);//0,&gfatfs);
	if (res != FR_OK)
	{
		printf("\r\n挂载文件系统失败,错误代码: %u",res);
		return;
	}
	
	res = get_flahFatfree(&ato, &afree);
	if ((res == FR_NO_FILESYSTEM)||(res == FR_INVALID_PARAMETER)){//没有文件系统，需要格式化
		res = get_flahFatfree(&ato, &afree);
		if((res == FR_NO_FILESYSTEM)||(res == FR_INVALID_PARAMETER)){
	// 		display(0,0,"没有文件系统", DIS_CENTER);
	// 		display(3, 0, "需要格式化,请确认!", DIS_CENTER);
// 			while(uck != KEY_ENT){
// 				uck = keywait(KEY_ENT, 2);
// 				if(uck != KEY_ENT){
// 					display(8, 0, "无文件系统，不能运行程序。", 0);
// 				}
// 			}

			res = format_disk_RW();
			if (res == FR_OK)
			{
				display(3, 0, "    格式化完成!    ", DIS_CENTER);
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
//unsigned int rfile_Wrecord_point=0;//记录指针 满4K/256=16 则要关闭文件并擦除

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
		dis_messgebox("打开目录错", res, 12);
		res = f_mkdir("1:/curdat");
		if(res != FR_OK){
			dis_messgebox("建立目录错", res, 12);
			return -1;
		}
	}
	
	// 	res = get_flahFatfree(&ato, &afree);
	
	
	memset(fileName, 0, sizeof(fileName));
	// 	if(mode == 1)
	// 		strcpy(fileName, "1:/curdat/cur");	//当前文件
	// 	else
	strcpy(fileName, "1:/curdat/his");	//历史文件
	BCD2Ascii((unsigned char*)&SysTime.month, (unsigned char*)fileName+strlen(fileName), 2);//文件名为his+月份+日期。如5月9日为：his0509 可保存1年
	MSG_LOG(" open RecordFat FileName%s\r\n", fileName);
	//没有索引文件，自己建立
	res = f_open(&rfile_Wrecord, fileName, FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
		dis_messgebox("创建记录文件错", res, 12);
		return -2;
	}
	MSG_LOG("cell data 2\r\n");
	
	if(_Fread(buf,DATALEN,&rfile_Wrecord) < 0){	//读出前面14个字节，看记录是不是当天的
		dis_messgebox("读记录文件错", res, 12);
		return -2;
	}
	
	//	BCD_LOG(buf, DATALEN, 1);
	
	Ascii2BCD(buf, buf+20, 14);
	if(memcmp(buf+20, (unsigned char*)&SysTime, 2) != 0){//此文件内的数据不是今年
		f_close(&rfile_Wrecord);
		MSG_LOG("建立新文件:%s\r\n", fileName);
		res = f_open(&rfile_Wrecord, fileName, FA_READ|FA_WRITE|FA_CREATE_ALWAYS);//覆盖建立新文件
		if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
		{
			dis_messgebox("创建新记录文件错", res, 12);
			return -2;
		}
		hisp = (Rec_Information *)buf;
		memset(buf, ' ', sizeof(buf));
		BCD2Ascii((unsigned char*)&SysTime, (unsigned char*)hisp->yy, 7);//写入当前日期时间
		memcpy(hisp->device_no, pFistVary.DeviceNo,7);
		length = 0;
		memcpy((unsigned char*)&length,pFistVary.fileSno,2);//文件号
		sprintf((char*)hisp->package_serial_no,"%05u",length);//采集包序列号	(ASCII)
		length+=1;
		memcpy(pFistVary.fileSno,(unsigned char*)&length,2);//文件序列号加1
		sysfewrite(BIT_GET_FILESNO,2,(unsigned char*)&length);
		
		sprintf(hisp->RecLength, "%05d", DATALEN);//单条记录长度
		
		_Fwrite(buf, DATALEN, &rfile_Wrecord);

		f_close(&rfile_Wrecord);
		//没有索引文件，自己建立
		res = f_open(&rfile_Wrecord, fileName, FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
		if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
		{
			dis_messgebox("创建记录文件错", res, 12);
			return -2;
		}
		MSG_LOG("cell data 3\r\n");

	}

#ifdef _debug_

	res = f_stat(fileName, &finf);
	if(res != FR_OK){
		//f_close(&rfile);
		dis_messgebox("记录文件信息错", res, 12);
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
	strcpy(fileName, "1:/curdat/his");	//历史文件
	BCD2Ascii((unsigned char*)&SysTime.month, (unsigned char*)fileName+strlen(fileName), 2);//文件名为his+月份+日期。如5月9日为：his0509 可保存1年


// 	res = f_stat(fileName, &finf);
// 	if(res != FR_OK){
// 		dis_messgebox("记录文件信息错", res, 12);
// 		return -3;
// 	}
// 	length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
// 	MSG_LOG("stat len:%d,%lld\r\n", length, finf.fsize);
	//新建 文件就不需要取长度定位了。没关闭文件前，用f_stat取出来是不对的，没有包含刚写入的数据长度
	
	f1 = f_size(&rfile_Wrecord);//取出来的长度不对。。。。。	
 	MSG_LOG("f_size length:%6.0f\r\n", f1);

	
	length = f1;
	MSG_LOG("Rindex len:%d\r\n", length);
	f_lseek(&rfile_Wrecord, length);	//定位到文件最后面
	
	if(_Fwrite(buf, DATALEN, &rfile_Wrecord) != DATALEN){
		close_rfile_Wrecord();
		open_rfile_Wrecord();
//		dis_messgebox("写记录文件错", res, 12);
		return -4;		
	}

	MSG_LOG("w FAT ok--\r\n");

	return 0;
}
//检查容量，确保每天有空间开机调用 capacity_M 容量阀值
int check_free_flash(unsigned char  capacity_M)
{
	unsigned char getbuff[32];
	unsigned char hisfilename[12];//1231 不含his 月日
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

//	display(6, 0, "清除文件...请稍候",0);

	
	GetDateTime();
	res = get_flahFatfree(&ato, &afree);
	if ( afree < capacity_M*1024)//
	{
		
		MSG_LOG("<%dM 容量\r\n",capacity_M);


			
			//找去年的先1.1开始
			for (i=0;i< 365;i++)
			{
				clr_wdt();
				res = get_flahFatfree(&ato, &afree);
				
				if (afree > capacity_M*1024) 
				{
					MSG_LOG("空间足够%dm\r\n",capacity_M);
					return ST_OK;
				}
				memcpy(getbuff,&SysTime.year_h,2);
				memcpy(getbuff+2,hisfilename,2);
				memcpy(getbuff+4,"\x01\x59\x59",3);
// 				MSG_LOG("time:");
// 				BCD_LOG(getbuff,7,1);
				TimeAdd(getbuff+20,getbuff,86400);	//一天
				memcpy(hisfilename,getbuff+20+2,2);//ym
// 				MSG_LOG("time:");
// 				BCD_LOG(getbuff+20,7,1);
// 				MSG_LOG("his:%02x%02x\r\n",hisfilename[0],hisfilename[1]);
				
				
				memset(fileName,0,sizeof(fileName));
				strcpy(fileName, "1:/curdat/his");	//历史文件
				BCD2Ascii(hisfilename,(unsigned char *)(fileName+strlen(fileName)),2);
				MSG_LOG("I:%d,path:%s\r\n",i,fileName);
				
				//读文件测试
				res = f_open(&rfile, fileName, FA_OPEN_EXISTING | FA_READ);
				if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
				{
					continue;
				}
				else
				{
					
					
					if(_Fread(buf,DATALEN,&rfile) < 0){	//读出前面14个字节，看记录是不是当天的
						dis_messgebox("读记录文件错", res, 12);
						f_close(&rfile);
						return -2;
					}
					
					MSG_LOG("读历史索引:");
					BCD_LOG(buf, 30, 1);
					
					Ascii2BCD(buf, buf+20, 14);
					if(memcmp(buf+20, (unsigned char*)&SysTime, 2) != 0){//此文件内的数据不是今年
						f_close(&rfile);
						MSG_LOG("去年删除:%s\r\n", fileName);
						f_unlink(fileName);


						
					}
					
					f_close(&rfile);
				}
			}
			
	

			hisfilename[0] = 0x12;
			hisfilename[1] = 0x31;		//


			//找今年 1.1 开始
			for (i=0;i< 365;i++)
			{
				clr_wdt();

				res = get_flahFatfree(&ato, &afree);
				
				if (afree > capacity_M*1024) 
				{
					MSG_LOG("空间足够%dm\r\n",capacity_M);
					return ST_OK;
				}
				memcpy(getbuff,&SysTime.year_h,2);
				memcpy(getbuff+2,hisfilename,2);
				memcpy(getbuff+4,"\x01\x59\x59",3);
				TimeAdd(getbuff+20,getbuff,86400);	//一天
				memcpy(hisfilename,getbuff+20+2,4);//ym
				
				
				memset(fileName,0,sizeof(fileName));
				strcpy(fileName, "1:/curdat/his");	//历史文件
				BCD2Ascii(hisfilename,(unsigned char *)(fileName+strlen(fileName)),2);
				MSG_LOG("I:%d,path:%s\r\n",i,fileName);
				
				//读文件测试
				res = f_open(&rfile, fileName, FA_OPEN_EXISTING | FA_READ);
				if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
				{
					continue;
				}
				else
				{
					
					f_close(&rfile);
					MSG_LOG("今年删除:%s\r\n", fileName);
					f_unlink(fileName);
						
						
				}
			}	
	}else{
		MSG_LOG("free容量足够:%dM =%dBK\r\n",afree/1024,afree);
	}





	return ST_OK;



}

//查找临时区域的记录4K 并回写在his文件里 ,顺便OPEN 清空flash 和指针
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

		MSG_LOG("查看记录:%d\r\n",i);
		BCD_LOG(buf,DATALEN,1);
		memset(buf+RECORD_LEN,0xFF,10);
		if (memcmp(buf,buf+RECORD_LEN, 10 ) == 0)
		{
			break;	//没有了
		}

		if (memcmp(buf,"\x02\x41",2 ) != 0)
		{
			continue;
		}

		//肯定是记录了
		MSG_LOG("回写记录:%d",i);

		// 		f1 = f_size(&rfile_Wrecord);//取出来的长度不对。。。。。
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


//向文件内写记录，文件记录位于0:/curdat目录下。写记录文件的时间：172ms，跨扇区265ms
//只需要写入历史文件，31个文件(文件名为his0101-his1231)，新的一年覆盖旧的。当前文件模式不变.第一行写入文件建立的日期时间，如果不是当天的则覆盖，是当天的向后写。
//buffer:要写入的记录指针
//mode:写入方式，0=写历史文件；1=写当前文件,都在curdat文件夹下。
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
		dis_messgebox("打开目录错", res, 12);
		res = f_mkdir("1:/curdat");
		if(res != FR_OK){
			dis_messgebox("建立目录错", res, 12);
			return -1;
		}
	}

	memset(fileName, 0, sizeof(fileName));
	if(mode == 1)
		strcpy(fileName, "1:/curdat/cur");	//当前文件
	else
		strcpy(fileName, "1:/curdat/his");	//历史文件
	BCD2Ascii((unsigned char*)&SysTime.month, (unsigned char*)fileName+strlen(fileName), 2);//文件名为his+月份+日期。如5月9日为：his0509 可保存1年
 	MSG_LOG("Write RecordFat FileName%s\r\n", fileName);
	//没有索引文件，自己建立
	res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
		dis_messgebox("创建记录文件错", res, 12);
		return -2;
	}
	MSG_LOG("cell data 2\r\n");

	if(_Fread(buf,DATALEN,&rfile) < 0){	//读出前面14个字节，看记录是不是当天的
		dis_messgebox("读记录文件错", res, 12);
		return -2;
	}

//	BCD_LOG(buf, DATALEN, 1);
	
	Ascii2BCD(buf, buf+20, 14);
	if(memcmp(buf+20, (unsigned char*)&SysTime, 2) != 0){//此文件内的数据不是今年
		f_close(&rfile);
		MSG_LOG("建立新文件:%s\r\n", fileName);
		res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_CREATE_ALWAYS);//覆盖建立新文件
		if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
		{
			dis_messgebox("创建新记录文件错", res, 12);
			return -2;
		}
		hisp = (Rec_Information *)buf;
		memset(buf, ' ', sizeof(buf));
		BCD2Ascii((unsigned char*)&SysTime, (unsigned char*)hisp->yy, 7);//写入当前日期时间
		memcpy(hisp->device_no, pFistVary.DeviceNo,7);
		length = 0;
		memcpy((unsigned char*)&length,pFistVary.fileSno,2);//文件号
		sprintf((char*)hisp->package_serial_no,"%05u",length);//采集包序列号	(ASCII)
		length+=1;
		memcpy(pFistVary.fileSno,(unsigned char*)&length,2);//文件序列号加1
		sysfewrite(BIT_GET_FILESNO,2,(unsigned char*)&length);

		sprintf(hisp->RecLength, "%05d", DATALEN);//单条记录长度
		
		_Fwrite(buf, DATALEN, &rfile);
	}
	else{//新建 文件就不需要取长度定位了。没关闭文件前，用f_stat取出来是不对的，没有包含刚写入的数据长度
// 		f1 = f_size(&rfile);//取出来的长度不对。。。。。
// 		MSG_LOG("f_size length:%6.0f\r\n", f1);

		res = f_stat(fileName, &finf);
		if(res != FR_OK){
			f_close(&rfile);
			dis_messgebox("记录文件信息错", res, 12);
			return -3;
		}
		length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
		MSG_LOG("stat len:%d,%lld\r\n", length, finf.fsize);
		
		MSG_LOG("Rindex len:%d\r\n", length);
		f_lseek(&rfile, length);	//定位到文件最后面
	}

	MSG_LOG("WFAT:");
	BCD_LOG(wbuf, DATALEN,1);
	if(_Fwrite(wbuf, DATALEN, &rfile) != DATALEN){
		f_close(&rfile);
		dis_messgebox("写记录文件错", res, 12);
		return -4;		
	}


	f_close(&rfile);

	length = get_timer0(2);
	MSG_LOG("write FAT delay:%dms\r\n", (2000-length));//测试写记录时间多长

	return 0;
	
}


//mode:写入方式，0=写历史文件；1=写当前文件,都在curdat文件夹下。
int WriteRecordFAT(unsigned char *wbuf, unsigned char mode)
{

//	unsigned char buf[RECORD_LEN+32];
	unsigned int addr=0;//,i=0;
	unsigned int rfile_Wrecord_point=0;//记录指针 满4K/256=16 则要关闭文件并擦除

#ifdef _debug_
	int length=0;
	char fileName[64];
#endif
	addr = FLASH_TEMP_RECORD;

	MSG_LOG("do %s\r\n",__FUNCTION__);
	
	set_timer0(2000, 2);


	if (get_bit_tmprec_point(&rfile_Wrecord_point) == ST_ERROR)
	{
		//不知道什么原因错了，关闭文件，回写
		close_rfile_Wrecord();
		check_tmp_record();
		get_bit_tmprec_point(&rfile_Wrecord_point);
	}


	if (rfile_Wrecord_point >= 32)
	{
		MSG_LOG("满:%d\r\n",rfile_Wrecord_point);
		//debugstring("满擦--\r\n");
		addr = FLASH_TEMP_RECORD;
		flasherase(ERASE4KBYTESECTOR,addr);
		rfile_Wrecord_point = 0;
		save_bit_tmprec_point(&rfile_Wrecord_point);

		//重新关闭打开文件
		close_rfile_Wrecord();
		open_rfile_Wrecord();

	}

	MSG_LOG("先写tmp FLASH off:%d\r\n",rfile_Wrecord_point);
	flashwrite(addr+rfile_Wrecord_point*RECORD_LEN,wbuf, RECORD_LEN);
	rfile_Wrecord_point++;
	save_bit_tmprec_point(&rfile_Wrecord_point);

	//再写文件
	write_rfile_Wrecord(wbuf);




#ifdef _debug_
	length = get_timer0(2);
	MSG_LOG("write FAT delay:%dms\r\n", (2000-length));//测试写记录时间多长
	memset(fileName,0,sizeof(fileName));
	sprintf(fileName,"write FAT:%dMS",(2000-length));
	//	debugstring(fileName);
	miniDispstr(0,0,(const char *)fileName,0);
#endif


	return 0;

}


/*
//向文件内写记录，文件记录位于0:/curdat目录下。写记录文件的时间：172ms，跨扇区265ms
//只需要写入历史文件，31个文件(文件名为his0101-his1231)，新的一年覆盖旧的。当前文件模式不变.第一行写入文件建立的日期时间，如果不是当天的则覆盖，是当天的向后写。
//buffer:要写入的记录指针
//mode:写入方式，0=写历史文件；1=写当前文件,都在curdat文件夹下。
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
		dis_messgebox("打开目录错", res, 12);
		res = f_mkdir("1:/curdat");
		if(res != FR_OK){
			dis_messgebox("建立目录错", res, 12);
			return -1;
		}
	}

// 	res = get_flahFatfree(&ato, &afree);


	memset(fileName, 0, sizeof(fileName));
	if(mode == 1)
		strcpy(fileName, "1:/curdat/cur");	//当前文件
	else
		strcpy(fileName, "1:/curdat/his");	//历史文件
	BCD2Ascii((unsigned char*)&SysTime.month, (unsigned char*)fileName+strlen(fileName), 2);//文件名为his+月份+日期。如5月9日为：his0509 可保存1年
 	MSG_LOG("Write RecordFat FileName%s\r\n", fileName);
	//没有索引文件，自己建立
	res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
		dis_messgebox("创建记录文件错", res, 12);
		return -2;
	}
	MSG_LOG("cell data 2\r\n");

	if(_Fread(buf,DATALEN,&rfile) < 0){	//读出前面14个字节，看记录是不是当天的
		dis_messgebox("读记录文件错", res, 12);
		return -2;
	}

//	BCD_LOG(buf, DATALEN, 1);
	
	Ascii2BCD(buf, buf+20, 14);
	if(memcmp(buf+20, (unsigned char*)&SysTime, 2) != 0){//此文件内的数据不是今年
		f_close(&rfile);
		MSG_LOG("建立新文件:%s\r\n", fileName);
		res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_CREATE_ALWAYS);//覆盖建立新文件
		if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
		{
			dis_messgebox("创建新记录文件错", res, 12);
			return -2;
		}
		hisp = (Rec_Information *)buf;
		memset(buf, ' ', sizeof(buf));
		BCD2Ascii((unsigned char*)&SysTime, (unsigned char*)hisp->yy, 7);//写入当前日期时间
		memcpy(hisp->device_no, pFistVary.DeviceNo,7);
		length = 0;
		memcpy((unsigned char*)&length,pFistVary.fileSno,2);//文件号
		sprintf((char*)hisp->package_serial_no,"%05u",length);//采集包序列号	(ASCII)
		length+=1;
		memcpy(pFistVary.fileSno,(unsigned char*)&length,2);//文件序列号加1
		sysfewrite(BIT_GET_FILESNO,2,(unsigned char*)&length);

		sprintf(hisp->RecLength, "%05d", DATALEN);//单条记录长度
		
		_Fwrite(buf, DATALEN, &rfile);
	}
	else{//新建 文件就不需要取长度定位了。没关闭文件前，用f_stat取出来是不对的，没有包含刚写入的数据长度
// 		f1 = f_size(&rfile_Wrecord);//取出来的长度不对。。。。。
// 		MSG_LOG("f_size length:%6.0f\r\n", f1);

		res = f_stat(fileName, &finf);
		if(res != FR_OK){
			f_close(&rfile);
			dis_messgebox("记录文件信息错", res, 12);
			return -3;
		}
		length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
		MSG_LOG("stat len:%d,%lld\r\n", length, finf.fsize);
		
		MSG_LOG("Rindex len:%d\r\n", length);
		f_lseek(&rfile, length);	//定位到文件最后面
	}




	length = get_timer0(2);
	MSG_LOG("write FAT delay1:%dms\r\n", (2000-length));//测试写记录时间多长

	MSG_LOG("WFAT:");
	BCD_LOG(wbuf, DATALEN,1);
	if(_Fwrite(wbuf, DATALEN, &rfile) != DATALEN){
		f_close(&rfile);
		dis_messgebox("写记录文件错", res, 12);
		return -4;		
	}

	length = get_timer0(2);
	MSG_LOG("write FAT delay2:%dms\r\n", (2000-length));//测试写记录时间多长
// #ifdef _debug_
// 	memset(fileName,0,sizeof(fileName));
// 	sprintf(fileName,"write FAT:%dMS",(2000-ato));
	//	debugstring(fileName);
// 	miniDispstr(0,0,(const char *)fileName,0);
// #endif



	f_close(&rfile);



 
	length = get_timer0(2);
	MSG_LOG("write FAT delay:%dms\r\n", (2000-length));//测试写记录时间多长
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
	//没有索引文件，自己建立
	res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_OPEN_ALWAYS);
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
		dis_messgebox("创建记录文件错", res, 12);
		return -2;
	}
	res = f_stat(fileName, &finf);
	if(res != FR_OK){
		f_close(&rfile);
		dis_messgebox("记录文件信息错", res, 12);
		return -3;
	}
	length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
	MSG_LOG("stat len:%d,%lld\r\n", length, finf.fsize);
	
	MSG_LOG("time.txt len:%d\r\n", length);
	f_lseek(&rfile, length);	//定位到文件最后面

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



//检测有没有需要采集的记录
// =0 有信息错误，不操作
// 1 要处理信息
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
//找到某个文件并写发送信息
//1 有记录文件了 并返回文件总长
// 0 无
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
			return 0;//没找到后台要采集的信息
		}
		
		memset(fileName,0,sizeof(fileName));
		strcpy(fileName, "1:/curdat/his");	//历史文件
		BCD2Ascii(hismsg.sfilename+2,(unsigned char *)(fileName+strlen(fileName)),2);
		MSG_LOG("path:%s\r\n",fileName);
		
		//读文件测试
		res = f_open(&rfile, fileName, FA_OPEN_EXISTING | FA_READ);
		if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
		{
			SaveFAT_hisRecInfor(0);		//	剔除一个文件，找不到就剔除 然后自增
			
		}else{		
			break;		//有个文件成功了
		}
		
		if (memcmp(hismsg.sfilename,hismsg.enddate,4) == 0)//没有文件了退出来
		{
			return 0;
		}
		
	} 
	
	
	//成功打开，写信息
	
	res = f_stat(fileName, &finf);
	if(res != FR_OK){
		f_close(&rfile);
		return 0;
	}

	length = hismsg.offset;
	if (hismsg.offset == 0)
	{
		MSG_LOG("跳过头128--\r\n");
		
		length += DATALEN;	//
		
	}

	MSG_LOG("读文件:%s\r\n",fileName);
	MSG_LOG("off:%d,flen:%lld\r\n", length, finf.fsize);
	
	f_lseek(&rfile, length);	//定位到文件最后面
	if(_Fread(getbuff, 8, &rfile) != 8){
		f_close(&rfile);
		return 0;		
	}
	if(memcmp(getbuff, "银行数据", 8) == 0)//
	{
		debugstring("该记录为银行卡记录\r\n");
		Qboboc_flag = 1;//区分银行记录和普通记录
	}
	else
	{
		Qboboc_flag=0;
		debugstring("该记录为普通卡记录\r\n");
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

//发送一条以后历史记录信息要增加，并剔除已发送完的文件 直到发送完 发送完后把时间写成非法
//0  改起始文件
//1  改偏移量
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
		TimeAdd(getbuff,getbuff+20,86400);	//一天
		memcpy(hismsg.sfilename,getbuff,4);
		hismsg.offset = 0;
		hismsg.filelen = 0;


		if (memcmp(hismsg.sfilename,hismsg.enddate,4) == 0)
		{
			MSG_LOG("collect END2--\r\n");
			memset(&hismsg.sfilename,0,4);		//制成非法
			sysfewrite(BIT_REORD_HIS,BIT_REORD_HIS_LEN,(unsigned char *)&hismsg);
			return 0;	//没有记录了
		}
		break;
	case 1:

// 		MSG_LOG("off add---\r\n");
// 		MSG_LOG("off:%d,allen:%d\r\n",hismsg.offset,hismsg.filelen);
		if(Qboboc_flag==1)
			hismsg.offset += 256;
		else
		hismsg.offset += DATALEN;
		  Qboboc_flag=0;//清银行卡记录标志位

		if (hismsg.offset >= hismsg.filelen)
		{
			
			MSG_LOG("remove one file-2--\r\n");

			memcpy(getbuff+20,hismsg.sfilename,4);
			memcpy(getbuff+20+4,"\x01\x59\x59",3);
			TimeAdd(getbuff,getbuff+20,86400);	//一天
			memcpy(hismsg.sfilename,getbuff,4);

			hismsg.offset = 0;
			hismsg.filelen = 0;

			if (memcmp(hismsg.sfilename,hismsg.enddate,4) == 0)
			{
				MSG_LOG("collect END--\r\n");
				memset(&hismsg.sfilename,0,4);		//制成非法
				sysfewrite(BIT_REORD_HIS,BIT_REORD_HIS_LEN,(unsigned char *)&hismsg);
				return 0;	//没有记录了 
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


//检测有没有需要补采集的记录
// <0  有错误
// == 0 无记录
// 1 有要发送的记录并返回信息
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
		return 0;//没找到后台要采集的信息		
	}

	MSG_LOG("find1--\r\n");


	if(Check_hisfile(&hismsg)  != 1)
	{
		return 0;	//没找到列表
	}

	MSG_LOG("find2--\r\n");



	memset(fileName,0,sizeof(fileName));
	strcpy(fileName, "1:/curdat/his");	//历史文件
	BCD2Ascii(hismsg.sfilename+2,(unsigned char *)(fileName+strlen(fileName)),2);





//	MSG_LOG("path:%s\r\n",fileName);
	//读文件测试
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
		MSG_LOG("跳过头128--\r\n");
		
		hismsg.offset += DATALEN;	//

	}

	if (hismsg.offset >= hismsg.filelen)
	{
		return 0;	// 可能是个空记录 理论上不可能，但是
	}
	

	length = hismsg.offset;
	MSG_LOG("读文件:%s\r\n",fileName);
	MSG_LOG("off:%d,flen:%lld\r\n", length, finf.fsize);

	f_lseek(&rfile, length);	//定位到文件最后面
	if(_Fread(disbuf, DATALEN, &rfile) != DATALEN){
	
		f_close(&rfile);
		return 0;		
	}
	
	
	if(Qboboc_flag==1)
	{
		memcpy(recrord,disbuf+8,120);
		length+=DATALEN;
		f_lseek(&rfile, length);	//定位到文件最后面
		if(_Fread(recrord+120, 21, &rfile) != 21){
			f_close(&rfile);
			return 0;		
		}
		debugstring("银行卡数据：");
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
		dis_messgebox("创建记录文件错", res, 12);
		return -2;
	}

	//新建 文件就不需要取长度定位了。没关闭文件前，用f_stat取出来是不对的，没有包含刚写入的数据长度
	f1 = f_size(&rfile);//取出来的长度不对。。。。。
	MSG_LOG("f_size length:%6.0f\r\n", f1);
	
	res = f_stat(fileName, &finf);
	if(res != FR_OK){
		f_close(&rfile);
		dis_messgebox("记录文件信息错", res, 12);
		return -3;
	}

	length = finf.fsize;//f_size(&filIndexHandle);//filIndexHandle.obj.objsize;//
	MSG_LOG("stat len:%d,%lld\r\n", length, finf.fsize);
	
	MSG_LOG("Rindex len:%d\r\n", length);
	MSG_LOG("写文件:--\r\n");
	f_lseek(&rfile, length);	//定位到文件最后面
	if(_Fwrite(dfiledata, datlen, &rfile) != datlen){
		f_close(&rfile);
		dis_messgebox("写记录文件错", res, 12);
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
		dis_messgebox("打开目录错", res, 12);
		res = f_mkdir("1:/download");
		if(res != FR_OK){
			dis_messgebox("建立目录错", res, 12);
			return -1;
		}
	}
	
	memset(fileName, 0, sizeof(fileName));

	strcpy(fileName, "1:/download/");	//历史文件
	strcat(fileName  ( char *)fileNameTmp);//文件名
	MSG_LOG("Write RecordFat FileName%s\r\n", fileName);

	MSG_LOG("覆盖建立新文件:%s\r\n", fileName);
	res = f_open(&rfile, fileName, FA_READ|FA_WRITE|FA_CREATE_ALWAYS);//覆盖建立新文件
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
		dis_messgebox("创建新记录文件错", res, 12);
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
	



	//下面更新

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


	MSG_LOG("建立新文件:%s\r\n", new_path);
	res = f_open(&rfile, new_path, FA_READ|FA_WRITE|FA_CREATE_ALWAYS);//覆盖建立新文件
	if(res != FR_OK)//if(access("0:/current/Rindex.txt",0)!=0)
	{
// 		dis_messgebox("创建新记录文件错", res, 12);
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
				dis_messgebox("程序文件读取错误", length, 12);
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
// 				dis_messgebox("程序文件读取错误", length, 12);
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



