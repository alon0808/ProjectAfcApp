#include "Macro_Proj.h"
#include "GPIOCtrl.h"
#include "LightColor.h"

#include <termios.h>
#include <fcntl.h>
#include "SlzrTypeDef.h"
#include "SlzrError.h"
#include "debug_.h"
#include "LtyLog.h"
#include "ev.h"
#include "LtyTimer.h"

#include <linux/input.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/stat.h>  



/*
*  GPIO 外设引脚定义
*/
#ifdef PRODUCT_P1

#define GPIO_3G_POWER 82
#define GPIO_3G2_RESET 81
#define GPIO_GPS_POWER 244
#define GPIO_GPS_RESET 245
#define POWER_CTRL 228    //PH4
#define CAMER_POWER 227   //PH3
#define PA_POWER 241	  //PH17
#define LED_R 53 //PB21
#define LED_G 41 //PB9
#define LED_B 42 //PB10

#elif defined PRODUCT_P2
#define GPIO_3G_POWER 82
#define GPIO_3G2_RESET 81
#define GPIO_GPS_POWER 230
#define GPIO_GPS_RESET 231
#define POWER_CTRL 228    //PH4
#define CAMER_POWER 227   //PH3
#define PA_POWER 241	  //PH17
#define LED_R 53 //PB21
#define LED_G 41 //PB9
#define LED_B 42 //PB10

#define RS485_1 235 //PH11
#define RS485_2 236 //PH12

#else
#define GPIO_3G_POWER 82
#define GPIO_3G2_RESET 81
#define GPIO_GPS_POWER 244
#define GPIO_GPS_RESET 245
#define POWER_CTRL 192    //PH4
#define CAMER_POWER 227   //PH3
#define PA_POWER 241	  //PH17
#define LED_R 53 //PB21
#define LED_G 41 //PB9
#define LED_B 42 //PB10

#define LED_RUN 248
#define LED_GPS 200
#define LED_LTE 101
#endif

#define BEER 246



#define GPIO_SET_DIR   0x1
#define GPIO_GET_DIR   0x2
#define GPIO_READ_BIT  0x3
#define GPIO_WRITE_BIT 0x4

#define GPIO_DEV "/dev/sun-gpio"

/*
*  GPIO 外设引脚定义
*/

//#endif

typedef struct
{
    SLZR_U32 u32Group;
    SLZR_U32 u32Pin;
    SLZR_U32 u32Val;
}gpio_info_t;


int m_fd, m_bstop;


/*
*  PH13 ~ PH16
*  PG0   ~ PG5
*  PH1 225 
*/
const SLZR_U32 IO_IN_arry[IO_IN_NUM]={240,239,238,225};
//? static struct pollfd fds[IO_IN_NUM];

SLZR_VOID CGPIOCtrl(void)
{
    m_fd = -1;
}

SLZR_VOID CGPIOCtrl_Release(void)
{
    if(m_fd > 0)
        close(m_fd);

    m_fd = -1;
}

SLZR_VOID _CGPIOCtrl(void)
{
    CGPIOCtrl_Release();
}

SLZR_RESULT Init_QR(struct ev_loop *loop)
{
//?    pthread_t th;
	SLZR_RESULT ret = SLZR_FAILURE;
	m_bstop = 0;
    CGPIOCtrl_Release();
//?	InitGPIOPin(QR_LIGHT,GPIO_PIN_HI,GPIO_DIR_OUT);
	InitGPIOPin(QR_B, GPIO_PIN_LOW,GPIO_DIR_OUT);
	InitGPIOPin(QR_R, GPIO_PIN_LOW,GPIO_DIR_OUT);
	InitGPIOPin(QR_G, GPIO_PIN_LOW,GPIO_DIR_OUT);	
	InitGPIOPin(QR_W, GPIO_PIN_LOW,GPIO_DIR_OUT);	
//	InitGPIOPin(QR_OPEN, GPIO_PIN_LOW,GPIO_DIR_OUT);
	
	ret = CGPIOCtrl_Open();
    if(SLZR_SUCCESS != ret)
    {
//?    	 ERROUT("fail to Open() gpio mod the resonis %s\n",strerror(errno));
    	 _DBG_("fail to Open() gpio mod the resonis \n");
    }
    return SLZR_SUCCESS;
}

