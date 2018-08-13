
#ifndef __FONT_HEAD_FILE_
#define __FONT_HEAD_FILE_

//#define _AREA_ENG_

#ifdef _AREA_ENG_
	#define _Font_Main_title "Card POS"
#else
	#define _Font_Main_title SYS_HEAD_STR
#endif

#ifdef _AREA_ENG_
#define _Font_Line_Str "Line"
#else
#define _Font_Line_Str "��·"
#endif

#ifdef _AREA_ENG_
#define StarPrice "StarPrice:"
#else
#define StarPrice "��:"
#endif

#ifdef _AREA_ENG_
#define _Font_PauseCard_Str "Pause card"
#else
#define _Font_PauseCard_Str "��ͣˢ��"
#endif

#ifdef _AREA_ENG_
#define _Font_Fare_Dis "Fare:%d.%02d"
#else
#define _Font_Fare_Dis "Ʊ��:%d.%02dԪ"
#endif

#ifdef _AREA_ENG_
#define _Font_Fare_Str "Fare"
#else
#define _Font_Fare_Str "Ʊ��"
#endif

#ifdef _AREA_ENG_
#define _Font_PLSinput "Input"
#else
#define _Font_PLSinput "������"
#endif

#ifdef _AREA_ENG_
#define _Font_OptionsMenu " -OptionsMenu- "
#else
#define _Font_OptionsMenu "  --���ܲ˵�--  "
#endif
#ifdef _AREA_ENG_
#define _Font_query_info "Query info"
#else
#define _Font_query_info "��ѯ��Ϣ "
#endif
#ifdef _AREA_ENG_
#define _Font_MSG_SETUP "Settings   "
#else
#define _Font_MSG_SETUP "�������� "
#endif
#ifdef _AREA_ENG_
#define _Font_PC_Cnnect "PC Connect "
#else
#define _Font_PC_Cnnect "����ͨѶ "
#endif
#ifdef _AREA_ENG_
#define _Font_Volume     "Volume    "
#else
#define _Font_Volume "�������� "
#endif
#ifdef _AREA_ENG_
#define _Font_UPData     "Upload data"
#else
#define _Font_UPData "�ϴ����� "
#endif

#ifdef _AREA_ENG_
#define _Font_Query_menu  "--Query menu--"
#else
#define _Font_Query_menu "--��ѯ�˵�--"
#endif

#ifdef _AREA_ENG_
#define _Font_Records		"Records    "
#else
#define _Font_Records "ˢ����¼ "
#endif//
#ifdef _AREA_ENG_
#define _Font_Device_Info	"Device Info"
#else
#define _Font_Device_Info "�豸��Ϣ "
#endif//
#ifdef _AREA_ENG_
#define _Font_HW_Ver		"HW Version "
#else
#define _Font_HW_Ver "�� �� �� "
#endif//
#ifdef _AREA_ENG_
#define _Font_un_upload		"un-upload  "
#else
#define _Font_un_upload "δ����¼ "
#endif//
#ifdef _AREA_ENG_
#define _Font_Statistics	"Statistics "
#else
#define _Font_Statistics "ˢ��ͳ�� "
#endif//
#ifdef _AREA_ENG_
#define _Font_Server_IP		"Server IP  "
#else
#define _Font_Server_IP "������IP "
#endif//
#ifdef _AREA_ENG_
#define _Font_Server_PORT	"Server Port "
#else
#define _Font_Server_PORT "�������˿� "
#endif//
#ifdef _AREA_ENG_
#define _Font_ShortMSG		"Messages   "
#else
#define _Font_ShortMSG "δ������ "
#endif//

#ifdef _AREA_ENG_
#define _Font_MainSet	" --SETTINGS-- "
#else
#define _Font_MainSet "  --���ò˵�--  "
#endif//
#ifdef _AREA_ENG_
#define _Font_DTset			"Date & Time  "
#else
#define _Font_DTset "����ʱ��     "
#endif//
#ifdef _AREA_ENG_
#define _Font_YDIPset		"CMCC IP addr "
#else
#define _Font_YDIPset "�ƶ�IP����   "
#endif//
#ifdef _AREA_ENG_
#define _Font_GJIPset		"Server IP    "
#else
#define _Font_GJIPset "����������   "
#endif//
#ifdef _AREA_ENG_
#define _Font_KEKDown		"KEK Download "
#else
#define _Font_KEKDown "KEK����      "
#endif//
#ifdef _AREA_ENG_
#define _Font_CLRBLK		"Clear BLK LST"
#else
#define _Font_CLRBLK "���ú�����   "
#endif//
#ifdef _AREA_ENG_
#define _Font_SetDEV		"Device No.   "
#else
#define _Font_SetDEV "�����豸��   "
#endif//
#ifdef _AREA_ENG_
#define _Font_Load_Key		"Load Key     "
#else
#define _Font_Load_Key "װ����Կ     "
#endif//
#ifdef _AREA_ENG_
#define _Font_WIFI_SSID		"WIFI SSID    "
#else
#define _Font_WIFI_SSID "��������ID   "
#endif//
#ifdef _AREA_ENG_
#define _Font_WIFI_PSW		"WIFI Password"
#else
#define _Font_WIFI_PSW "����������Կ "
#endif//
#ifdef _AREA_ENG_
#define _Font_ResetPos		"Reset POS    "
#else
#define _Font_ResetPos "�ָ�����״̬ "
#endif//

