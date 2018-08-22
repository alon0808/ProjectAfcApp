
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



extern GPRMC gprmc;
extern void *GPS_main(void *arg);