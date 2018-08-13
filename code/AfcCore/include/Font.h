
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
#define _Font_Line_Str "线路"
#endif

#ifdef _AREA_ENG_
#define StarPrice "StarPrice:"
#else
#define StarPrice "起步:"
#endif

#ifdef _AREA_ENG_
#define _Font_PauseCard_Str "Pause card"
#else
#define _Font_PauseCard_Str "暂停刷卡"
#endif

#ifdef _AREA_ENG_
#define _Font_Fare_Dis "Fare:%d.%02d"
#else
#define _Font_Fare_Dis "票价:%d.%02d元"
#endif

#ifdef _AREA_ENG_
#define _Font_Fare_Str "Fare"
#else
#define _Font_Fare_Str "票价"
#endif

#ifdef _AREA_ENG_
#define _Font_PLSinput "Input"
#else
#define _Font_PLSinput "请输入"
#endif

#ifdef _AREA_ENG_
#define _Font_OptionsMenu " -OptionsMenu- "
#else
#define _Font_OptionsMenu "  --功能菜单--  "
#endif
#ifdef _AREA_ENG_
#define _Font_query_info "Query info"
#else
#define _Font_query_info "查询信息 "
#endif
#ifdef _AREA_ENG_
#define _Font_MSG_SETUP "Settings   "
#else
#define _Font_MSG_SETUP "参数设置 "
#endif
#ifdef _AREA_ENG_
#define _Font_PC_Cnnect "PC Connect "
#else
#define _Font_PC_Cnnect "串口通讯 "
#endif
#ifdef _AREA_ENG_
#define _Font_Volume     "Volume    "
#else
#define _Font_Volume "音量调整 "
#endif
#ifdef _AREA_ENG_
#define _Font_UPData     "Upload data"
#else
#define _Font_UPData "上传数据 "
#endif

#ifdef _AREA_ENG_
#define _Font_Query_menu  "--Query menu--"
#else
#define _Font_Query_menu "--查询菜单--"
#endif

#ifdef _AREA_ENG_
#define _Font_Records		"Records    "
#else
#define _Font_Records "刷卡记录 "
#endif//
#ifdef _AREA_ENG_
#define _Font_Device_Info	"Device Info"
#else
#define _Font_Device_Info "设备信息 "
#endif//
#ifdef _AREA_ENG_
#define _Font_HW_Ver		"HW Version "
#else
#define _Font_HW_Ver "版 本 号 "
#endif//
#ifdef _AREA_ENG_
#define _Font_un_upload		"un-upload  "
#else
#define _Font_un_upload "未传记录 "
#endif//
#ifdef _AREA_ENG_
#define _Font_Statistics	"Statistics "
#else
#define _Font_Statistics "刷卡统计 "
#endif//
#ifdef _AREA_ENG_
#define _Font_Server_IP		"Server IP  "
#else
#define _Font_Server_IP "服务器IP "
#endif//
#ifdef _AREA_ENG_
#define _Font_Server_PORT	"Server Port "
#else
#define _Font_Server_PORT "服务器端口 "
#endif//
#ifdef _AREA_ENG_
#define _Font_ShortMSG		"Messages   "
#else
#define _Font_ShortMSG "未发短信 "
#endif//

#ifdef _AREA_ENG_
#define _Font_MainSet	" --SETTINGS-- "
#else
#define _Font_MainSet "  --设置菜单--  "
#endif//
#ifdef _AREA_ENG_
#define _Font_DTset			"Date & Time  "
#else
#define _Font_DTset "设置时间     "
#endif//
#ifdef _AREA_ENG_
#define _Font_YDIPset		"CMCC IP addr "
#else
#define _Font_YDIPset "移动IP设置   "
#endif//
#ifdef _AREA_ENG_
#define _Font_GJIPset		"Server IP    "
#else
#define _Font_GJIPset "服务器设置   "
#endif//
#ifdef _AREA_ENG_
#define _Font_KEKDown		"KEK Download "
#else
#define _Font_KEKDown "KEK下载      "
#endif//
#ifdef _AREA_ENG_
#define _Font_CLRBLK		"Clear BLK LST"
#else
#define _Font_CLRBLK "重置黑名单   "
#endif//
#ifdef _AREA_ENG_
#define _Font_SetDEV		"Device No.   "
#else
#define _Font_SetDEV "设置设备号   "
#endif//
#ifdef _AREA_ENG_
#define _Font_Load_Key		"Load Key     "
#else
#define _Font_Load_Key "装载密钥     "
#endif//
#ifdef _AREA_ENG_
#define _Font_WIFI_SSID		"WIFI SSID    "
#else
#define _Font_WIFI_SSID "设置无线ID   "
#endif//
#ifdef _AREA_ENG_
#define _Font_WIFI_PSW		"WIFI Password"
#else
#define _Font_WIFI_PSW "设置无线密钥 "
#endif//
#ifdef _AREA_ENG_
#define _Font_ResetPos		"Reset POS    "
#else
#define _Font_ResetPos "恢复出厂状态 "
#endif//