SLZR_RESULT Init_3Gmode(void)
{
	int ret;

	InitGPIOPin(GPIO_3G_POWER,GPIO_PIN_HI,GPIO_DIR_OUT);
	
	ret = SetGPIODir(GPIO_3G_POWER, GPIO_DIR_OUT);
	printf("[%s]1 ret = %d\r\n", __FUNCTION__, ret);

	ret = GPIOSetVal(GPIO_3G_POWER, GPIO_PIN_HI);
	printf("[%s]2 ret = %d\r\n", __FUNCTION__, ret);
	
	
    ret = InitGPIOPin(PA_POWER,GPIO_PIN_HI,GPIO_DIR_OUT);
	printf("[%s]3 ret = %d\r\n", __FUNCTION__, ret);
	
	InitGPIOPin(BEER, GPIO_PIN_HI,GPIO_DIR_OUT);

    return ret;
}

SLZR_RESULT Init_SCREEN(struct ev_loop *loop)
{
//?    pthread_t th;
	SLZR_RESULT ret = SLZR_FAILURE;
	m_bstop = 0;
    CGPIOCtrl_Release();

	InitGPIOPin(SCREEN_B, GPIO_PIN_LOW,GPIO_DIR_OUT);
	InitGPIOPin(SCREEN_R, GPIO_PIN_LOW,GPIO_DIR_OUT);
	InitGPIOPin(SCREEN_G, GPIO_PIN_LOW,GPIO_DIR_OUT);
	InitGPIOPin(SCREEN_OPEN, GPIO_PIN_LOW,GPIO_DIR_OUT);
	
	ret = CGPIOCtrl_Open();
    if(SLZR_SUCCESS != ret)
    {
//?    	 ERROUT("fail to Open() gpio mod the resonis %s\n",strerror(errno));
    	 _DBG_("fail to Open() gpio mod the resonis \n");
    }
    return SLZR_SUCCESS;
}


SLZR_RESULT CGPIOCtrl_Open()
{
	int m_gpiofd;

    m_gpiofd = open(GPIO_DEV, O_RDWR);
    if(m_gpiofd < 0)
        return errno;

    return SLZR_SUCCESS;
}

/*
*   描述:
*		设置GPIO的值
*  输入:
*		gpio 端口,高低电平值
*/
SLZR_RESULT GPIOSetVal(SLZR_U32 pin , gpio_pin_e enPin)
{
	FILE *fp=NULL;
	SLZR_CHAR val[4]="out";
	SLZR_CHAR gpio_dir[64]={0};
	if (enPin == GPIO_PIN_LOW){
		strcpy(val , "0");		
	}else{
		strcpy(val , "1");
	}
	sprintf(gpio_dir , "/sys/class/gpio/gpio%d/value" , pin);
	fp = fopen(gpio_dir , "w");
	if (!fp){
		return errno;
	}
	if ( fwrite( val , 1 , strlen(val) , fp ) <= 0 ){
		perror("");		
	}
	fclose(fp);
	return SLZR_SUCCESS;	
}

/*
*   描述:
*		设置GPIO的 输出模式
*  输入:
*		gpio 端口,输出模式
*/
SLZR_RESULT SetGPIODir(SLZR_U32 pin, gpio_dir_e enDir)
{
	FILE *fp=NULL;
	SLZR_CHAR dir[4]="out";
	SLZR_CHAR gpio_dir[64]={0};
	if ( enDir == GPIO_DIR_IN ){
		strcpy(dir , "in");		
	}else{
		strcpy(dir , "out");
	}
	sprintf(gpio_dir , "/sys/class/gpio/gpio%d/direction" , pin);
	fp = fopen(gpio_dir , "w");
	if (!fp){
		return errno;
	}
	if ( fwrite( dir , 1 , strlen(dir) , fp ) <= 0 ){
		perror("");		
	}
	fclose(fp);
	
    return SLZR_SUCCESS;
}

/*
*   描述:
*		产生GPIO
*  输入:
*		gpio 端口
*/
SLZR_RESULT GPIOGenDir(SLZR_U32 pin)
{
	SLZR_CHAR gpio_dir[64]={0};	
	FILE *fp=NULL;
	SLZR_CHAR gpio_str[8] = {0};	
	
	sprintf(gpio_str , "%d" , pin);
	sprintf(gpio_dir , "/sys/class/gpio/gpio%d" , pin);
	fp = fopen( "/sys/class/gpio/export" , "w");
	if (!fp){
		return errno;
	}
	if (fwrite( gpio_str , 1 , strlen(gpio_str) , fp ) <= 0 ){
		perror("");
	}
	fclose(fp);
	while ( access(gpio_dir, F_OK)  < 0 ){
		usleep(100);
	}
	/*GPIO dir exists*/
    return SLZR_SUCCESS;
}

