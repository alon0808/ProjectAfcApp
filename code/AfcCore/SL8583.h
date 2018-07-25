#ifndef _sl8583__H_
#define _sl8583__H_

/*flag*/
#define		FIX_LEN		0	/*(LENgth fix ��ָ��ISO_8583�еĳ��Ⱦ�������ĳ���)*/
#define		LLVAR_LEN	1	/*(LENgth 00~99)*/
#define		LLLVAR_LEN	2	/*(LENgth 00~999)*/
/*type*/
#define		L_BCD		0	/*�����BCD��*/
#define		L_ASC		1	/*�����ASC��*/
#define		R_BCD		2	/*�Ҷ���BCD��*/
#define		R_ASC		3	/*�Ҷ���ASC��*/


//�����ڵ�����λ�ã�ע�������ֵ���ĵ����λ�����1
#define field_dealcode 3
#define field_errode 9
#define field_recAdd 11
#define field_time 12
#define field_date 13
#define field_recupnum 14
#define field_errmsg 38
#define field_ack 39
#define field_cmdcode 40
#define field_posserial 41
#define field_bussinessno 42
#define field_lineno 43
#define field_deviceno 44
#define field_posfilestyle 45
#define field_severfilelist 46
#define field_ackfile 47
#define field_filedata 48
#define field_posparaver 49
#define field_severparaver 50
#define field_POSpara 51
#define field_ackpospara 52
#define field_kek 53
#define field_posinfo 54
#define field_trec 57
#define field_trec_ack 58
#define field_MAC 64


struct  ISO_8583 /* ISO 8583 Message Structure Definitions */
{
    int				len;   /* data element max length */
    unsigned char   type;  /* bit0,bit1���� */
	/*bit2:Ϊ0ʱ�����,Ϊ1ʱ�Ҷ���*/
	/* bit3:Ϊ0ʱBCD��,Ϊ1ʱASC��*/
	/*type:0,1,2,3����*/
    unsigned char   flag;  /* length field length: 0--FIX_LEN�� 1--LLVAR_LEN�� 2--LLLVAR_LEN��*/
};

typedef struct 
{
	short bitf;			/*field map if 1 true  0 false*/
    short len;			/*field length*/
    int   dbuf_addr;	/*field data's start address*/
}data_element_flag;


typedef struct  
{
	unsigned char Plen[2];			//���ĳ���
	unsigned char ID;				//ID 60H
	unsigned char desAddr[2];		//Ŀ�ĵ�ַ
	unsigned char sourceAddr[2];	//Դ��ַ
	unsigned char appType;		//Ӧ�����
	unsigned char appVer;		//����汾��
	unsigned char posStatedealType;			//�ն�״̬//����Ҫ�󣨺�4�ֽڣ�
	unsigned char keyFlagkeySN[2];			//���ܱ�ʶ//��Կ����
	unsigned char pnull;			//����

	unsigned char MSGtype[2];		//��Ϣ����
	unsigned char MSGbitmap[16];	//λԪ��
	unsigned char MSGdealCode[3];	//���״�����

}DEF8583Head;

///ǩ����Ϣ����
typedef struct 
{
	unsigned char msgType[2];		//��Ϣ����
	unsigned char bitMap[16];		//λԪ��
	unsigned char dealCmd[3];		//������
}DEF8583MSG_6001A;



#define SL8583FileMode_PRO 1	//�����ļ�
#define SL8583FileMode_BLK 2	//������
#define SL8583FileMode_GPS 3	//��λ������Ϣ
#define SL8583FileMode_PRI 4	//Ʊ����Ϣ�汾
#define SL8583FileMode_PKI 5	//��Կ��Ϣ