#ifdef _AREA_ENG_
#define _Font_InputCODE	"Input code"
#else
#define _Font_InputCODE "输入密码"
#endif//
#ifdef _AREA_ENG_
#define _Font_Inputerror "Password wrong"
#else
#define _Font_Inputerror "输入错误"
#endif//
#ifdef _AREA_ENG_
#define _Font_swipeCard "pls swipe"
#else
#define _Font_swipeCard "请刷卡"
#endif//
#ifdef _AREA_ENG_
#define _Font_Rs232Debug "Debug info COM"
#else
#define _Font_Rs232Debug "串口输出信息"
#endif//
#ifdef _AREA_ENG_
#define _Font_HitDebuginfo "Debug info COM"
#else
#define _Font_HitDebuginfo "Need to connect PC-COM, use the serial port monitor."
#endif//
#ifdef _AREA_ENG_
#define _Font_Complete " Complete "
#else
#define _Font_Complete "完成!"
#endif//
#ifdef _AREA_ENG_
#define _Font_ICcard "IC card:%d"
#else
#define _Font_ICcard "IC卡共:%d"
#endif//
#ifdef _AREA_ENG_
#define _Font_RECqurey "-Records query-"
#else
#define _Font_RECqurey "--记录查询--"
#endif//
#ifdef _AREA_ENG_
#define _Font_NOREC "No records"
#else
#define _Font_NOREC "无刷卡记录"
#endif//
#ifdef _AREA_ENG_
#define _Font_findREC "find record..."
#else
#define _Font_findREC "查找记录中..."
#endif//

#ifdef _AREA_ENG_
#define _Font_showCardNo "No."
#else
#define _Font_showCardNo "卡号"
#endif//
#ifdef _AREA_ENG_
#define _Font_showCardtype "type"
#else
#define _Font_showCardtype "卡类"
#endif//
#ifdef _AREA_ENG_
#define _Font_KouKuandian "Deduct:%d.%02d "
#else
#define _Font_KouKuandian "扣款:%d.%02d "
#endif//
#ifdef _AREA_ENG_
#define _Font_Balancedian "Balance:%d.%02d"
#else
#define _Font_Balancedian "余额:%d.%02d"
#endif//
#ifdef _AREA_ENG_
#define _Font_KouCIdian "Deduct:%d "
#else
#define _Font_KouCIdian "扣次:%d "
#endif//
#ifdef _AREA_ENG_
#define _Font_BalanceCI "Balance:%d"
#else
#define _Font_BalanceCI "余次:%d"
#endif//
#ifdef _AREA_ENG_
#define _Font_SwipTimes "Times:%d"
#else
#define _Font_SwipTimes  "刷卡量:%d次 "
#endif//

