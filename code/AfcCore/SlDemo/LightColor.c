#include "Macro_Proj.h"
#include "LightColor.h"
#include "DemoMain.h"

#include "UtilityProc.h"

#include "inputmisc/GPIOCtrl.h"

#include "../include/LtyError.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <linux/input.h>  
//#include <stdlib.h>  
//#include <sys/types.h>  
//#include <sys/stat.h>  

#define KeyBoardDEV_PATH "/dev/input/event1"   //difference is possible  

struct ev_loop *loop;


/**	
*   ����:����ܿ��ƺ���
*	��ʾ   0    1    2    3    4    5    6    7    8    9
*	bufֵ  0    1    2    3    4    5    6    7    8    9

*   ��ʾ   0.   1.    2.    3.    4.    5.    6.    7.    8.    9.
*   bufֵ  10   11    12    13    14    15    16    17    18    19

*    -    A    B    C    D    E    F     A.    B.    C.    D.    E.    F.   �� 
*   20    21   22   23   24   25   26    27    28    29    30    31    32   33
*   ������ʾ-*     

*��������bufֵ��ʾ��
*  ������ʾ2.5 buf={71,71,12,5}
*  �¼�����ַ� 
*/

/*
* ��������: ����ͨ�ַ�,ת��ΪLED�󱳵���ʶ����ַ�
* ���������inch--��ͨ�ַ�, point--�Ƿ��С����(0--����, 1--��)
* ����������� 
* �� �� ֵ����Ӧ��LED����ʶ����ַ�
* ���ܱ�ע:
*/
char LED_ShowIO(char inch, int point)
{
	char outch;	
	
	if(inch >= '0' && inch <= '9')
	{
		outch = inch - '0';

		if(point == 1)
			outch += 10;
	}
	else if(inch >= 'A' && inch <= 'F')
	{
		outch = inch - 'A';
		outch += 21;

		if(point == 1)
			outch += 6;
	}
	else if(inch == '-')
		outch = 20;
	else
		outch = 33;

	return outch;
}

/*
* ��������: ����ͨ�ַ���,ת��ΪLED�󱳵���ʶ����ַ���
* ���������inbuf--��ͨ�ַ���, inlen--��ͨ�ַ�������, outbuf--LED����ʶ���ַ���
* ����������� 
* �� �� ֵ����
* ���ܱ�ע:
*/
void Show_LEDBuff(char *inbuf, int inlen, char *outbuf)
{
	int i, j, point_flag = 0;

	inbuf[inlen] = 0x00;

	/*������ʾ���Ƿ���С��*/
	for(i=0; i<inlen; i++)
	{
		if(inbuf[i] == '.')
		{
			point_flag = 1;
			break;
		}
	}

	for(i=0, j=0; i<4; i++)
	{
		if(i+inlen < 4+point_flag)		/*��buf������λ,ǰ�油�ո�*/
		{
			outbuf[i] = 33;
			continue;
		}
		
		if(inbuf[j+1] == '.')
		{
			outbuf[i] = LED_ShowIO(inbuf[j++], 1);
			j++;
		}
		else
			outbuf[i] = LED_ShowIO(inbuf[j++], 0);
	}
}

/*
* �������ƣ�IO_LEDBuff 
* ��������: ��4λ(��С����5λ)����ʾ������С�����2λ�������
* ���������inbuf:��Ҫ�������ʾ���ݣ�outbuf:�Բ������ʾ����
* �����������
* �� �� ֵ����
* ���ܱ�ע:
*/

void IO_LEDBuff(char *inbuf, char *outbuf)
{
	int len, i, be_point=0, af_point=0, point=0, flag=0;
	char ibuf[6];

	strcpy(ibuf, inbuf);

	len = strlen(ibuf);
	
	for(i=0; i<len; i++)
	{
		if(flag == 0 && ibuf[i] == '.')
		{
			flag = 1;
			point = i;
		}
		else if(flag == 0)
			be_point++;
		else if(flag == 1)
			af_point++;
	}

	if(len < 5 && be_point < 4 && af_point < 2)
	{
		if(af_point == 0)
		{
			if(be_point > 2)
			{
				ibuf[len] = '.';
				ibuf[len+1] = '0';
				len += 2;
			}
			else
			{
				ibuf[len] = '.';
				ibuf[len+1] = '0';
				ibuf[len+2] = '0';
				len += 3;
			}
		}
		else if(af_point == 1)
		{
			ibuf[len] = '0';
			len += 1;
		}
	}

	Show_LEDBuff(ibuf, len, outbuf);
}


