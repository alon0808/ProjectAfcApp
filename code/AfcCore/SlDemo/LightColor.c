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
*   功能:数码管控制函数
*	显示   0    1    2    3    4    5    6    7    8    9
*	buf值  0    1    2    3    4    5    6    7    8    9

*   显示   0.   1.    2.    3.    4.    5.    6.    7.    8.    9.
*   buf值  10   11    12    13    14    15    16    17    18    19

*    -    A    B    C    D    E    F     A.    B.    C.    D.    E.    F.   空 
*   20    21   22   23   24   25   26    27    28    29    30    31    32   33
*   其他显示-*     

*不是上述buf值显示空
*  比如显示2.5 buf={71,71,12,5}
*  新加入的字符 
*/

/*
* 函数功能: 将普通字符,转换为LED后背灯能识别的字符
* 输入参数：inch--普通字符, point--是否带小数点(0--不带, 1--带)
* 输出参数：无 
* 返 回 值：对应的LED灯能识别的字符
* 功能备注:
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
* 函数功能: 将普通字符串,转换为LED后背灯能识别的字符串
* 输入参数：inbuf--普通字符串, inlen--普通字符串长度, outbuf--LED灯能识别字符串
* 输出参数：无 
* 返 回 值：无
* 功能备注:
*/
void Show_LEDBuff(char *inbuf, int inlen, char *outbuf)
{
	int i, j, point_flag = 0;

	inbuf[inlen] = 0x00;

	/*查找显示中是否有小数*/
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
		if(i+inlen < 4+point_flag)		/*当buf不足四位,前面补空格*/
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
* 函数名称：IO_LEDBuff 
* 函数功能: 对4位(加小数点5位)的显示内容做小数点后2位补齐操作
* 输入参数：inbuf:需要补齐的显示内容，outbuf:以补齐的显示内容
* 输出参数：无
* 返 回 值：无
* 功能备注:
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
* 函数名称：Show_AFLED 
* 函数功能: 设置显示到后背LED的字符
* 输入参数：pbuf:显示的内容
* 输出参数：LTY_SUCCESS:成功; ENOTINIT:打开设备失败; 
errno:写内容失败
* 返 回 值：无
* 功能备注:
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
* 函数名称：SetScreenLight_Color 
* 函数功能: 设置屏幕上方灯的颜色
* 输入参数：on_off:开关, pin: 灯颜色的设置
* 输出参数：无 
* 返 回 值：无
* 功能备注:
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

//PH11 ping = 235, 0是
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
* 函数名称：SetQRLight_Color 
* 函数功能: 设置扫码区灯的颜色
* 输入参数：on_off:开关, pin: 灯颜色的设置
* 输出参数：无 
* 返 回 值：无
* 功能备注:
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
*  函数名称：  Light_main 
*  函数介绍:   灯对外函数
*  输入参数：  LightChoose:选择操作的灯, on_off:开关, pin: 灯颜色的设置, buff: 后背LED灯显示字符串
*  输出参数：  无 
*  返回值  ：  操作结果
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
*  函数名称：  Voice_main 
*  函数介绍:   声音播报对外函数
*  输入参数：  VoiceChoose:选择要播报的内容
*  输出参数：  无 
*  返回值  ：  无
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