#ifdef _AREA_ENG_
#define Card_Name_white "White card"
#else
#define Card_Name_white "白卡"
#endif
#ifdef _AREA_ENG_
#define Card_Name_NORMAL "Normal card"
#else
#define Card_Name_NORMAL "普通卡"
#endif
#ifdef _AREA_ENG_
#define Card_Name_STUDENT "Student card"
#else
#define Card_Name_STUDENT "学生卡"
#endif
#ifdef _AREA_ENG_
#define Card_Name_EXPERT "Expert card"
#else
#define Card_Name_EXPERT "专家卡"
#endif
#ifdef _AREA_ENG_
#define Card_Name_ZILU "Children card"
#else
#define Card_Name_ZILU "子女卡"
#endif
#ifdef _AREA_ENG_
#define Card_Name_ZUL "Staff card"
#else
#define Card_Name_ZUL "员工卡"
#endif
#ifdef _AREA_ENG_
#define Card_Name_OLDMAN "Elder card"
#else
#define Card_Name_OLDMAN "老年卡"
#endif
#ifdef _AREA_ENG_
#define Card_Name_COUPON "Dis-coupon card"
#else
#define Card_Name_COUPON "残优卡"
#endif
#ifdef _AREA_ENG_
#define Card_Name_STUFF "Staff card"
#else
#define Card_Name_STUFF "员工卡"
#endif
#ifdef _AREA_ENG_
#define Card_Name_RELATION "Relative card"
#else
#define Card_Name_RELATION "家属卡"
#endif
#ifdef _AREA_ENG_
#define Card_Name_PRACTISE "Trainee card"
#else
#define Card_Name_PRACTISE "学员卡"
#endif
#ifdef _AREA_ENG_
#define Card_Name_YOUDAI "Coutesy card"
#else
#define Card_Name_YOUDAI "优待卡"
#endif
#ifdef _AREA_ENG_
#define Card_Name_JILIAN "Commemorative card"
#else
#define Card_Name_JILIAN "纪念卡"
#endif
#ifdef _AREA_ENG_
#define	Card_Name_YOUHUI "Coupon card"
#else
#define	Card_Name_YOUHUI "优惠卡"
#endif
#ifdef _AREA_ENG_
#define	Card_Name_Driver "Driver card"
#else
#define	Card_Name_Driver "司机卡"
#endif
#ifdef _AREA_ENG_
#define	Card_Grey_Record "GreyRecord card"
#else
#define	Card_Grey_Record "灰记录"
#endif
#ifdef _AREA_ENG_
#define	Card_Set_Line  "SetLine card"
#else
#define	Card_Set_Line  "线路设置"
#endif
#ifdef _AREA_ENG_
#define	Card_Name_departcard "Depart card"
#else
#define	Card_Name_departcard "发车卡"
#endif
#ifdef _AREA_ENG_
#define	Card_Name_stopcard "Stop card"
#else
#define	Card_Name_stopcard "到站卡"
#endif
#ifdef _AREA_ENG_
#define	Card_Name_Blkcard "Black card"
#else
#define	Card_Name_Blkcard "黑名单"
#endif
#ifdef _AREA_ENG_
#define	Card_Name_ZWYcard "Manager card"
#else
#define	Card_Name_ZWYcard "站务员卡"
#endif

#ifdef _AREA_ENG_
#define	Card_deviceNO "NO."
#else
#define	Card_deviceNO "设备号:"
#endif

#ifdef _AREA_ENG_
#define	Card_SerialNO "Serial:"
#else
#define	Card_SerialNO "序列号:"
#endif

#ifdef _AREA_ENG_
#define	Card_LoadKey "Load Key"
#else
#define	Card_LoadKey "装载密钥"
#endif
#ifdef _AREA_ENG_
#define	Card_SWKeyCard "Swipe Key card"
#else
#define	Card_SWKeyCard "请刷密码卡"
#endif
#ifdef _AREA_ENG_
#define	Card_getCardinfo "Get Card info"
#else
#define	Card_getCardinfo "取卡信息"
#endif
#ifdef _AREA_ENG_
#define	Card_CardtypeE "Card Incorrect"
#else
#define	Card_CardtypeE "不是密钥卡!"
#endif
#ifdef _AREA_ENG_
#define	Card_AuthenticateErr "Authenticate Err"
#else
#define	Card_AuthenticateErr "认证错误!"
#endif

#ifdef _AREA_ENG_
#define	Card_LoadKEyOK "Load Key complete"
#else
#define	Card_LoadKEyOK "装载密钥完成"
#endif

#ifdef _AREA_ENG_
#define	Card_Resetpos " Reset POS "
#else
#define	Card_Resetpos "恢复到出厂状态"
#endif

#ifdef _AREA_ENG_
#define	Card_HitResetpos "Will erase all records and parameters"
#else
#define	Card_HitResetpos "会清除所有记录和参数!"
#endif
#ifdef _AREA_ENG_
#define	Card_HitSure "Are you sure?"
#else
#define	Card_HitSure "确定吗?"
#endif
#ifdef _AREA_ENG_
#define	Card_Welcome "Welcome!"
#else
#define	Card_Welcome "欢迎乘车!"
#endif

#ifdef _AREA_ENG_
#define	Card_Worring "Warning:"
#else
#define	Card_Worring "警告:"
#endif
#ifdef _AREA_ENG_
#define	Card_NotBzcard "Warning:"
#else
#define	Card_NotBzcard "非本商户卡!"
#endif
#ifdef _AREA_ENG_
#define	Please_Coin "Please Coin!"
#else
#define	Please_Coin "请投币!"
#endif
#ifdef _AREA_ENG_
#define	Expired_card "Expired card!"
#else
#define	Expired_card "卡过期或未启用!"
#endif
#ifdef _AREA_ENG_
#define	Card_Carderror "Card info Error"
#else
#define	Card_Carderror "卡错误,请到充值点恢复."
#endif