/*
* �������ƣ�Show_AFLED 
* ��������: ������ʾ����LED���ַ�
* ���������pbuf:��ʾ������
* ���������LTY_SUCCESS:�ɹ�; ENOTINIT:���豸ʧ��; 
errno:д����ʧ��
* �� �� ֵ����
* ���ܱ�ע:
*/
int Show_AFLED(char *pbuf)
{	
	//write(m_tm1650fd,pbuf,4);	return 0;	
	
	char path[64] = "/dev/tm1650";  
    char value_str[3];  
    int fd;  
    //snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);  

    fd = open(path, O_WRONLY);  
    if (fd < 0) {  
		 printf("open PATH is losed!");
         return ENOTINIT;
    }  
    if (write(fd, pbuf, 4) < 0) {  

         return errno;
    }
    close(fd);  	
	
	return LTY_SUCCESS;
}

/*
* �������ƣ�SetScreenLight_Color 
* ��������: ������Ļ�Ϸ��Ƶ���ɫ
* ���������on_off:����, pin: ����ɫ������
* ����������� 
* �� �� ֵ����
* ���ܱ�ע:
*/
void SetScreenLight_Color(int on_off, int pin)
{
	Init_SCREEN(loop);
	
	if(on_off == LIGHT_OPEN)
	{
		SetGPIODir(pin, GPIO_DIR_OUT);
		GPIOSetVal(pin, GPIO_PIN_HI);
	}
		
}

//PH11 ping = 235, 0��
void SetR485_EN(int on_off)
{
	int ret;
	ret = SetGPIODir(235, GPIO_DIR_OUT);
//	printf("[%s]1 ret = %d\r\n", __FUNCTION__, ret);
	if(on_off == 1){
		ret = GPIOSetVal(235, GPIO_PIN_HI);
//		printf("[%s]2 ret = %d\r\n", __FUNCTION__, ret);
	}
	else{
		ret = GPIOSetVal(235, GPIO_PIN_LOW);
//		printf("[%s]3 ret = %d\r\n", __FUNCTION__, ret);
	}
}



/*
* �������ƣ�SetQRLight_Color 
* ��������: ����ɨ�����Ƶ���ɫ
* ���������on_off:����, pin: ����ɫ������
* ����������� 
* �� �� ֵ����
* ���ܱ�ע:
*/
void SetQRLight_Color(int on_off, int pin)
{
	Init_QR(loop);

	if(on_off == LIGHT_OPEN)
	{

		SetGPIODir(pin, GPIO_DIR_OUT);
		GPIOSetVal(pin, GPIO_PIN_HI);
	}
	
}


/*
*  �������ƣ�  Light_main 
*  ��������:   �ƶ��⺯��
*  ���������  LightChoose:ѡ������ĵ�, on_off:����, pin: ����ɫ������, buff: ��LED����ʾ�ַ���
*  ���������  �� 
*  ����ֵ  ��  �������
*/
int Light_main(int LightChoose, int on_off, int pin, char *buff)
{
	if(LightChoose == SCREEN_LIGHT)
	{
		SetScreenLight_Color(on_off, pin);
	}
	else if(LightChoose == QR_LIGHT)
	{
		SetQRLight_Color(on_off, pin);
	}
	else if(LightChoose == LED_LIGHT)
	{
		char outbuff[10];
		
		IO_LEDBuff(buff, outbuff);
		Show_AFLED(outbuff);
	}

	return Ret_OK;
}

/*
*  �������ƣ�  Voice_main 
*  ��������:   �����������⺯��
*  ���������  VoiceChoose:ѡ��Ҫ����������
*  ���������  �� 
*  ����ֵ  ��  ��
*/

void Voice_main(int VoiceChoose)
{
	if(VoiceChoose == Invaild_Code)
		system("aplay /mnt/qrdata/wav/93.wav");
	else if(VoiceChoose == Fail_OpenDoor)
		system("aplay /mnt/qrdata/wav/94.wav");
	else if(VoiceChoose == Fail_CountryCode)
		system("aplay /mnt/qrdata/wav/95.wav");
	else if(VoiceChoose == Re_QWCode)
		system("aplay /mnt/qrdata/wav/96.wav");
	else if(VoiceChoose == NoAgain_Card)
		system("aplay /mnt/qrdata/wav/97.wav");
	else if(VoiceChoose == WelCome)
		system("aplay /mnt/qrdata/wav/success.wav");
	else if(VoiceChoose == FrontDoor_Down)
		system("aplay /mnt/qrdata/wav/frontdoordown.wav");
	else if(VoiceChoose == Thanks_Again)
		system("aplay /mnt/qrdata/wav/thankyou.wav");
}

