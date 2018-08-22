#include "Macro_Proj.h"
#include "RecordFile.h"
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

#include "SlzrTypeDef.h"
#include "PSAMLib.h"
#include "MYDES.h"

#include "ICCardLib.h"
#include "STProLib.h"

#define _debug_recordFile



stFileRecordPoint recordPoint;//指向当前在记录内的写入偏移的指针0-2M

#define HIS_TLL 10
typedef struct{
	char fileName[256];
	int rfile_Wrecord;
	int Tll;	//延时，延时一段时间关闭文件再打开。
}stTTFile;

stTTFile ghisfile;
stTTFile recordFile_fd;

//打开一个文件，如果不存在则新建。
int file_open_creat(char *filename)
{
	int hand;
	int ret=-1;
	
	ret = access(filename, F_OK);
	
	printf("[%s]access file %s ret:%d\n", __FUNCTION__, filename, ret);
	
	if(ret >= 0){						//文件存在
		hand = open(filename, O_RDWR);
		if (hand < 0)
		{
			printf("open %s fail!\n", filename);
			perror("open");
		}
		
	}
	else{	//文件对 当前新建一个
		printf("[%s] %s不存在,新建...\r\n", __FUNCTION__, filename);
		
		hand = open(filename, O_CREAT | O_RDWR, S_IRWXG | S_IRWXO | S_IRWXU);	
		
	}
	return hand;
}

int RecordFile_open(void)
{
	unsigned char buff[1024];
	char fullName[300];
	int i,  ret=-1, flag=1;
	
	
	strcpy(fullName, WorkDir);
	strcat(fullName, _File_Record_Cur);	//当前文件，2M空间
	
	ret = access(fullName, F_OK);
	
	printf("[%s]access file %s ret:%d\n", __FUNCTION__, fullName, ret);
	
	if(ret >= 0){						//文件存在
		recordFile_fd.rfile_Wrecord = open(fullName, O_RDWR);
		if (recordFile_fd.rfile_Wrecord >= 0) {
			flag = 0;
		}
		else
		{
			printf("open %s fail!\n", fullName);
			perror("open");
			flag = 2;
		}
		
	}
	
	if(flag != 0){	//文件对 当前新建一个
		printf("[%s] %s不存在,新建...\r\n", __FUNCTION__, fullName);
		
		recordFile_fd.rfile_Wrecord = open(fullName, O_CREAT | O_RDWR, S_IRWXG | S_IRWXO | S_IRWXU);	
		if (recordFile_fd.rfile_Wrecord >= 0)
		{
			ret = lseek(recordFile_fd.rfile_Wrecord, 0, SEEK_SET);
			
			printf("[%s] %s lseek! ret:%d\r\n", __FUNCTION__, fullName, ret);
			
			memset(buff, 0xFF, sizeof(buff));
			for (i=0; i<(RECORD_SPACE_LENGH/1024); i++)	//写入2M字节的FF
			{
				ret = write(recordFile_fd.rfile_Wrecord, buff, 1024);//
				if(ret < 0){
					printf("[%s] %s写入文件误@! ret:%d\r\n", __FUNCTION__, fullName, ret);
					flag = 3;
					break;
				}
			}
		}
		else{
			printf("[%s] %s创建 文件误@! fd:%d\r\n", __FUNCTION__, fullName, recordFile_fd.rfile_Wrecord);
			
			flag = 4;
		}
		
	}
	
	
	return flag;
}

