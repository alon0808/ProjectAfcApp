#ifndef _INCLUDE_EC20LX_HTTP_H
#define _INCLUDE_EC20LX_HTTP_H

//#include "SlzrTypeDef.h"


#define BLKindex_LIST_FILE_NAME "/usrdata/qr/BLKindex"
#define BLACK_LIST_FILE_NAME "/usrdata/qr/blacklist"
#define BUSPRO_LIST_FILE_NAME "/usrdata/qr/buspro"
#define PROindex_LIST_FILE_NAME "/usrdata/qr/PROindex"
#define EC20PRO_LIST_FILE_NAME "/usrdata/qr/ec20pro"


#define SL8583FileFLAG_PRO	"PRO"	//�����ļ����ա���������POS�ն˽���淶v2.00.docx�� �޶�ͨѶ��ʽ 
#define SL8583FileFLAG_BLK "BLK"	//������
#define SL8583FileFLAG_GPS "GPS"	//��λ������Ϣ
#define SL8583FileFLAG_CSN "CSN"	//
#define SL8583FileFLAG_PRI "PRI"	//Ʊ����Ϣ�汾,��ʱû��
#define SL8583FileFLAG_PKI "PKI"	//��Կ��Ϣ
#define SL8583FileFLAG_WHT	"WHT"	//������ ����������������������������

#define SL8583FileFLAG_EC20	"EC2"	//Linux ģ���Ӧ�ó���

#define HTTP_FINISH_DOWN 0
#define HTTP_NEED_DOWN 0x01
#define HTTP_START_DOWN 0x02


#ifdef __cplusplus
extern "C" {
#endif
	extern void* main_HTTPDataDown(void *arg);
#ifdef __cplusplus
}
#endif

#endif
