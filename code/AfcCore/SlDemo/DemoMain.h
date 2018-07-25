
#ifndef _INCLUDE_DemoMain_H_
#define _INCLUDE_DemoMain_H_


#ifdef __cplusplus
extern "C" {
#endif
	/***
	* @Description - Run the demo entry
	*
	* @Return int
	*/

//GPS
typedef unsigned int UINT; 
typedef int BYTE; 
typedef long int WORD; 

typedef struct __gprmc__
{
    UINT time;                  //ʱ��
    char pos_state;             //��λ״̬
    float latitude;             //γ��
    float longitude;            //����
    float speed;                //�ƶ��ٶ�
    float direction;            //����
    UINT date;                  //����
    float declination;          //��ƫ��
    char dd;                    //��ƫ�Ƿ���
    char mode;
} GPRMC;
 

//extern int open_com(char *device_name);
extern int gprmc_analysis(char *buff,GPRMC *gprmc);
extern int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
extern int GPS_main(void);

//GPS
#ifdef __cplusplus
}
#endif

#endif