int Record_PointFile_open(void)
{
	char fullName[300];
	int fd=-1, ret=-1, flag=1;
	
	strcpy(fullName, WorkDir);
	strcat(fullName, _File_Record_Point);	//当前文件，2M空间
	
	ret = access(fullName, F_OK);
	
	printf("[%s]access file %s ret:%d\n", __FUNCTION__, fullName, ret);
	
	if(ret >= 0){						//文件存在
		fd = open(fullName, O_RDWR);
		if (fd >= 0) {
			flag = 0;
		}
		else
		{
			printf("open %s fail!\n", fullName);
			perror("open");
			flag = 2;
		}
		
	}
	
	if(flag != 0){	//文件对 当前新建一个
		printf("[%s] %s不存在,新建...\r\n", __FUNCTION__, fullName);
		
		fd = open(fullName, O_CREAT | O_RDWR, S_IRWXG | S_IRWXO | S_IRWXU);	
		if (fd >= 0)
		{
			recordPoint.headp = 0;
			recordPoint.endp = 0;
			recordPoint.crc = GenerateCRC32((unsigned char *)&recordPoint.headp, 8);
			
#ifdef _debug_recordFile
			printf("[%s] %s写文件ret:%d\r\n", __FUNCTION__, fullName, ret);
			debugdata((unsigned char *)&recordPoint, sizeof(stFileRecordPoint), 1);
#endif
			ret = write(fd, (unsigned char *)&recordPoint, sizeof(stFileRecordPoint));//
			if(ret < 0){
				printf("[%s] %s写入文件误@! ret:%d\r\n", __FUNCTION__, fullName, ret);
				flag = 3;
			}
		}
		else{
			printf("[%s] %s创建 文件误@! fd:%d\r\n", __FUNCTION__, fullName, fd);
			
			flag = 4;
		}
		
	}
	
	if(fd >= 0){
		ret = lseek(fd, 0, SEEK_SET);

		read(fd, (unsigned char*)&recordPoint, sizeof(stFileRecordPoint));	//把存贮信息读出来
		
#ifdef _debug_recordFile
		printf("[%s] %s读文件ret:%d\r\n", __FUNCTION__, fullName, ret);
		debugdata((unsigned char *)&recordPoint, sizeof(stFileRecordPoint), 1);
#endif
		close(fd);
	}
	
	return flag;
}

//读取文件的内容
int GetFileDatac(char *filename, int offset, int len, unsigned char *obuf)
{
	int fd, res;
	
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		MSG_LOG("[%s] OPEN FILES[%s] fail!\r\n", __FUNCTION__, filename);
		perror(__FUNCTION__);
		return fd;
	}
	res = read(fd, obuf, len);
	if (res < 0) {
		MSG_LOG("[%s] Read FILES[%s] fail!\r\n", __FUNCTION__, filename);
		perror(__FUNCTION__);
	}
	
	close(fd);
	return res;
}

//检测有没有需要采集的记录
// =0 有信息错误，不操作
// 1 要处理信息
int Check_Send_FAT_Rcord(stFAT_hisRec *hisinfor)
{//需要根据补采的天数来确定补采的文件及文件的开始地址。
#warning "check senf fat rcord need code!!"

	return 0;
}

//如果传的参数为>1，则需要把当前发送的位置偏移加上记录长度*num 。offset
//如果是0，则直接存贮结构数据
int SaveFAT_hisRecInfor(unsigned char num, stFAT_hisRec *hisinfor)
{//保存补采上传的信息，如果一个补采完成了，则需要修改补采的起始日期
	unsigned char buff[512];
	char fullName[300];
	int fd=-1, ret=-1, flag=1;
	
	if(num != 0){
		hisinfor->offset += (num * RECORD_JTB_LEN);	//偏移量加
		if(hisinfor->offset >= hisinfor->filelen){//这个文件采完了,日期要加1
			
			memcpy(buff, hisinfor->startdate, 4);
			memcpy(buff, "\x23\x59\x59", 3);
			ret = BCDTime2Long(buff);
			
			memcpy(buff, hisinfor->enddate, 4);
			memcpy(buff, "\x23\x59\x59", 3);
			flag = BCDTime2Long(buff);

			if(ret >= flag){//如果起始时间大于或超过结束日期则采集结束
				memset((unsigned char *)&hisinfor->colStyle, 0, sizeof(hisinfor));
			}
			else{
				TimeAdd(buff+20, buff, (24*60*60));//开始日期加一天
				memcpy(hisinfor->startdate, buff+20, 4);
			}
			hisinfor->offset = 0;
			hisinfor->filelen = 0;
		}
	}
	hisinfor->scrc32 = GenerateCRC32((unsigned char *)&hisinfor->colStyle, BIT_REORD_HIS_LEN-4);


	strcpy(fullName, WorkDir);
	strcat(fullName, _File_His_Collect);
	
	remove(fullName);
	
	printf("[%s] %s,新建...\r\n", __FUNCTION__, fullName);
	
	fd = open(fullName, O_CREAT | O_RDWR, S_IRWXG | S_IRWXO | S_IRWXU);	
	if (fd < 0){
		printf("[%s] %s创建 文件误@! fd:%d\r\n", __FUNCTION__, fullName, fd);
		
		flag = 4;
		
	}
	
	if(fd >= 0){
		ret = lseek(fd, 0, SEEK_SET);
		
#ifdef _debug_recordFile
		printf("[%s] %s写文件ret:%d\r\n", __FUNCTION__, fullName, ret);
		debugdata((unsigned char *)&hisinfor->colStyle, sizeof(hisinfor), 1);
#endif
		
		ret = write(fd, (unsigned char *)&hisinfor->colStyle, sizeof(hisinfor));//
		
		close(fd);
	}
	


	return 0;

}

