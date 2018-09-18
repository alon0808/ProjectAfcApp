

//creat by lks

#ifndef __CITY_HANDAN_
#define __CITY_HANDAN_

#define SYS_cityName "HD"

#define SYS_HEAD_STR	"邯郸公交"
#define SYS_HEAD_STREn	"  HANDAN  "
#define PCityCode "\x07\x55"

#define CLOUD_BUSSINESSNO "\x99\x98"//"\x03\x10"

//192.168.1.4 8818
#define GJ_IP 0x0401A8C0
#define GJ_PORT 8818

//邯郸专网？
#define NETAPN "hdgj.spjk.heapn"

#define qPBOC_BUS	// 银行卡功能

//同一张卡连刷延时秒数。
#define NORM_DLY_TIME 0
//卡不能连刷时间。300秒
#define WAIT_TIME 300

//蓝泰源的二维码有效期，单位：秒
#define limitTime_LTYQRC 180

//定义后是小额收费。用于邯郸食堂
//	#define _HD_XIAOER_
//	#define	_HD_XIAOER_OPEN    //小额对外
//定义后用专网
#define _GPRS_Zhuangwang_
//有GPRS及CPU卡，用的是建设部密钥
//定义为新的模具车载机,需要更换编译工程。
#define _New_Bu_mode_
//使用外接键盘，现在用COM1
#define KEYBOARD	1
//使用长沙测试平台
#define _MOBILE_IP_CHANGSHA_
#define _MOBILE_CMNET_
#define GPRS_MG323_
//用外接的DVR传数据,现在还没有用。
//	#define OTHER_GPRS
//从用串口转网口，使用的是透传，不用串口协议,在邯郸用，还在测试，用不用另说，如果确认不用需要把这个相关的删除。
//	#define COM2RJ45_
//没有移动后台
#define NO_MOBILE_SERVISE
//定义为建设部的卡
#define BUS_PRO_JSB
//定义为CPU卡,应该不用2.4G的
#define BUS_CPU_
//不用专门的CPU 卡的 PSAM卡，密钥通过M1卡的PSAM算出，MAC1用软件计算。(各2.4G读头中也有效)
#define NO_CPU_PSAM_
//此CPU卡需要选择PSE，都加上这个,有没有都选一下， 中钞的PSE："\x02\x00\xA4\x04\x00\x0E\x31\x50\x41\x59\x2E\x53\x59\x53\x2E\x44\x44\x46\x30\x33"
#define CPU_PSE "\x02\x00\xA4\x04\x00\x0E\x31\x50\x41\x59\x2E\x53\x59\x53\x2E\x44\x44\x46\x30\x33"
//胜华电子的移动手机卡，13.56M 厦门盛华13.56MHZ手机卡测试密钥
//			#define CPU_PSE "\x02\x00\xA4\x04\x00\x10\xD1\x56\x00\x00\x40\x10\x00\x30\x00\x00\x00\x01\x00\x00\x00\x00"
//移动2.4G手机CPU卡，不是移动区，公交专用区
//	#define _CPU_24G_
//厦门盛华电子2.4G读头，使用M1卡的功能，只有发行区、钱包区和公共信息区,密钥使用租赁平台的方法。
//CPU卡使用的公司的PSAM卡，计算的密钥
// #define _XMSHDZ_24G_
// #define G24Com COM6


//定义不使用2.4G卡
#define NO_MOBILE_24G_
//老的扣款方式（电信的卡用）
#define _PURSE_OLD_STYLE
#define  BUS_PRO_JTB



#endif

