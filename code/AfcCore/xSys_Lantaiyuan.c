#include "Macro_Proj.h"
#include "xSys_Lantaiyuan.h"



//读CPU序列号，一只有12字节长度，因系统中只能用4字节，所以把这12字节计算了一个MAC值，当做4字节的唯一序列号。
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

//读	F4 CPU序列号，一只有12字节长度，因系统中只能用4字节，所以把这12字节计算了一个MAC值，当做4字节的唯一序列号。
void Get_SerialNumF4(unsigned char *ser)
{
	Get_SerialNum(ser);
}