#ifdef _AREA_ENG_
#define _Font_InputCODE	"Input code"
#else
#define _Font_InputCODE "��������"
#endif//
#ifdef _AREA_ENG_
#define _Font_Inputerror "Password wrong"
#else
#define _Font_Inputerror "�������"
#endif//
#ifdef _AREA_ENG_
#define _Font_swipeCard "pls swipe"
#else
#define _Font_swipeCard "��ˢ��"
#endif//
#ifdef _AREA_ENG_
#define _Font_Rs232Debug "Debug info COM"
#else
#define _Font_Rs232Debug "���������Ϣ"
#endif//
#ifdef _AREA_ENG_
#define _Font_HitDebuginfo "Debug info COM"
#else
#define _Font_HitDebuginfo "Need to connect PC-COM, use the serial port monitor."
#endif//
#ifdef _AREA_ENG_
#define _Font_Complete " Complete "
#else
#define _Font_Complete "���!"
#endif//
#ifdef _AREA_ENG_
#define _Font_ICcard "IC card:%d"
#else
#define _Font_ICcard "IC����:%d"
#endif//
#ifdef _AREA_ENG_
#define _Font_RECqurey "-Records query-"
#else
#define _Font_RECqurey "--��¼��ѯ--"
#endif//
#ifdef _AREA_ENG_
#define _Font_NOREC "No records"
#else
#define _Font_NOREC "��ˢ����¼"
#endif//
#ifdef _AREA_ENG_
#define _Font_findREC "find record..."
#else
#define _Font_findREC "���Ҽ�¼��..."
#endif//

#ifdef _AREA_ENG_
#define _Font_showCardNo "No."
#else
#define _Font_showCardNo "����"
#endif//
#ifdef _AREA_ENG_
#define _Font_showCardtype "type"
#else
#define _Font_showCardtype "����"
#endif//
#ifdef _AREA_ENG_
#define _Font_KouKuandian "Deduct:%d.%02d "
#else
#define _Font_KouKuandian "�ۿ�:%d.%02d "
#endif//
#ifdef _AREA_ENG_
#define _Font_Balancedian "Balance:%d.%02d"
#else
#define _Font_Balancedian "���:%d.%02d"
#endif//
#ifdef _AREA_ENG_
#define _Font_KouCIdian "Deduct:%d "
#else
#define _Font_KouCIdian "�۴�:%d "
#endif//
#ifdef _AREA_ENG_
#define _Font_BalanceCI "Balance:%d"
#else
#define _Font_BalanceCI "���:%d"
#endif//
#ifdef _AREA_ENG_
#define _Font_SwipTimes "Times:%d"
#else
#define _Font_SwipTimes  "ˢ����:%d�� "
#endif//

