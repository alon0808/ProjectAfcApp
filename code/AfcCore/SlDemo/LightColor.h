
#ifndef _INCLUDE_LIGHTCOLOR_H_
#define _INCLUDE_LIGHTCOLOR_H_


#ifdef _cplusplus
extern "C"
{
#endif


#define SCREEN_LIGHT	1	/*屏幕上方灯*/
#define QR_LIGHT		2	/*扫码区灯*/
#define LED_LIGHT		3	/*后背LED灯*/

#define LIGHT_OPEN		0	/*打开灯*/
#define LIGHT_CLOSE 	1	/*关闭灯*/


#define gpio_pin_e int
#define gpio_dir_e int

/*
* 二维码扫码管脚
*/
#define QR_R		192		/*扫码区灯----红色*/
#define QR_G		193		/*扫码区灯----绿色*/
#define QR_B		194		/*扫码区灯----蓝色*/
#define QR_W		195		/*扫码区灯----白色*/
#define QR_BRIGHT	34		/*扫码区灯----亮度, PWM占空比越高,亮度越亮*/
#define QR_OPEN		35		/*扫码区灯----高电平开启电源*/


#define SCREEN_G	41		/*屏幕上方灯----绿色*/
#define SCREEN_B	42		/*屏幕上方灯----蓝色*/
#define SCREEN_R	53		/*屏幕上方灯----红色*/
#define SCREEN_OPEN	247		/*屏幕上方灯----高电平开启电源*/


#define IO_IN_NUM 20

#define GPIO_PIN_HI		0	/*置 高电平*/
#define GPIO_PIN_LOW	1	/*置 低电平*/

#define GPIO_DIR_IN		1	/*拉进*/
#define GPIO_DIR_OUT	2	/*推出*/


#define Invaild_Code		1	/*声音:无效码*/
#define Fail_OpenDoor		2	/*声音:开门故障*/
#define Fail_CountryCode	3	/*声音:城市代码不符*/
#define Re_QWCode			4	/*声音:刷新二维码*/
#define NoAgain_Card		5	/*声音:请勿重刷*/
#define WelCome				6	/*声音:欢迎乘坐*/
#define FrontDoor_Down		7	/*声音:从前门上车*/
#define Thanks_Again		8	/*声音:谢谢,欢迎再次乘坐*/


extern int Light_main(int LightChoose, int on_off, int pin, char *buff);
extern void SetR485_EN(int on_off);

extern 
void Voice_main(int VoiceChoose);
extern int KeyBoard_main(int *value);

#ifdef _cplusplus
}
#endif


#endif
