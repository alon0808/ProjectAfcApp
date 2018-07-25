#include "Macro_Proj.h"

#include "DemoMain.h"

#include "ProcCharacter.h"
#include "xSysApi_LanTai.h"

//GPS
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <stdlib.h>

#include "inputmisc/IcCardCtrlApi.h"


#define GPS_LEN 512


int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio; 

    if( tcgetattr( fd,&oldtio)  !=  0)
    {
        perror("SetupSerial 1");
        return -1;
    }

    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE; 

    switch( nBits )
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    } 

    switch( nEvent )
    {
        case 'O':                     //奇校验       
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':                     //偶校验      
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':
            newtio.c_cflag &= ~PARENB;
            break;
	}

	switch( nSpeed )
    {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    } 

    if( nStop == 1 )
    {
        newtio.c_cflag &=  ~CSTOPB;
    }
    else if ( nStop == 2 )
    {
        newtio.c_cflag |=  CSTOPB;
    }
	
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH); 

    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }

    return 0;
}


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
	   // if(memcat(&buff[i], "$GNRMC", 6) == 0)
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
//printf("\nGNRMC111: %s\n", buff_gnrmc);

    if(NULL == (ptr = strstr(buff_gnrmc,"$GNRMC")))        
		return -1;
//printf("\nGNRMC222: %s\n", buff_gnrmc);
	sscanf(ptr,"$GNRMC,%d.000,%c,%f,N,%f,E,%f,%f,%d,,,%c*",&(gprmc->time),&(gprmc->pos_state),&(gprmc->latitude),&(gprmc->longitude),&(gprmc->speed),&(gprmc->direction),&(gprmc->date),&(gprmc->mode));

	return 0;

}


int GPS_main(void)
{
    int fd = 0;
    int nread = 0; 

    GPRMC gprmc; 

    char gps_buff[GPS_LEN];
    char *dev_name = "/dev/ttyS2"; 

    //fd = open_com(dev_name); 

	fd = open(dev_name, O_RDONLY);  

    set_opt(fd,9600,8,'N',1);

    while(1)
    {
      sleep(2);
 //注意这个时间的设置，设置不恰好的话，会导致GPS数据读取不完成，数据解析出错误

      nread = read(fd,gps_buff,sizeof(gps_buff));
      printf("gps_buff: %s", gps_buff);
      memset(&gprmc, 0 , sizeof(gprmc));
      gprmc_analysis(gps_buff, &gprmc); 

      if(nread > 0)
      {
 		printf("===========================================\n");

        printf("= GPS状态位 : %c  [A:有效状态 V:无效状态]==\n" ,gprmc.pos_state);

        printf("= GPS模式位 : %c  [A:自主定位 D:差分定位]==\n" , gprmc.mode);

        printf("=日期 : 20%02d-%02d-%02d=\n",gprmc.date%100, (gprmc.date%10000)/100,gprmc.date/10000);

        printf("=时间 : %02d:%02d:%02d=\n",(gprmc.time/10000+8)%24,(gprmc.time%10000)/100,gprmc.time%100);

        printf("=纬度 : 北纬:%.3f=\n",(gprmc.latitude/100));

        printf("=经度 : 东经:%.3f=\n",(gprmc.longitude/100));

        printf("=速度 : %.3f =\n",gprmc.speed);

        printf("===========================================\n");

      }
    }

    close(fd);

    return 0;
}

//GPS