#ifdef _AREA_ENG_
#define Card_Name_white "White card"
#else
#define Card_Name_white "�׿�"
#endif
#ifdef _AREA_ENG_
#define Card_Name_NORMAL "Normal card"
#else
#define Card_Name_NORMAL "��ͨ��"
#endif
#ifdef _AREA_ENG_
#define Card_Name_STUDENT "Student card"
#else
#define Card_Name_STUDENT "ѧ����"
#endif
#ifdef _AREA_ENG_
#define Card_Name_EXPERT "Expert card"
#else
#define Card_Name_EXPERT "ר�ҿ�"
#endif
#ifdef _AREA_ENG_
#define Card_Name_ZILU "Children card"
#else
#define Card_Name_ZILU "��Ů��"
#endif
#ifdef _AREA_ENG_
#define Card_Name_ZUL "Staff card"
#else
#define Card_Name_ZUL "Ա����"
#endif
#ifdef _AREA_ENG_
#define Card_Name_OLDMAN "Elder card"
#else
#define Card_Name_OLDMAN "���꿨"
#endif
#ifdef _AREA_ENG_
#define Card_Name_COUPON "Dis-coupon card"
#else
#define Card_Name_COUPON "���ſ�"
#endif
#ifdef _AREA_ENG_
#define Card_Name_STUFF "Staff card"
#else
#define Card_Name_STUFF "Ա����"
#endif
#ifdef _AREA_ENG_
#define Card_Name_RELATION "Relative card"
#else
#define Card_Name_RELATION "������"
#endif
#ifdef _AREA_ENG_
#define Card_Name_PRACTISE "Trainee card"
#else
#define Card_Name_PRACTISE "ѧԱ��"
#endif
#ifdef _AREA_ENG_
#define Card_Name_YOUDAI "Coutesy card"
#else
#define Card_Name_YOUDAI "�Ŵ���"
#endif
#ifdef _AREA_ENG_
#define Card_Name_JILIAN "Commemorative card"
#else
#define Card_Name_JILIAN "���"
#endif
#ifdef _AREA_ENG_
#define	Card_Name_YOUHUI "Coupon card"
#else
#define	Card_Name_YOUHUI "�Żݿ�"
#endif
#ifdef _AREA_ENG_
#define	Card_Name_Driver "Driver card"
#else
#define	Card_Name_Driver "˾����"
#endif
#ifdef _AREA_ENG_
#define	Card_Grey_Record "GreyRecord card"
#else
#define	Card_Grey_Record "�Ҽ�¼"
#endif
#ifdef _AREA_ENG_
#define	Card_Set_Line  "SetLine card"
#else
#define	Card_Set_Line  "��·����"
#endif
#ifdef _AREA_ENG_
#define	Card_Name_departcard "Depart card"
#else
#define	Card_Name_departcard "������"
#endif
#ifdef _AREA_ENG_
#define	Card_Name_stopcard "Stop card"
#else
#define	Card_Name_stopcard "��վ��"
#endif
#ifdef _AREA_ENG_
#define	Card_Name_Blkcard "Black card"
#else
#define	Card_Name_Blkcard "������"
#endif
#ifdef _AREA_ENG_
#define	Card_Name_ZWYcard "Manager card"
#else
#define	Card_Name_ZWYcard "վ��Ա��"
#endif

#ifdef _AREA_ENG_
#define	Card_deviceNO "NO."
#else
#define	Card_deviceNO "�豸��:"
#endif

#ifdef _AREA_ENG_
#define	Card_SerialNO "Serial:"
#else
#define	Card_SerialNO "���к�:"
#endif

#ifdef _AREA_ENG_
#define	Card_LoadKey "Load Key"
#else
#define	Card_LoadKey "װ����Կ"
#endif
#ifdef _AREA_ENG_
#define	Card_SWKeyCard "Swipe Key card"
#else
#define	Card_SWKeyCard "��ˢ���뿨"
#endif
#ifdef _AREA_ENG_
#define	Card_getCardinfo "Get Card info"
#else
#define	Card_getCardinfo "ȡ����Ϣ"
#endif
#ifdef _AREA_ENG_
#define	Card_CardtypeE "Card Incorrect"
#else
#define	Card_CardtypeE "������Կ��!"
#endif
#ifdef _AREA_ENG_
#define	Card_AuthenticateErr "Authenticate Err"
#else
#define	Card_AuthenticateErr "��֤����!"
#endif

#ifdef _AREA_ENG_
#define	Card_LoadKEyOK "Load Key complete"
#else
#define	Card_LoadKEyOK "װ����Կ���"
#endif

#ifdef _AREA_ENG_
#define	Card_Resetpos " Reset POS "
#else
#define	Card_Resetpos "�ָ�������״̬"
#endif

#ifdef _AREA_ENG_
#define	Card_HitResetpos "Will erase all records and parameters"
#else
#define	Card_HitResetpos "��������м�¼�Ͳ���!"
#endif
#ifdef _AREA_ENG_
#define	Card_HitSure "Are you sure?"
#else
#define	Card_HitSure "ȷ����?"
#endif
#ifdef _AREA_ENG_
#define	Card_Welcome "Welcome!"
#else
#define	Card_Welcome "��ӭ�˳�!"
#endif

#ifdef _AREA_ENG_
#define	Card_Worring "Warning:"
#else
#define	Card_Worring "����:"
#endif
#ifdef _AREA_ENG_
#define	Card_NotBzcard "Warning:"
#else
#define	Card_NotBzcard "�Ǳ��̻���!"
#endif
#ifdef _AREA_ENG_
#define	Please_Coin "Please Coin!"
#else
#define	Please_Coin "��Ͷ��!"
#endif
#ifdef _AREA_ENG_
#define	Expired_card "Expired card!"
#else
#define	Expired_card "�����ڻ�δ����!"
#endif
#ifdef _AREA_ENG_
#define	Card_Carderror "Card info Error"
#else
#define	Card_Carderror "������,�뵽��ֵ��ָ�."
#endif

