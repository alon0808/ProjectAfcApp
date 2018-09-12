#ifndef QR_H
#define QR_H

#include "LtyCommon.h"

#include "ev.h"
//#include "LtyTimer.h"



#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <openssl/objects.h>
#include <openssl/pem.h>


#define LTY_QRCODE_LEN 36
typedef struct ltyQrCode_Format
{
    unsigned char  QRDateBefor;
    unsigned char  QRCityID[6];
    unsigned char  QRLineID[9];
    unsigned char  QRTimestamp[10];
    unsigned char  QRSignature[10];
}QrCode_Format_t ;

//////记录
typedef struct _stTbpGpsInfo
{
	LTY_U64     ulTime;		                //定位时间
	LTY_U32     u32Lon;			            //经度
	LTY_U32     u32Lat;			            //纬度
	LTY_U16     usVec1;		                //速度
	LTY_U16     usDirection;	            //方向，0-359，单位为度(°)，正北为0，顺时针
	LTY_S16     usAltitude; 	            //海拔高度(m)
}PACKED(1) stTbpGpsInfo;

typedef struct TBPEventHead_s
{
    LTY_U16 u16Flag;            //TBP_EVENT_FLAG
    LTY_U8  u8Parity;           //数据校验和
    LTY_U8  u8EvtType;          //结构体中位置不能改变，涉及到校验和数据准确
    LTY_U32 u32EvtSN;
    LTY_U64 u64DateTime;        //存储时间
    LTY_U8  u8ForceGetoff;      //0:正常下车，1:强制下车
    LTY_U8  u8Status;           //事件状态 0xDF:未上传，0x20:
    LTY_U8  u8DeviceSN[10];     //设备序列号
    LTY_U32 u32DeviceID;        //车载编号
    LTY_U32 u32LineID;          //线路号
    LTY_U32 u32Price;           //每次刷卡的扣费金额(分段上车扣费0元)
    LTY_U8  u8Reserved;         //保留
}PACKED(1) TBPEventHead_t;

typedef struct _stTbpQrCodeReq
{
    LTY_U32     PayDeviceNo;                     
    LTY_U32     PayLineID;
    LTY_U8      PayCityCode[6];
    LTY_U16     PayCompanyCode;
    LTY_U64     ulScanQRCTime;	            //扫码时间
	LTY_U8      ucTDCFlag;	  	            //固定为数字 3， 0x03
	LTY_U8      szVirtualCardCode[15];   	//虚拟卡 ID=6 字节城市代码 + 9 字节注册唯一码
	LTY_U8      szVerifyCode[10];		  	//验证码
	LTY_U32     u32BusStationCode;	        //城市公交站点编码
	LTY_U32     u32BusStationNo;  	        //站点序号
	LTY_U8      szDriverID[10];		  	    //司机编号(BCD编码)
	LTY_U32     u32BusSeq;		            //公交班次序号
	LTY_U8      ucOnBusFlag;		        //0,上车； 1， 下车
	LTY_U8      ucOnDirectionFlag;          //0,上行； 1， 下行
	LTY_U16     usDistance_Next;            //距离下站距离(m)
    LTY_U32     uiTime_Next;              //距离下站的时间(s)
    LTY_U8      ucNextNo;                //下一个站点序号
	stTbpGpsInfo gps;				  	    //GIS淇℃伅
}PACKED(1) stTbpQrCodeReq;
typedef struct TBPEventComm_s
{
    TBPEventHead_t stHead;      //41
    stTbpQrCodeReq stQrData;    //101 上传TBP的二维码交易数据
}PACKED(1) TBPEventComm_t;


/////
typedef struct Qr_input_info{
unsigned int  BusNo;           //车辆编号
unsigned int  LineNo;          //线路编号
unsigned int  CompanyNo;     //公司代码
unsigned char CityID[6];        //城市ID
unsigned int  Price;			 //票价
unsigned char Datetime[7];	     //日期时间BCD码，YYYYMMDDhhmmss
unsigned int  QrcodeLen;		 //二维码数据长度。//
unsigned char Qrcode[50];	     //二维码内容 
unsigned int  Longitude;       //经度
unsigned int  Latitude;         //维度
unsigned short  u8Angle;      //方位角，GPS方位角
}StHDQRCinput;
StHDQRCinput myqr_input; 

typedef struct ltyQr_AllInfo{
    LTY_U8   u8PriceMode;/*0 一票制 1 分段收费*/
    QrCode_Format_t stAppQr;
    LTY_U64  u64Time;/*扫码时间， UTC表示*/
    LTY_U32  u32BusNo;/*车辆编号*/
    LTY_U32  u32LineNo;/*gprs线路号*/
    LTY_U32  u32StationId;/*扫码所在的站点Id, 从0开始*/
    LTY_U32  u32Direction;/*方向0 上行 1 下行*/
    LTY_U8   u8ForceOff;/*0 上车， 1 正常下车 2 强制下车 BusOnOff_Flag_E*/
    LTY_U32  u32CityBusId;/*城市站点编码上车*/
    LTY_CHAR aszDriverId[10];/*司机编号*/
    LTY_U32  u32Price;/*上车为0，下车为扣的费用*/
}PACKED(1)Qr_AllInfo_t;



struct QRCODE_FORMAT_S
{
	unsigned char  QRDateBefor[2 + 1];
	unsigned char  QRPrice[4 + 1];
	unsigned char  QRCityID[6 + 1];
	unsigned char  QRLineID[6 + 1];
	unsigned char  QROrder[20 + 1];
	unsigned char  QRTimestamp[10 + 1];
	unsigned char  QRIsvalid;
	unsigned char  QRSignature[10 + 1];
}PACKED(1) ;
typedef struct QRCODE_FORMAT_S  QRCODE_FORMAT_t;
typedef enum ltyQr_Source_E
{
    Qr_Source_Com,
    Qr_Source_Slave
}Qr_Source_E;

class CQR 
{
public:
    CQR();
    virtual ~CQR();
    LTY_RESULT Init(struct ev_loop *loop);
    LTY_VOID   Release(); 
	int HDQRCodeVerify(StHDQRCinput *inData, unsigned char *OutRecord, unsigned int *OutRecLen);  
private:
	
	LTY_STRING base64Encode(const unsigned char *bytes, int len);
	RSA* rsaEncryptUsingPublicKeyFile(unsigned char *keyFile);
	char * rsaSign(const char *content, const char *key); 
	LTY_U32    m_QRTimeOutDuration;
    LTY_U8     m_u32CityId[6];
    LTY_U32  StartParsQr(LTY_U8 *pRcvData,LTY_U32 RcvLen);
   
    LTY_S32 ParseLtyQr(LTY_U8* u8Qr, LTY_U32 RcvLen);
};

#endif



