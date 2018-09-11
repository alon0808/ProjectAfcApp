#include "Macro_Proj.h"
#include "xSys_Lantaiyuan.h"
#include "MYDES.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>



//��fileNameָ�����ļ������ж�ȡ��lineNumber��
//����ֵ���ɹ�����1��ʧ�ܷ���0
static int get_file_line(char *result, char *fileName, int lineNumber)
{

#define FILEBUFFER_LENGTH 5000
#define EMPTY_STR ""

	FILE *filePointer;
	int i = 0;
	char buffer[FILEBUFFER_LENGTH];
	char *temp;

	memset(buffer, '\0', FILEBUFFER_LENGTH * sizeof(char));
	strcpy(buffer, EMPTY_STR);

	if ((fileName == NULL) || (result == NULL))
	{
		return 0;
	}

	if (!(filePointer = fopen(fileName, "rb")))
	{
		return 0;
	}


	while ((!feof(filePointer)) && (i < lineNumber))
	{
		if (!fgets(buffer, FILEBUFFER_LENGTH, filePointer))
		{
			return 0;
		}
		i++;//��������Ǽ���һ����
	}

	if (0 != fclose(filePointer))
	{
		return 0;
	}

	if (0 != strcmp(buffer, EMPTY_STR))
	{
		while (NULL != (temp = strstr(buffer, "\n")))
		{
			*temp = '\0';
		}

		while (NULL != (temp = strstr(buffer, "\r")))
		{
			*temp = '\0';
		}



		strcpy(result, buffer);
	}
	else
	{
		strcpy(result, EMPTY_STR);
		return 0;
	}

	return 1;
}


//��CPU���кţ�һֻ��12�ֽڳ��ȣ���ϵͳ��ֻ����4�ֽڣ����԰���12�ֽڼ�����һ��MACֵ������4�ֽڵ�Ψһ���кš�
void Get_SerialNum(unsigned char *ser)
{
	int  icnt;

	char FileName[256];
	unsigned char csnbuf[200];

	unsigned char buff[20];
	unsigned char cmac[4];
	unsigned char kbuf[16];
	unsigned char radon[8];

	memset(buff, '1', sizeof(buff));
	buff[19] = 0;

	strcpy(FileName, "/sys/class/sunxi_info/sys_info");

	if (1 == get_file_line((char*)csnbuf, FileName, 3)) {
		for (icnt = 0; icnt < 10; icnt++)
			buff[icnt] = (unsigned char)csnbuf[icnt + 20];
#ifdef _debugPSAM_
		printf("write00 SN\"%s\"\n", (char*)csnbuf);
		printf("write 22SN\"%s\" Success.\n", (char*)buff);
#endif
	}

	memset(kbuf, 0x11, 16);
	memset(radon, 0x22, 8);

#ifdef _debugPSAM_
	printf("readd11 SN\"%s\" Success.\n", (char*)buff);
#endif

	CountMac((char*)kbuf, radon, buff, 16, cmac);

	printf("----SN----:");
	debugdata(cmac, 4, 1);

	memcpy(ser, cmac, 4);
}

//��	F4 CPU���кţ�һֻ��12�ֽڳ��ȣ���ϵͳ��ֻ����4�ֽڣ����԰���12�ֽڼ�����һ��MACֵ������4�ֽڵ�Ψһ���кš�
void Get_SerialNumF4(unsigned char *ser)
{
	Get_SerialNum(ser);
}

void delayxms(unsigned int xms) {
	usleep(xms);
}



