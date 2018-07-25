
#ifndef _INCLUDE_LIGHTCOLOR_H_
#define _INCLUDE_LIGHTCOLOR_H_


#ifdef _cplusplus
extern "C"
{
#endif


#define SCREEN_LIGHT	1	/*��Ļ�Ϸ���*/
#define QR_LIGHT		2	/*ɨ������*/
#define LED_LIGHT		3	/*��LED��*/

#define LIGHT_OPEN		0	/*�򿪵�*/
#define LIGHT_CLOSE 	1	/*�رյ�*/


#define gpio_pin_e int
#define gpio_dir_e int

/*
* ��ά��ɨ��ܽ�
*/
#define QR_R		192		/*ɨ������----��ɫ*/
#define QR_G		193		/*ɨ������----��ɫ*/
#define QR_B		194		/*ɨ������----��ɫ*/
#define QR_W		195		/*ɨ������----��ɫ*/
#define QR_BRIGHT	34		/*ɨ������----����, PWMռ�ձ�Խ��,����Խ��*/
#define QR_OPEN		35		/*ɨ������----�ߵ�ƽ������Դ*/


#define SCREEN_G	41		/*��Ļ�Ϸ���----��ɫ*/
#define SCREEN_B	42		/*��Ļ�Ϸ���----��ɫ*/
#define SCREEN_R	53		/*��Ļ�Ϸ���----��ɫ*/
#define SCREEN_OPEN	247		/*��Ļ�Ϸ���----�ߵ�ƽ������Դ*/


#define IO_IN_NUM 20

#define GPIO_PIN_HI		0	/*�� �ߵ�ƽ*/
#define GPIO_PIN_LOW	1	/*�� �͵�ƽ*/

#define GPIO_DIR_IN		1	/*����*/
#define GPIO_DIR_OUT	2	/*�Ƴ�*/


#define Invaild_Code		1	/*����:��Ч��*/
#define Fail_OpenDoor		2	/*����:���Ź���*/
#define Fail_CountryCode	3	/*����:���д��벻��*/
#define Re_QWCode			4	/*����:ˢ�¶�ά��*/
#define NoAgain_Card		5	/*����:������ˢ*/
#define WelCome				6	/*����:��ӭ����*/
#define FrontDoor_Down		7	/*����:��ǰ���ϳ�*/
#define Thanks_Again		8	/*����:лл,��ӭ�ٴγ���*/


extern int Light_main(int LightChoose, int on_off, int pin, char *buff);
extern void SetR485_EN(int on_off);

extern 
void Voice_main(int VoiceChoose);
extern int KeyBoard_main(int *value);

#ifdef _cplusplus
}
#endif


#endif