/*
*   描述:
*		清除设备GPIO
*  输入:
*		gpio 端口
*/
SLZR_RESULT ClrGPIODir(SLZR_U32 pin)
{
	SLZR_CHAR gpio_dir[64]={0};	
	FILE *fp=NULL;
	char gpio_str[8] = {0};	
	
	sprintf(gpio_str , "%d" , pin);
	sprintf(gpio_dir , "/sys/class/gpio/gpio%d" , pin);
	fp = fopen( "/sys/class/gpio/unexport" , "w");
	if (!fp){
		return errno;
	}
	if ( fwrite( gpio_str , 1 , strlen(gpio_str) , fp ) <= 0 ){
		perror("");		
	}
	fclose(fp);
	while ( access(gpio_dir, F_OK)  > 0 ){
		usleep(100);
	}
	return SLZR_SUCCESS;
}

/*
*   描述:
*		设置GPIO的状态
*  输入:
*		gpio 端口,状态值
*/
SLZR_RESULT SetGPIOPin(SLZR_U32 pin, gpio_pin_e enPin,gpio_dir_e enDir)
{
//	if(SLZR_SUCCESS != GPIOGenDir(pin)) 			
//		return errno;
//	if(SLZR_SUCCESS != SetGPIODir(pin,enDir)) 	    
//		return errno;
	
	if(GPIO_DIR_IN != enDir){
		if(SLZR_SUCCESS != GPIOSetVal(pin,enPin)) 	
			return errno;
	}
//	if(GPIO_DIR_IN != enDir){
//		if(SLZR_SUCCESS !=  ClrGPIODir(pin)) 			
//			return errno;
//	}

    return SLZR_SUCCESS;
}

SLZR_RESULT InitGPIOPin(SLZR_U32 pin, gpio_pin_e enPin,gpio_dir_e enDir)
{
	if(SLZR_SUCCESS != GPIOGenDir(pin)) 			
		return errno;
	if(SLZR_SUCCESS != SetGPIODir(pin,enDir)) 	    
		return errno;
	
	if(GPIO_DIR_IN != enDir){
		if(SLZR_SUCCESS != GPIOSetVal(pin,enPin)) 	
			return errno;
	}

//	if(GPIO_DIR_IN != enDir){
//		if(SLZR_SUCCESS !=  ClrGPIODir(pin)) 			
//			return errno;
//	}
	return SLZR_SUCCESS;
}

SLZR_RESULT InitGPIOPinInt(SLZR_U32 pin, gpio_pin_e enPin,gpio_dir_e enDir)
{
	if(SLZR_SUCCESS != GPIOGenDir(pin)) 			
		return errno;
	if(SLZR_SUCCESS != SetGPIODir(pin,enDir)) 	    
		return errno;
	
	if(GPIO_DIR_IN != enDir){
		if(SLZR_SUCCESS != GPIOSetVal(pin,enPin)) 	
			return errno;
	}
	
	if(GPIO_DIR_IN != enDir){
		if(SLZR_SUCCESS !=  ClrGPIODir(pin)) 			
			return errno;
	}
	return SLZR_SUCCESS;
}

/*
*   描述:
*		获取GPIO的状态
*  输入:
*		gpio 端口,状态值(高低电平)
*/
SLZR_RESULT GetGPIOPin(SLZR_U32 pin, gpio_pin_e *penPin)
{
	char path[64];  
    char value_str[3];  
    int fd;  
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);  

    fd = open(path, O_RDONLY);  
    if (fd < 0) {  

         return ENOTINIT;
    }  
    if (read(fd, value_str, 3) < 0) {  

         return errno;
    }
    close(fd);  
	
    *(SLZR_U32 *)penPin = atoi(value_str);
	
	return SLZR_SUCCESS;
}

/* none表示引脚为输入，不是中断引脚
 * rising表示引脚为中断输入，上升沿触发
 * falling表示引脚为中断输入，下降沿触发
 * both表示引脚为中断输入，边沿触发
 * 0-->none, 1-->rising, 2-->falling, 3-->both
*/
SLZR_RESULT gpio_edge(SLZR_U32 pin, SLZR_U32 edge)
{
    const SLZR_CHAR dir_str[] = "none\0rising\0falling\0both"; 
	SLZR_U8 ptr;
	SLZR_CHAR path[64];  
    SLZR_U32 fd; 
		switch(edge){
		case 0:
			ptr = 0;
		break;
		case 1:
			ptr = 5;
			break;
		case 2:
			ptr = 12;
		break;
		case 3:
			ptr = 20;
		break;
		default:
			ptr = 0;
	} 
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/edge", pin);  
    fd = open(path, O_WRONLY);  
    if (fd < 0) {  
        DBGBOUT("Failed to open gpio edge for writing!\n");  
        return SLZR_FAILURE;  
    }  
    if (write(fd, &dir_str[ptr], strlen(&dir_str[ptr])) < 0) {  
        DBGBOUT("Failed to set edge!\n");  
        return SLZR_FAILURE;  
    }
    close(fd);  
    return SLZR_SUCCESS;  
}