int Record_PointFile_save(void)
{
	char fullName[300];
	int fd=-1, ret=-1, flag=1;
	
	
	strcpy(fullName, WorkDir);
	strcat(fullName, _File_Record_Point);	//当前文件，2M空间
	

	remove(fullName);
	
	printf("[%s] %s,新建...\r\n", __FUNCTION__, fullName);
	
	fd = open(fullName, O_CREAT | O_RDWR, S_IRWXG | S_IRWXO | S_IRWXU);	
	if (fd < 0){
		printf("[%s] %s创建 文件误@! fd:%d\r\n", __FUNCTION__, fullName, fd);
		
		flag = 4;

	}
	
	if(fd >= 0){
		ret = lseek(fd, 0, SEEK_SET);
		recordPoint.crc = GenerateCRC32((unsigned char *)&recordPoint.headp, 8);
		
#ifdef _debug_recordFile
		printf("[%s] %s写文件ret:%d\r\n", __FUNCTION__, fullName, ret);
		debugdata((unsigned char *)&recordPoint, sizeof(stFileRecordPoint), 1);
#endif

		ret = write(fd, (unsigned char *)&recordPoint, sizeof(stFileRecordPoint));//

		close(fd);
	}
	
	return flag;
}

int close_rfile_Wrecord(stTTFile *sfP)
{
	MSG_LOG("do %s\r\n", __FUNCTION__);
	if(sfP->rfile_Wrecord >= 0)
		close(sfP->rfile_Wrecord);
	sfP->rfile_Wrecord = -1;
	sfP->Tll = 0;
	return 0;
}

