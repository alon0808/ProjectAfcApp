


//车载机任务标志
#define MISS_IF1 0x10//需要登陆到IF1
#define MISS_M_SINGIN 0x11		//0x01 登陆移动,签到.
#define MISS_M_Settlement 0x12	//0x02 批结算
#define MISS_M_TradeBatch 0x13	//0x03 批上送交易
#define MISS_IF2 0x20//需要登陆到IF2
#define MISS_M_DPARAM 0x21		//0x01 下载参数
#define MISS_M_DPGRAM 0x22		//0x02 程序下载请求
#define MISS_M_DPGRAMOK 0x23	//0x03 程序下载结果通知
#define MISS_M_KEK 0x24			//0x04 KEK下载
#define MISS_M_ActiveTest 0x25	//0x05 回响

#define MISS_GJ 0x40	//需要登陆到公交
#define MISS_G_LOGINGJ 0x40		//0xF0 登陆公交后台
#define MISS_G_DBLKD 0x41		//0xF1 下载黑名单 减  现在是增减都在一个包里，不需要区分
#define MISS_G_UREC 0x42		//2 上传数据
#define MISS_G_FILES 0x43		//3 下载文件 程序,所有的文件下载都使用此命令
#define MISS_G_DLine	0x44		//3 下载文件 线路参数
#define MISS_G_DSound	0x45		//3 下载文件 语音参数
#define MISS_G_DPROOK	0x46	//下载程序结果
#define MISS_G_DBLKI 0x47		//0xF1 下载黑名单 加
#define MISS_G_DLineOK	0x48	//下载程序结果
#define MISS_G_DSoundOK	0x49	//下载程序结果
#define MISS_G_HART 0x4A		//心跳包
#define MISS_G_GPS 0x4B			//GPS massage
#define MISS_G_TOO 0x4C			//终端机告知指令 -2002: 终端机主动发送终端机基本信息给接收机。
#define MISS_G_ALAM 0x4D		//发送报警等GPS信息
#define MISS_G_PRICE 0x4E		//下载票价线路信息
#define MISS_G_FREE 0x4F		//连接到公交，但是空闲中

#define MISS_G_TREC 0x80		//实时处理的二维码数据

#define MISS_G_MSG1 0x60		//需要发送短信
#define MISS_G_MSG2 0x61		//需要发送内容

#define MISS_HTTP 0x80			//80以上的任务都是HTTP下载的
#define MISS_HTTP_BLK 0x81		//通过LINUX模块下载黑名单文件 
#define MISS_HTTP_EC20 0x82		//通过LINUX模块下载LInux模块程序 
#define MISS_HTTP_PRO 0x83		//通过LINUX模块下载车载机程序 

#define MISS_G_SINGOUT 0x70		//终端签退

#define MISS_COM_PARA 0x80		//串口下载程序中
//签到状态，0位：KEK需下载，1位：参数需下载，2位：程序需下载
#define LoadStyle_KEK (0x01<<0)
#define LoadStyle_para (0x01<<1)
#define LoadStyle_program (0x01<<2)


//GPRS无线通讯状态
#define GPRS_LINK_CMD	0xF0//模块关闭中
#define GPRS_TCPCLOSE_CMD 0xF1//TCPIP链接关闭中

//启动MC55i,需要开启，使电平为高
#define GPRS_LINK_UP 0xF3
//GPRS正在等模块注册
#define GPRS_AT_WAIT 0xF4
//命令关闭模块，AT^SMSO
#define GPRS_MC55_CLOSE 0xF5
//模块需要重启
#define GPRS_MODE_NEED_CLOSE 0xF6
//模块下电
#define GPRS_MODE_POWER_DOWN 0xF7
//模块上电
#define GPRS_MODE_POWER_ON 0xF8
//AT软件复位指令
#define GPRS_AT_RESET 0xFA
//模块电源关闭然后打开，重新开始GPRS测试
#define GPRS_HWPOWER_RESET 0xFE


//模块需要关闭TCP/IP
#define GPRS_NEED_CLOSEIP 0x2F

#define GPRS_Out_CMD 0xF2//GPRS分离
#define GPRS_SENDING_CMD 0xA0//正在发送数据，等收应答中
#define GPRS_RING_CMD 0xA1//有电话呼入
#define GPRS_CStatues 0xA2	//在过程中查询WIFI网络是否正常。用于检测网络是否离开(还没有使用)

#define TCPSTARTSTAT	22	//7TCP/IP开始值，之前的值是模块初始化进程

