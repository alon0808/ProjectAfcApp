
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
    UINT time;                  //时间
    char pos_state;             //定位状态
    float latitude;             //纬度
    float longitude;            //经度
    float speed;                //移动速度
    float direction;            //方向
    UINT date;                  //日期
    float declination;          //磁偏角
    char dd;                    //磁偏角方向
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