int open_rfile_Wrecord(void)
{
	int res;
//	int length;
	
	unsigned char datet[16];
	unsigned char buf[256];
	
	MSG_LOG("do %s\r\n", __FUNCTION__);
	
	memset(datet, 0, sizeof(datet));
	get_datatime((char*)datet);
	
	memset(ghisfile.fileName, 0, sizeof(ghisfile.fileName));
	
	strcpy(ghisfile.fileName, WorkDir);
	strcat(ghisfile.fileName, "/his");
	BCD2Ascii(gDeviceParaTab.SN, (unsigned char*)ghisfile.fileName + strlen(ghisfile.fileName), 4);	//设备序列号
	memcpy(ghisfile.fileName + strlen(ghisfile.fileName), datet + 4, 4);	//日期时间
	strcat(ghisfile.fileName, ".his");
	
	MSG_LOG("dealCOM:%s\r\n", ghisfile.fileName);
	ghisfile.rfile_Wrecord = -1;
	ghisfile.Tll = 0;
	
	if (access(ghisfile.fileName, F_OK) != -1) {//文件存在，看看是不是今本年的
		ghisfile.rfile_Wrecord = open(ghisfile.fileName, O_RDONLY);
		if (ghisfile.rfile_Wrecord < 0) {
			MSG_LOG("OPEN FILES[%s] fail!\r\n", ghisfile.fileName);
			perror(__FUNCTION__);
			return -2;
		}
		res = read(ghisfile.rfile_Wrecord, buf, 8);
		if (res < 0) {
			MSG_LOG("Read FILES[%s] fail!\r\n", ghisfile.fileName);
			perror(__FUNCTION__);
			return -2;
		}
		if (memcmp(buf, datet, 4) != 0) {//不是今年的文件，删掉重建
			close(ghisfile.rfile_Wrecord);
			remove(ghisfile.fileName);
			
			ghisfile.rfile_Wrecord = open(ghisfile.fileName, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
			if (ghisfile.rfile_Wrecord < 0)
			{
				MSG_LOG("Creat FILES[%s] fail!\r\n", ghisfile.fileName);
				perror(__FUNCTION__);
				return -3;
			}
			write(ghisfile.rfile_Wrecord, datet, 14);	//在文件的最前面写入日期时间
		}
		
		close(ghisfile.rfile_Wrecord);
		ghisfile.rfile_Wrecord = open(ghisfile.fileName, O_WRONLY);//再以写的方式打开文件
		if (res < 0) {
			MSG_LOG("OPen 2 FILES[%s] fail!\r\n", ghisfile.fileName);
			perror(__FUNCTION__);
			return -4;
		}
		
	}
	else {
		ghisfile.rfile_Wrecord = open(ghisfile.fileName, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
		if (ghisfile.rfile_Wrecord < 0)
		{
			MSG_LOG("Creat FILES[%s] fail!\r\n", ghisfile.fileName);
			perror(__FUNCTION__);
			return -5;
		}
		write(ghisfile.rfile_Wrecord, datet, 14);	//在文件的最前面写入日期时间
	}
	
	lseek(ghisfile.rfile_Wrecord, 0, SEEK_END);
	
	return 0;
}

void initRecordFilePara(void)
{
	Record_PointFile_open();
	RecordFile_open();
	open_rfile_Wrecord();
}

unsigned int Get_Record_point(unsigned char *headp, unsigned char mode)
{
	unsigned int itmp;

	
	itmp = GenerateCRC32((unsigned char *)&recordPoint.headp, 8);
	
// #ifdef _debug_recordFile
// 	printf("[%s] itmp:%08X\r\n", __FUNCTION__, itmp);
// 	debugdata((unsigned char *)&recordPoint, sizeof(stFileRecordPoint), 1);
// #endif

	if(itmp != recordPoint.crc){//i 不正确，重新取文件
		Record_PointFile_open();
	}

	memcpy(headp, (unsigned char *)&recordPoint.headp, 4);
	return recordPoint.endp;
}

//指示头指针或尾指针加rLen. mode=0为尾指针加rlen, mode=1为头指针加rLen
int ADD_Record_point(unsigned char mode, unsigned int rLen)
{
	int ret;
	if(mode == 0)
		recordPoint.endp += rLen;
	else
		recordPoint.headp += rLen;
	ret = Record_PointFile_save();	//保存

	return ret;
}

extern void writeBackRec(unsigned char*dat, int len, unsigned char mode)
{
	
	if (ghisfile.rfile_Wrecord <= 0) {
		open_rfile_Wrecord();
	}
	lseek(ghisfile.rfile_Wrecord, 0, SEEK_END);
	write(ghisfile.rfile_Wrecord, dat, len);
	
	if(ghisfile.Tll++ >= HIS_TLL){	//写入10次就关闭一下文件
		close_rfile_Wrecord(&ghisfile);
	}
}

// 写记录文件
int FR_flashwrite(unsigned int addr, unsigned char *writebuf, unsigned int length)
{
	int ret;
	
	printf("[%s] ret:%d\n", __FUNCTION__,  recordFile_fd.rfile_Wrecord);

	if(recordFile_fd.rfile_Wrecord <= 0){
		if (RecordFile_open() != 0)
			return -1;
	}
	if(addr > RECORD_SPACE_LENGH)
		return -2;
	
	lseek(recordFile_fd.rfile_Wrecord, addr, SEEK_SET);

	ret = write(recordFile_fd.rfile_Wrecord, writebuf, length);
	
	if(recordFile_fd.Tll++ >= HIS_TLL){	//写入10次就关闭一下文件
		close_rfile_Wrecord(&recordFile_fd);
	}

	return ret;

}

// 读记录文件
int FR_flashread(unsigned int addr,unsigned char *rec_data,unsigned int length)
{
	
	int ret;
	
	printf("[%s] ret:%d\n", __FUNCTION__,  recordFile_fd.rfile_Wrecord);

	if(recordFile_fd.rfile_Wrecord <= 0){
		if (RecordFile_open() != 0)
			return -1;
		if(recordFile_fd.rfile_Wrecord < 0)
			return -1;
	}
	
	if(addr > RECORD_SPACE_LENGH)
		return -2;

	lseek(recordFile_fd.rfile_Wrecord, addr, SEEK_SET);
	
	ret = read(recordFile_fd.rfile_Wrecord, rec_data, length);
	
	return ret;
}

void Showcard(void) {

}
























