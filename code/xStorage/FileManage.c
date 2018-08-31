#include "Macro_Proj.h"
#include "FileManage.h"

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <unistd.h>
#include <fcntl.h>


#define DIRECTORY_FILE	"/mnt/posdata/"
#define FILENAME_PBOC_CA_PUBKEY	"PbocCaPubKey.config"

#define MAX_FILE_NUM	5

int s_fd[MAX_FILE_NUM] = { -1, -1, -1, -1, -1 };
//int s_fd[MAX_FILE_NUM];


#define GET_FILE_HANDLE(pTmpFd, fileNameType) 	if (fileNameType <= fnt_invalid || fileNameType >= fnt_max_value) {\
PRINT_ERR_LOCATION("param is out of range,please check", "");\
return Ret_Err_Param;\
}\
pTmpFd = s_fd + fileNameType;

int FmOpen(int fileNameType, int accessMode) {
	char fullName[300];
	int i, fd = -1, ret = -1, flag = 0;
	int *pTmpFd;
	char *pFileName;
	int mode;
	int retcode = Ret_OK;

	unsigned char *dp;
	unsigned int dLen;

	GET_FILE_HANDLE(pTmpFd, fileNameType);
	fd = *pTmpFd;
	switch (fileNameType)
	{
	case fnt_pboc_cakey:
		pFileName = FILENAME_PBOC_CA_PUBKEY;
		break;
	default:
		PRINT_ERR_LOCATION("param has no function to process,please check", "");
		retcode = Ret_Err_Param;
		goto FmOpen_OVER;
	}
	sprintf(fullName, "%s%s", DIRECTORY_FILE, pFileName);

	mode = O_RDWR;
	if ((accessMode & fam_read) != fam_read) {
		mode = O_WRONLY;
	}
	if ((accessMode & fam_write) != fam_write) {
		mode = O_RDONLY;
	}

	if (fd >= 0) {
		close(fd);
		fd = -1;
	}

	ret = access(fullName, F_OK);

	PRINT_DEBUG("[%s]access file %s ret:%d dLen:%d mode:%02X\n", __FUNCTION__, fullName, ret, dLen, accessMode);
	if (ret >= 0) {
		fd = open(fullName, O_RDONLY);
		if (fd >= 0) {
			//*pTmpFd = fd;
		}
		else
		{
			PRINT_ERR_LOCATION("open %s fail open!\n", fullName);
			retcode = Ret_Error;
			goto FmOpen_OVER;
		}
	}
	else if ((accessMode & fam_create) == fam_create) {
		PRINT_INFOR_LOCATION("文件不存在,新建...", fullName);
		fd = open(fullName, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
		if (fd >= 0) {
			//*pTmpFd = fd;
		}
		else
		{
			perror("read:");
			PRINT_ERR_LOCATION("1open fail new!\n", fullName);
			retcode = Ret_Error;
			goto FmOpen_OVER;
		}
	}
	else {
		retcode = Ret_Error;
		goto FmOpen_OVER;
	}
FmOpen_OVER:
	*pTmpFd = fd;
	PRINT_DEBUG("1FmOpen over:%d!\n", fd);

	return retcode;
}

int FmWriteOnce(int fileNameType, void *pData, int datLen) {
	char fullName[300];
	int i, fd = -1, ret = -1, flag = 0;
	int *pTmpFd;
	char *pFileName;
	int mode;

	unsigned char *dp;
	unsigned int dLen;

	GET_FILE_HANDLE(pTmpFd, fileNameType);
	fd = *pTmpFd;
	if (fd < 0) {

	}
	fd = *pTmpFd;



}


int FmWrite(int fileNameType, int sIndex, void *pData, int dLen) {
	int fd = -1, ret = -1;
	int *pTmpFd;
	char *pFileName;

	unsigned char *dp = (unsigned char *)pData;

	GET_FILE_HANDLE(pTmpFd, fileNameType);
	fd = *pTmpFd;
	if (fd < 0) {
		return Ret_Error;
	}
	if (dLen == 0) {
		return Ret_OK;
	}
	if (dLen < 0 || pData == NULL) {
		return Ret_Err_Param;
	}
	if (sIndex < 0) {
		ret = lseek(fd, 0, SEEK_END);
	}
	else {
		ret = lseek(fd, sIndex, SEEK_SET);
	}
	write(fd, dp, dLen);

	return Ret_OK;
}


int FmRead(int fileNameType, int sIndex, void *pData, int dLen) {
	int fd = -1, ret = -1;
	int *pTmpFd;
	int tmpI = 0;

	unsigned char *dp = (unsigned char *)pData;

	GET_FILE_HANDLE(pTmpFd, fileNameType);
	fd = *pTmpFd;
	if (fd < 0) {
		UNUSED_VAR(tmpI);
		return Ret_Error;
	}
	if (dLen == 0) {
		return Ret_OK;
	}
	if (dLen < 0 || pData == NULL) {
		return Ret_Err_Param;
	}
	if (sIndex < 0) {
		ret = lseek(fd, 0, SEEK_END);
	}
	else {
		ret = lseek(fd, sIndex, SEEK_SET);
	}
	tmpI = read(fd, pData, dLen);

	return Ret_OK;
}

int FmClose(int fileNameType) {
	int fd = -1;
	int *pTmpFd;

	GET_FILE_HANDLE(pTmpFd, fileNameType);
	fd = *pTmpFd;
	if (fd >= 0) {
		close(fd);
	}
	*pTmpFd = -1;

	return Ret_OK;
}
