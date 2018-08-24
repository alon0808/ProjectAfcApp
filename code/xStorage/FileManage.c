#include "Macro_Proj.h"
#include "FileManage.h"

#include <unistd.h>
#include <fcntl.h>


int FmOpen(int *pHandle, char *pFilePath, int mode) {
	char fullName[300];
	int i, fd = -1, ret = -1, flag = 0;

	unsigned char *dp;
	unsigned int dLen;

	if (pHandle == NULL) {
		return Ret_Err_Param;
	}
	ret = access(fullName, F_OK);

	PRINT_DEBUG("[%s]access file %s ret:%d dLen:%d\n", __FUNCTION__, fullName, ret, dLen);
	if (ret >= 0) {
		fd = open(fullName, O_RDONLY);
		if (fd >= 0) {

		}
		else
		{
			PRINT_ERROR("open %s fail!\n", fullName);
		}
	}
	else {
		PRINT_INFOR_LOCATION("文件不存在,新建...", fullName);
		fd = open(fullName, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
	}



	return Ret_OK;
}

int FmWriteOnce(char *pFilePath, void *pData, int datLen) {

}

int FmClose(int *pHandle) {

}
