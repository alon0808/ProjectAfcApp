

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
    unsigned int time;                  //ʱ��
    char pos_state;             //��λ״̬
    float latitude;             //γ��
    float longitude;            //����
    float speed;                //�ƶ��ٶ�
    float direction;            //����
    unsigned int date;                  //����
    float declination;          //��ƫ��
    char dd;                    //��ƫ�Ƿ���
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
 //ע�����ʱ������ã����ò�ǡ�õĻ����ᵼ��GPS���ݶ�ȡ����ɣ����ݽ���������

      nread = UartRead(fd,(unsigned char*)gps_buff,sizeof(gps_buff));
 //     printf("gps_buff:%d: %s\r\n", nread, gps_buff);
      memset(&gprmc, 0 , sizeof(gprmc));
      gprmc_analysis(gps_buff, &gprmc); 

      if(nread != SLZR_FAILURE)
      {
 		printf("===========================================\n");

        printf("= GPS״̬λ : %c  [A:��Ч״̬ V:��Ч״̬]\n" ,gprmc.pos_state);

        printf("= GPSģʽλ : %c  [A:������λ D:��ֶ�λ]\n" , gprmc.mode);

        printf("=���� : 20%02d-%02d-%02d\n",gprmc.date%100, (gprmc.date%10000)/100,gprmc.date/10000);

        printf("=ʱ�� : %02d:%02d:%02d\n",(gprmc.time/10000+8)%24,(gprmc.time%10000)/100,gprmc.time%100);

        printf("=γ�� : ��γ:%.3f\n",(gprmc.latitude));//����dddmm.mmmm���ȷָ�ʽ��ǰ��λ��������0��

        printf("=���� : ����:%.3f\n",(gprmc.longitude));//ddmm.mmmm���ȷָ�ʽ��ǰ��λ��������0��

        printf("=�ٶ� : %.3f(��)\n",gprmc.speed);	//�ڣ�Knots��һ��Ҳ��1.852ǧ�ף�Сʱ��
													//��ƫ�ǣ���000 - 180���ȣ�ǰ��λ��������0��

        printf("===========================================\n");

      }
    }

    close(fd);

    return 0;
}