#ifdef _AREA_ENG_
#define	Card_forbidCard "Forbidden"
#else
#define	Card_forbidCard "此卡禁止刷卡!"
#endif
#ifdef _AREA_ENG_
#define	Card_NodealCard "Forbid Card"
#else
#define	Card_NodealCard "消费时不能刷此卡"
#endif

#ifdef _AREA_ENG_
#define	PLS_Swipe_again "swipe again"
#else
#define	PLS_Swipe_again " 请 重 刷 "
#endif

#ifdef _AREA_ENG_
#define	PLS_Swipe_OK "swipe OK"
#else
#define	PLS_Swipe_OK " 刷卡完成 "
#endif
#ifdef _AREA_ENG_
#define	EEisFull "eeprom full!"
#else
#define	EEisFull "请上传数据!"
#endif

#ifdef _AREA_ENG_
#define	ModyfiDataTime "correction time!"
#else
#define	ModyfiDataTime "修改时间"
#endif

#ifdef _AREA_ENG_
#define	InputDataTime "Input Datetime"
#else
#define	InputDataTime " 请输入正确时间 "
#endif

#ifdef _AREA_ENG_
#define	PriceErr "price Err"
#else
#define	PriceErr " 票价错误 "
#endif

#ifdef _AREA_ENG_
#define	LineInfoErr "LineInfo Err"
#else
#define	LineInfoErr " 线路信息错误 "
#endif

#ifdef _AREA_ENG_
#define	PositionErr "Position Err"
#else
#define	PositionErr " 定位信息错误 "
#endif

#ifdef _AREA_ENG_
#define	CheckVer "Checking Version"
#else
#define	CheckVer "查找版本中"
#endif

#ifdef _AREA_ENG_
#define	UpData "UpDateing...."
#else
#define	UpData "更新参数..."
#endif

#ifdef _AREA_ENG_
#define	UpDataLine "UpDate line"
#else
#define	UpDataLine "更新线路信息"
#endif

#ifdef _AREA_ENG_
#define	STOP "STOP!"
#else
#define	STOP "暂停刷卡..."
#endif

#ifdef _AREA_ENG_
#define	SwipOff "Swip Card PLS!"
#else
#define	SwipOff "下车请刷卡"
#endif

#ifdef _AREA_ENG_
#define	DeviceType "Device Type"
#else
#define	DeviceType "设备类型"
#endif

#ifdef _AREA_ENG_
#define	SetDevice "Set Device Type"
#else
#define	SetDevice "设置设备类型"
#endif

#ifdef _AREA_ENG_
#define	FronDevice "Front Device"
#else
#define	FronDevice "前门机"
#endif

#ifdef _AREA_ENG_
#define	BackDevice "Back Device"
#else
#define	BackDevice "后门机"
#endif

#ifdef _AREA_ENG_
#define	Successfull "Successfull"
#else
#define	Successfull "设置成功"
#endif

#ifdef _AREA_ENG_
#define	deductMoneyErr "Debit fails"
#else
#define	deductMoneyErr "钱包操作失败"
#endif

#ifdef _AREA_ENG_
#define	DontSwipAgain "Swip Already"
#else
#define	DontSwipAgain "已经刷卡了"
#endif

#ifdef _AREA_ENG_
#define	OnBus " On Bus"
#else
#define	OnBus "上车"
#endif

#ifdef _AREA_ENG_
#define	AlsoNtime "Also Swip%dTimes"
#else
#define	AlsoNtime "还要刷%d次"
#endif

#ifdef _AREA_ENG_
#define	NoSame "No the same card"
#else
#define	NoSame "不是同一张卡"
#endif

#ifdef _AREA_ENG_
#define	SwipSameCard "PLS Swip the Same Card"
#else
#define	SwipSameCard "请刷同一张卡"
#endif

#ifdef _AREA_ENG_
#define	SwipAgain "PLS Swip Again"
#else
#define	SwipAgain "请再次刷卡"
#endif

#ifdef _AREA_ENG_
#define	CanErr "Can Connection Error"
#else
#define	CanErr "CAN 通讯错误"
#endif

#ifdef _AREA_ENG_
#define	AnOtherErr "AnOtherErr"
#else
#define	AnOtherErr "其他错误"
#endif

#ifdef _AREA_ENG_
#define	Err "Err"
#else
#define	Err "错误"
#endif

#ifdef _AREA_ENG_
#define	NoStationIfo "No Station Ifo"
#else
#define	NoStationIfo "没有站点信息"
#endif

#define Conn(x,y) x##y




#endif

