
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



extern GPRMC gprmc;
extern void *GPS_main(void *arg);