/*
*  GPIO init
*/
SLZR_RESULT gpio_init(SLZR_U32 gpiopin,SLZR_U32 *fd)
{
	SLZR_CHAR path[64];
	if(GPIOGenDir(gpiopin))
	{
		DBGPOUT("gpio_gen_dir set error \n");
		return SLZR_FAILURE;
	}
	if(InitGPIOPinInt(gpiopin, GPIO_PIN_LOW,GPIO_DIR_IN))
	{
		DBGPOUT("gpio_set_dir set error \n");
		return SLZR_FAILURE;
	}
	if(gpio_edge(gpiopin,3) != 0)
	{
		DBGPOUT("gpio_edge set error \n");
		return SLZR_FAILURE;
	}
	sprintf(path, "/sys/class/gpio/gpio%d/value", gpiopin);  
    *fd = open(path,O_RDONLY);
	if(*fd < 0)
	{
		DBGPOUT("gpio_fd set error \n");
		return SLZR_FAILURE;
	}
	return SLZR_SUCCESS;
}

/*
* 描述
*        读取IO口状态
*输入
*	IO编号
*输出
*	IO口值
*/
SLZR_U32 gpio_read(SLZR_U32 pin)  
{
    char path[64];  
    char value_str[3];  
    int fd;  
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);  

    fd = open(path, O_RDONLY);  
    if (fd < 0) {  

//?        LOGOUT("Failed to open gpio value for reading!\n");  
       _DBG_("Failed to open gpio value for reading!\n");  
        return -1;  
    }  
    if (read(fd, value_str, 3) < 0) {  

//?        LOGOUT("Failed to read value!\n");  
        _DBG_("Failed to read value!\n");  
        return -1;  
    }  
    close(fd);  
    return (atoi(value_str));
} 

/*pos led set*/
SLZR_RESULT set_ic_r(int value)
{
	SLZR_RESULT ret;
	
	if(value)
		ret = SetGPIOPin(QR_R, GPIO_PIN_HI,GPIO_DIR_OUT);
	else
		ret = SetGPIOPin(QR_R, GPIO_PIN_LOW,GPIO_DIR_OUT);
	if (ret != 0){
	}

	return SLZR_SUCCESS;
}

SLZR_RESULT set_ic_g(int value)
{
	SLZR_RESULT ret;
	if(value)
		ret = SetGPIOPin(QR_G, GPIO_PIN_HI,GPIO_DIR_OUT);
	else
		ret = SetGPIOPin(QR_G, GPIO_PIN_LOW,GPIO_DIR_OUT);


	if (ret != 0){
	}

	return SLZR_SUCCESS;
}

SLZR_RESULT set_ic_b(int value)
{
	SLZR_RESULT ret;
	if(value)
		ret = SetGPIOPin(QR_B, GPIO_PIN_HI,GPIO_DIR_OUT);
	else
		ret = SetGPIOPin(QR_B, GPIO_PIN_LOW,GPIO_DIR_OUT);

	if (ret != 0){
	}

	return SLZR_SUCCESS;
}

SLZR_RESULT set_beer_b(int value)
{
	SLZR_RESULT ret;
	
	if(value)
		ret = SetGPIOPin(BEER, GPIO_PIN_HI,GPIO_DIR_OUT);
	else
		ret = SetGPIOPin(BEER, GPIO_PIN_LOW,GPIO_DIR_OUT);
		

	if (ret != 0){
	}

	return SLZR_SUCCESS;
}


/**	
*     功能:数码管控制函数
*	显示 0    1    2    3    4    5    6    7    8    9
*	buf值  0    1    2    3    4    5    6    7    8    9

*     显示 0.   1.    2.    3.    4.    5.    6.    7.    8.    9.
*    buf值  10   11   12   13   14    15   16   17    18   19

*不是上述buf值显示空
*  比如显示2.5 buf={71,71,12,5}
*  新加入的字符 
*       -    A    B    C    D    E    F     A.    B.    C.    D.    E.    F.   空 
*     20    21  22   23  24  25  26     27   28   29    30   31   32   33
*    其他显示-*     

SLZR_RESULT ShowAFLED(SLZR_U8 *pbuf)
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
	
	return SLZR_SUCCESS;
}
*/

