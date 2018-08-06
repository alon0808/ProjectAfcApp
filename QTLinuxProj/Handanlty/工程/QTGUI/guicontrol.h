#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include <stdio.h>

#include "cJSON.h"
#include "LtyTypeDef.h"
#include "LtyMsgItsIO.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct Rs485State
{
	int chnl;
	int state;
	char cameraID[9];
}RS485STATE;





typedef struct QrPrice_s
{
	int tickettype;
	int ticketprice;
}TQrPrice,*PTQrPrice;

typedef struct flash_card_result_s{
	LTY_U8 u8Type;
	LTY_CHAR money[15];
	LTY_CHAR money_total[15];
	LTY_CHAR money_qr[15];
	LTY_CHAR money_total_qr[15];
	LTY_CHAR fail_result[100];
	LTY_S32 show_time; //显示时间
	LTY_U8 message_tip_type;
	LTY_U8 card_type;
	LTY_CHAR card_number[100];
	LTY_U16 ic_number;
	LTY_U16 qr_number;
}flash_card_result_t;

enum NETWORK_TYPE
{
   E_NETWORK_TYPE_G3 = 1,
   E_NETWORK_TYPE_TMP,
   E_NETWORK_TYPE_OTHER
};

enum TMP_TYPE
{
   E_TMP_TYPE_LOGIN = 1,
   E_TMP_TYPE_CONN,
   E_TMP_TYPE_OTHER
};

/*2.1 刷卡结果*/ 
#define CARD_FLASH_SUCCESS		1	/*刷卡成功*/ 
#define CARD_FLASH_FAIL			2	/*刷卡失败*/
#define CARD_FLASH_ABNORMAL		3	/*刷卡异常*/
#define CARD_FLASH_BALANCE_LOW	4	/*余额不足*/
#define CARD_FLASH_MESSAGE		5	/*自定义消息*/
#define QR_SCAN_SUCCESS			6	/*扫码成功*/
#define QR_SCAN_FAILED			7	/*验码失败*/
#define QR_SCAN_ABNORMAL		8	/*扫码异常*/


int ltySendMessageToUi(cJSON *root,char _type, void *pStream);
int start_init_gui(int argc, const char* argv[], void* pArg);
int ApcAlgStart();

#define MASTER_TYPE 1
#define SLAVE_TYPE  2
#define NET_LINKEXCEP 0
#define NET_LINKOK  1

void gui_system_tbp_connect_status(char _status,char _type);
void gui_ic_card_pay_info(void *_param);
void gui_flahs_card_info(void *_param);
void gui_system_maintain_get(char _type,char _main_type,char _operate_type, 
                                      char _percent,const char *_failed_result);
void gui_passenger_flow_info(void *_param, itsio_passenger_flow_t passenger);
	
void gui_system_starting_info_set(cJSON *root);
void starting_info_common_set(cJSON *root);
void gui_starting_screen_flag_set(cJSON *root);
void gui_g3_network_get(char _status,char _type);
void starting_screen_flag_set(cJSON *root); 	
void gui_starting_screen_flag_get();
void gui_tmp_network_get(LTY_U32 _status, LTY_U32 _type, LTY_U32 errorTtpe);
void gui_Alg_Image_get(LTY_U32 _oprType);
void gui_system_pos_config_get();
void gui_station_and_line_set(cJSON *root);



#ifdef __cplusplus
}
#endif


void CheckMouse(void);


#endif

