

#include "Macro_Proj.h"
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <stdlib.h>

#include "SlzrError.h"

#include "inputmisc/IcCardCtrlApi.h"
#include "inputmisc/GPIOCtrl.h"
#include "UtilityProc.h"


#define GPS_LEN 1024

typedef struct __gprmc__
{
    unsigned int time;                  //时间
    char pos_state;             //定位状态
    float latitude;             //纬度
    float longitude;            //经度
    float speed;                //移动速度
    float direction;            //方向
    unsigned int date;                  //日期
    float declination;          //磁偏角
    char dd;                    //磁偏角方向
    char mode;
} GPRMC;



GPRMC gprmc;

int gprmc_analysis(char *buff, GPRMC *gprmc)
{
	char *ptr = NULL; 
	int i, j, len, flag = 0;
	char buff_gnrmc[100];

	if(gprmc == NULL)
        return -1;

	len = strlen(buff);
    if(len < 10)
        return -1;

	for(j=0,i=0; i<len; i++)
	{
	    if(buff[i] == '$' && buff[i+1] == 'G' && buff[i+2] == 'N' && buff[i+3] == 'R' && buff[i+4] == 'M' && buff[i+5] == 'C')
	    {
	    	flag = 1;
	    }

		if(flag == 1)
		{
			buff_gnrmc[j] = buff[i];
			j++;

			if(i+2 < len)
			{
				if(buff[i+1] == '\n' || buff[i+2] == '$')
					break;
			}
		}
	}
	buff_gnrmc[j] = '\0';
//printf("\nGNRMC111:%s\n", buff_gnrmc);

    if(NULL == (ptr = strstr(buff_gnrmc,"$GNRMC")))
        return -1;
//printf("\nGNRMC222:%s\n", buff_gnrmc);
//printf("\nGNRMCpppp:%s\n", ptr);
//GNRMC222: $GNRMC,072905.000,A,2234.2030,N,11409.9442,E,0.00,0.00,100818,,,A*79
	j = sscanf(ptr,"$GNRMC,%d.000,%c,%f,N,%f,E,%f,%f,%d,,,%c*",&(gprmc->time),&(gprmc->pos_state),&(gprmc->latitude),&(gprmc->longitude),&(gprmc->speed),&(gprmc->direction),&(gprmc->date),&(gprmc->mode));
	
//printf("\nGNRMC333: %d\n", j);

	return 0;

}

#define GPS_dev_name  "ttyS2"
#define GPIO_GPS_POWER 230
void *GPS_main(void *arg)//int GPS_main(void)
{
    unsigned int fd = 0;
    int nread = 0; 


    char gps_buff[GPS_LEN];

	InitGPIOPin(GPIO_GPS_POWER,GPIO_PIN_HI,GPIO_DIR_OUT); 
	Uart_Init((unsigned int *)&fd, (unsigned char*)GPS_dev_name, 9600); 
	
    while(1)
    {
      sleep(2);
 //注意这个时间的设置，设置不恰好的话，会导致GPS数据读取不完成，数据解析出错误

      nread = UartRead(fd,(unsigned char*)gps_buff,sizeof(gps_buff));
 //     printf("gps_buff:%d: %s\r\n", nread, gps_buff);
      memset(&gprmc, 0 , sizeof(gprmc));
      gprmc_analysis(gps_buff, &gprmc); 

      if(nread != SLZR_FAILURE)
      {
 		printf("===========================================\n");

        printf("= GPS状态位 : %c  [A:有效状态 V:无效状态]\n" ,gprmc.pos_state);

        printf("= GPS模式位 : %c  [A:自主定位 D:差分定位]\n" , gprmc.mode);

        printf("=日期 : 20%02d-%02d-%02d\n",gprmc.date%100, (gprmc.date%10000)/100,gprmc.date/10000);

        printf("=时间 : %02d:%02d:%02d\n",(gprmc.time/10000+8)%24,(gprmc.time%10000)/100,gprmc.time%100);

        printf("=纬度 : 北纬:%.3f\n",(gprmc.latitude));//经度dddmm.mmmm，度分格式（前导位数不足则补0）

        printf("=经度 : 东经:%.3f\n",(gprmc.longitude));//ddmm.mmmm，度分格式（前导位数不足则补0）

        printf("=速度 : %.3f(节)\n",gprmc.speed);	//节，Knots（一节也是1.852千米／小时）
													//磁偏角，（000 - 180）度（前导位数不足则补0）

        printf("===========================================\n");

      }
    }

    close(fd);

    return 0;
}


