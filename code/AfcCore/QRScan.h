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

//////��¼
typedef struct _stTbpGpsInfo
{
	LTY_U64     ulTime;		                //��λʱ��
	LTY_U32     u32Lon;			            //����
	LTY_U32     u32Lat;			            //γ��
	LTY_U16     usVec1;		                //�ٶ�
	LTY_U16     usDirection;	            //����0-359����λΪ��(��)������Ϊ0��˳ʱ��
	LTY_S16     usAltitude; 	            //���θ߶�(m)
}PACKED(1) stTbpGpsInfo;

typedef struct TBPEventHead_s
{
    LTY_U16 u16Flag;            //TBP_EVENT_FLAG
    LTY_U8  u8Parity;           //����У���
    LTY_U8  u8EvtType;          //�ṹ����λ�ò��ܸı䣬�漰��У�������׼ȷ
    LTY_U32 u32EvtSN;
    LTY_U64 u64DateTime;        //�洢ʱ��
    LTY_U8  u8ForceGetoff;      //0:�����³���1:ǿ���³�
    LTY_U8  u8Status;           //�¼�״̬ 0xDF:δ�ϴ���0x20:
    LTY_U8  u8DeviceSN[10];     //�豸���к�
    LTY_U32 u32DeviceID;        //���ر��
    LTY_U32 u32LineID;          //��·��
    LTY_U32 u32Price;           //ÿ��ˢ���Ŀ۷ѽ��(�ֶ��ϳ��۷�0Ԫ)
    LTY_U8  u8Reserved;         //����
}PACKED(1) TBPEventHead_t;

typedef struct _stTbpQrCodeReq
{
    LTY_U32     PayDeviceNo;                     
    LTY_U32     PayLineID;
    LTY_U8      PayCityCode[6];
    LTY_U16     PayCompanyCode;
    LTY_U64     ulScanQRCTime;	            //ɨ��ʱ��
	LTY_U8      ucTDCFlag;	  	            //�̶�Ϊ���� 3�� 0x03
	LTY_U8      szVirtualCardCode[15];   	//���⿨ ID=6 �ֽڳ��д��� + 9 �ֽ�ע��Ψһ��
	LTY_U8      szVerifyCode[10];		  	//��֤��
	LTY_U32     u32BusStationCode;	        //���й���վ�����
	LTY_U32     u32BusStationNo;  	        //վ�����
	LTY_U8      szDriverID[10];		  	    //˾�����(BCD����)
	LTY_U32     u32BusSeq;		            //����������
	LTY_U8      ucOnBusFlag;		        //0,�ϳ��� 1�� �³�
	LTY_U8      ucOnDirectionFlag;          //0,���У� 1�� ����
	LTY_U16     usDistance_Next;            //������վ����(m)
    LTY_U32     uiTime_Next;              //������վ��ʱ��(s)
    LTY_U8      ucNextNo;                //��һ��վ�����
	stTbpGpsInfo gps;				  	    //GIS信息
}PACKED(1) stTbpQrCodeReq;
typedef struct TBPEventComm_s
{
    TBPEventHead_t stHead;      //41
    stTbpQrCodeReq stQrData;    //101 �ϴ�TBP�Ķ�ά�뽻������
}PACKED(1) TBPEventComm_t;


/////
typedef struct Qr_input_info{
unsigned int  BusNo;           //�������
unsigned int  LineNo;          //��·���
unsigned int  CompanyNo;     //��˾����
unsigned char CityID[6];        //����ID
unsigned int  Price;			 //Ʊ��
unsigned char Datetime[7];	     //����ʱ��BCD�룬YYYYMMDDhhmmss
unsigned int  QrcodeLen;		 //��ά�����ݳ��ȡ�//
unsigned char Qrcode[50];	     //��ά������ 
unsigned int  Longitude;       //����
unsigned int  Latitude;         //ά��
unsigned short  u8Angle;      //��λ�ǣ�GPS��λ��
}StHDQRCinput;
StHDQRCinput myqr_input; 

typedef struct ltyQr_AllInfo{
    LTY_U8   u8PriceMode;/*0 һƱ�� 1 �ֶ��շ�*/
    QrCode_Format_t stAppQr;
    LTY_U64  u64Time;/*ɨ��ʱ�䣬 UTC��ʾ*/
    LTY_U32  u32BusNo;/*�������*/
    LTY_U32  u32LineNo;/*gprs��·��*/
    LTY_U32  u32StationId;/*ɨ�����ڵ�վ��Id, ��0��ʼ*/
    LTY_U32  u32Direction;/*����0 ���� 1 ����*/
    LTY_U8   u8ForceOff;/*0 �ϳ��� 1 �����³� 2 ǿ���³� BusOnOff_Flag_E*/
    LTY_U32  u32CityBusId;/*����վ������ϳ�*/
    LTY_CHAR aszDriverId[10];/*˾�����*/
    LTY_U32  u32Price;/*�ϳ�Ϊ0���³�Ϊ�۵ķ���*/
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