#ifdef _AREA_ENG_
#define	Card_forbidCard "Forbidden"
#else
#define	Card_forbidCard "�˿���ֹˢ��!"
#endif
#ifdef _AREA_ENG_
#define	Card_NodealCard "Forbid Card"
#else
#define	Card_NodealCard "����ʱ����ˢ�˿�"
#endif

#ifdef _AREA_ENG_
#define	PLS_Swipe_again "swipe again"
#else
#define	PLS_Swipe_again " �� �� ˢ "
#endif

#ifdef _AREA_ENG_
#define	PLS_Swipe_OK "swipe OK"
#else
#define	PLS_Swipe_OK " ˢ����� "
#endif
#ifdef _AREA_ENG_
#define	EEisFull "eeprom full!"
#else
#define	EEisFull "���ϴ�����!"
#endif

#ifdef _AREA_ENG_
#define	ModyfiDataTime "correction time!"
#else
#define	ModyfiDataTime "�޸�ʱ��"
#endif

#ifdef _AREA_ENG_
#define	InputDataTime "Input Datetime"
#else
#define	InputDataTime " ��������ȷʱ�� "
#endif

#ifdef _AREA_ENG_
#define	PriceErr "price Err"
#else
#define	PriceErr " Ʊ�۴��� "
#endif

#ifdef _AREA_ENG_
#define	LineInfoErr "LineInfo Err"
#else
#define	LineInfoErr " ��·��Ϣ���� "
#endif

#ifdef _AREA_ENG_
#define	PositionErr "Position Err"
#else
#define	PositionErr " ��λ��Ϣ���� "
#endif

#ifdef _AREA_ENG_
#define	CheckVer "Checking Version"
#else
#define	CheckVer "���Ұ汾��"
#endif

#ifdef _AREA_ENG_
#define	UpData "UpDateing...."
#else
#define	UpData "���²���..."
#endif

#ifdef _AREA_ENG_
#define	UpDataLine "UpDate line"
#else
#define	UpDataLine "������·��Ϣ"
#endif

#ifdef _AREA_ENG_
#define	STOP "STOP!"
#else
#define	STOP "��ͣˢ��..."
#endif

#ifdef _AREA_ENG_
#define	SwipOff "Swip Card PLS!"
#else
#define	SwipOff "�³���ˢ��"
#endif

#ifdef _AREA_ENG_
#define	DeviceType "Device Type"
#else
#define	DeviceType "�豸����"
#endif

#ifdef _AREA_ENG_
#define	SetDevice "Set Device Type"
#else
#define	SetDevice "�����豸����"
#endif

#ifdef _AREA_ENG_
#define	FronDevice "Front Device"
#else
#define	FronDevice "ǰ�Ż�"
#endif

#ifdef _AREA_ENG_
#define	BackDevice "Back Device"
#else
#define	BackDevice "���Ż�"
#endif

#ifdef _AREA_ENG_
#define	Successfull "Successfull"
#else
#define	Successfull "���óɹ�"
#endif

#ifdef _AREA_ENG_
#define	deductMoneyErr "Debit fails"
#else
#define	deductMoneyErr "Ǯ������ʧ��"
#endif

#ifdef _AREA_ENG_
#define	DontSwipAgain "Swip Already"
#else
#define	DontSwipAgain "�Ѿ�ˢ����"
#endif

#ifdef _AREA_ENG_
#define	OnBus " On Bus"
#else
#define	OnBus "�ϳ�"
#endif

#ifdef _AREA_ENG_
#define	AlsoNtime "Also Swip%dTimes"
#else
#define	AlsoNtime "��Ҫˢ%d��"
#endif

#ifdef _AREA_ENG_
#define	NoSame "No the same card"
#else
#define	NoSame "����ͬһ�ſ�"
#endif

#ifdef _AREA_ENG_
#define	SwipSameCard "PLS Swip the Same Card"
#else
#define	SwipSameCard "��ˢͬһ�ſ�"
#endif

#ifdef _AREA_ENG_
#define	SwipAgain "PLS Swip Again"
#else
#define	SwipAgain "���ٴ�ˢ��"
#endif

#ifdef _AREA_ENG_
#define	CanErr "Can Connection Error"
#else
#define	CanErr "CAN ͨѶ����"
#endif

#ifdef _AREA_ENG_
#define	AnOtherErr "AnOtherErr"
#else
#define	AnOtherErr "��������"
#endif

#ifdef _AREA_ENG_
#define	Err "Err"
#else
#define	Err "����"
#endif

#ifdef _AREA_ENG_
#define	NoStationIfo "No Station Ifo"
#else
#define	NoStationIfo "û��վ����Ϣ"
#endif

#define Conn(x,y) x##y




#endif