// #define SL8583FileFLAG_PRO7x "P7X"	//�����ļ�  POS_Cand_FLAG ����
// #define SL8583FileFLAG_PRO7S "P7S"	//�����ļ�
// #define SL8583FileFLAG_PROST "PST"	//�����ļ�
// #define SL8583FileFLAG_PROET "PET"	//�����ļ�
#define SL8583FileFLAG_PRO	"PRO"	//�����ļ����ա���������POS�ն˽���淶v2.00.docx�� �޶�ͨѶ��ʽ 
#define SL8583FileFLAG_BLK "BLK"	//������
#define SL8583FileFLAG_GPS "GPS"	//��λ������Ϣ
#define SL8583FileFLAG_CSN "CSN"	//
#define SL8583FileFLAG_PRI "PRI"	//Ʊ����Ϣ�汾,��ʱû��
#define SL8583FileFLAG_PKI "PKI"	//��Կ��Ϣ
#define SL8583FileFLAG_WHT	"WHT"	//������ ����������������������������
#define SL8583FileFLAG_TPK	"TPK"	//��Ѷ��Կ ��Ѷ����Կ	��Ѷ��ά�빫Կ�ļ�

#define SL8583FileFLAG_EC20	"EC2"	//Linux ģ���Ӧ�ó���

#define WHT_BUS "BUS"
#define WHT_BUS_JTB "JTB"
//#define SL8583FileFLAG_PPP "PPP"	//ƽ̨�豸�����汾������ǵ���ָ����Ǵ���ͳһģʽ��

#define slFileDonwLenPer 800		//�����ļ�ʱһ�����Ĵ�С,������Ҫ��10��������
//#define slFileDonwLenPer 1000		//�����ļ�ʱһ�����Ĵ�С,������Ҫ��10��������
typedef struct{
	unsigned char Miss_Fileflag[3];	//�����ļ���ʶ
	unsigned char Miss_Revert;//
   	unsigned char Miss_FileVer2[8];	//ASCII ����ʾΪ�Ӱ汾
	
	unsigned char Miss_ver[2];//���ذ汾��
	unsigned char Miss_REV[2];//������Ϊ��4�ֽڶ���

	unsigned int Miss_ALL_LEn;//���������ֽ���
	unsigned int Miss_offset;//���ڸ��µİ����	//V120���ĳ�int���ɴ�������ļ�ƫ������
	
// 	unsigned char Miss_time;
// 	unsigned char Miss_NULL1[3];//������Ϊ��4�ֽڶ���
	unsigned int filecrc;		//�������ص��ļ����� �����������Ժ�����ļ�

	int tmpfilehand;			//��ʱ�ļ�����

	unsigned int CRC32;
}st8583filedown;//�����ļ��ṹ 

// typedef struct{//ǩ��״̬ ÿ������������ǩ��
// 	unsigned char ISOK;//�����Ƿ���Ч  �˲�����Ҫ����ʱ�жϸ�ֵ
// 	//------������Կ WK����------------------
// 	unsigned char PIN_KEY[16];//û��
// 	unsigned int crc_pinkey;
// 	unsigned char MAC_KEY[16];//
// 	unsigned int crc_mackey;
// 	//---------------------------------------
// 	unsigned char gcmdcode[100];//�������ƣ�LLVAR�����99���ַ�,ǰһ���ֽ��ǳ���
// }stsl8583Style;//ǩ��״̬����������������Ϣ��KEK ���� ��������״̬����������

#define sl8583fileNum 7
typedef struct{	//�ļ��б�
	unsigned char filename[4];
	unsigned char fileVer2[8];		//�ļ��Ӱ汾
	unsigned int fileVer;
	unsigned int filelen;
	unsigned int crc32;			//�����żӵģ������س��������

}stsl8583filelist;

typedef struct{
	unsigned char msg_len[2];		//������ĳ��ȡ���������
	unsigned char msg_type[2];
	unsigned char msg_id[4];
	unsigned char msg_buff[32];
}stsl8583msg_down;



typedef struct{
	unsigned char msg_len[2];		//buff �ĳ���
	unsigned char msg_type[2];
	unsigned char msg_id[4];
	unsigned char msg_ack_code[2];
	unsigned char msg_buff[32];
}stsl8583msg_ack;

#endif


