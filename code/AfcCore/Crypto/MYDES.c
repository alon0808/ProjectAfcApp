

/**************************************************
* �ļ��� :MYDES.c
* ����   :DES 3DES ��MAC�㷨�ļ�
* ����   :21/3/2008	by KSLi
**************************************************/
#include "Macro_Proj.h"
#include "string.h"
//#include "..\include\include.h" 
//---------------------------------------------------------------------------


void BitsToByte(unsigned char *bitstr,unsigned char *bytechar)
//������Ϊ8���ֽڵı���λ(���һ��λ��Ч)��ѹ����һ���ֽ�,����λ�Ĵ�����ɸߵ���
{
   unsigned char i;
   unsigned char tmpchar,tmpstr[8];

   tmpchar=0;
   memcpy(tmpstr,bitstr,8);
   for (i=0;i<8;i++) tmpstr[i]=tmpstr[i]&0x01;  //clear high 7 bit
   for (i=0;i<7;i++) {
      tmpchar=tmpchar|tmpstr[i];
      tmpchar=tmpchar<<1;
   }
   tmpchar=tmpchar|tmpstr[7];

   *bytechar=tmpchar;
}

void ByteToBits(unsigned char bytechar,unsigned char *bitstr)
//��һ���ֽ���չΪ8���ֽڵı���λ(���һ��λ��Ч)��
{
   short i;
   unsigned char tmpchar;

   tmpchar=bytechar;
   for (i=7;i>=0;i--) {
      bitstr[i]=tmpchar&0x01;
      tmpchar=tmpchar>>1;
   }
}

void Permute_IP(unsigned char *text,unsigned char *text_ip)
//����������ʼ�û�
//text-����,����Ϊ8�ֽ�;test_ip-�û��������,����Ϊ8�ֽ�
{
   unsigned char byte_text[64],byte_ip_text[64];
   unsigned char i,j;

   memset(byte_text,0,64);
   memset(byte_ip_text,0,64);
   for (i=0;i<8;i++) ByteToBits(text[i],byte_text+i*8);

   for (i=0;i<4;i++) {
      for (j=0;j<8;j++) {
         byte_ip_text[i*8+j]=byte_text[(7-j)*8+i*2+1];
      }
   }

   for (i=4;i<8;i++) {
      for (j=0;j<8;j++) {
         byte_ip_text[i*8+j]=byte_text[(7-j)*8+(i-4)*2];
      }
   }

   for (i=0;i<8;i++) BitsToByte(byte_ip_text+i*8,&text_ip[i]);
}

void Reverse_IP(unsigned char *ip_text,unsigned char *text)
//���ʼ�û�
//ip_text-Դ��,����Ϊ8�ֽ�;test-���û������,����Ϊ8�ֽ�
{
   unsigned char byte_ip_text[64],byte_text[64];
   unsigned char i,j;

   memset(byte_text,0,64);
   memset(byte_ip_text,0,64);
   for (i=0;i<8;i++) ByteToBits(ip_text[i],byte_ip_text+i*8);

   for (i=0;i<8;i++) {
      for (j=0;j<4;j++) {
         byte_text[i*8+j*2]=byte_ip_text[(j+4)*8+(7-i)];
      }
   }

   for (i=0;i<8;i++) {
      for (j=0;j<4;j++) {
         byte_text[i*8+j*2+1]=byte_ip_text[j*8+(7-i)];
      }
   }

   for (i=0;i<8;i++) BitsToByte(byte_text+i*8,&text[i]);
}

void MoveLeft(unsigned char *buff)
//������Ϊ28���ֽڵĴ�ѭ������
{
   unsigned char i;
   unsigned char tmpchar;

   tmpchar=buff[0];
   for (i=0;i<27;i++) buff[i]=buff[i+1];
   buff[27]=tmpchar;
}

void GetCircleKey(unsigned char *inkey,unsigned char *outkey)
//��ԭʼ����(64λ)����һ��48λ������ԿKi,1<=i<=16
//inkey-ԭʼ����,����Ϊ8���ֽ�,outkey-16��48λ����Կ,Ҫ�󳤶�Ϊ96���ֽ�
{
   unsigned char i,j;
   unsigned char keybuff[64];
   unsigned char c0[28],d0[28];
   unsigned char ki_buff[48];

   for (i=0;i<8;i++) ByteToBits(inkey[i],keybuff+i*8);
   //�õ�C0
   for (i=0;i<3;i++) {
      for (j=0;j<8;j++) {
         c0[i*8+j]=keybuff[(7-j)*8+i];
      }
   }
   c0[24]=keybuff[59];
   c0[25]=keybuff[51];
   c0[26]=keybuff[43];
   c0[27]=keybuff[35];

   //�õ�D0
   for (i=0;i<3;i++) {
      for (j=0;j<8;j++) {
         d0[i*8+j]=keybuff[(7-j)*8+(6-i)];
      }
   }
   d0[24]=keybuff[27];
   d0[25]=keybuff[19];
   d0[26]=keybuff[11];
   d0[27]=keybuff[3];
   
   //�õ�16��48λ������Կ
   for (i=0;i<16;i++) {
      switch (i) {
         case 0:
         case 1:
         case 8:
         case 15:
            MoveLeft(c0);
            MoveLeft(d0);
            break;
         default:
            MoveLeft(c0);
            MoveLeft(c0);
            MoveLeft(d0);
            MoveLeft(d0);
            break;
      }

      ki_buff[0]=c0[13];
      ki_buff[1]=c0[16];
      ki_buff[2]=c0[10];
      ki_buff[3]=c0[23];
      ki_buff[4]=c0[0];
      ki_buff[5]=c0[4];

      ki_buff[6]=c0[2];
      ki_buff[7]=c0[27];
      ki_buff[8]=c0[14];
      ki_buff[9]=c0[5];
      ki_buff[10]=c0[20];
      ki_buff[11]=c0[9];

      ki_buff[12]=c0[22];
      ki_buff[13]=c0[18];
      ki_buff[14]=c0[11];
      ki_buff[15]=c0[3];
      ki_buff[16]=c0[25];
      ki_buff[17]=c0[7];

      ki_buff[18]=c0[15];
      ki_buff[19]=c0[6];
      ki_buff[20]=c0[26];
      ki_buff[21]=c0[19];
      ki_buff[22]=c0[12];
      ki_buff[23]=c0[1];

      ki_buff[24]=d0[12];
      ki_buff[25]=d0[23];
      ki_buff[26]=d0[2];
      ki_buff[27]=d0[8];
      ki_buff[28]=d0[18];
      ki_buff[29]=d0[26];

      ki_buff[30]=d0[1];
      ki_buff[31]=d0[11];
      ki_buff[32]=d0[22];
      ki_buff[33]=d0[16];
      ki_buff[34]=d0[4];
      ki_buff[35]=d0[19];

      ki_buff[36]=d0[15];
      ki_buff[37]=d0[20];
      ki_buff[38]=d0[10];
      ki_buff[39]=d0[27];
      ki_buff[40]=d0[5];
      ki_buff[41]=d0[24];

      ki_buff[42]=d0[17];
      ki_buff[43]=d0[13];
      ki_buff[44]=d0[21];
      ki_buff[45]=d0[7];
      ki_buff[46]=d0[0];
      ki_buff[47]=d0[3];
	  
      for (j=0;j<6;j++) BitsToByte(ki_buff+8*j,&outkey[i*6+j]);
   }
}

void Select_E(unsigned char *str32,unsigned char *str48)
//ѡ������E,��32λ�������в���,����48λ���
//str32-32λ����,����Ϊ4���ֽ�;str48-48λ���,����Ϊ6���ֽ�
{
   unsigned char in_buff[33],out_buff[48];
   unsigned char i,j;

   for (i=0;i<4;i++) ByteToBits(str32[i],in_buff+i*8);

   for (i=0;i<8;i++) {
      for (j=0;j<2;j++)
         if (i!=0) out_buff[i*6+j]=in_buff[i*4+j-1];
      for (j=2;j<6;j++) out_buff[i*6+j]=in_buff[i*4+j-1];
   }
   out_buff[0]=in_buff[31];
   out_buff[1]=in_buff[0];
   out_buff[47]=in_buff[0];

   for (i=0;i<6;i++) BitsToByte(out_buff+i*8,&str48[i]);
}

void Select_Si(unsigned char *str48,unsigned char *str32)
//ͨ��ѡ����Si��48λ������32λ���
//str48-����Ϊ6���ֽ�;str32-����Ϊ4���ֽ�
{
	unsigned char sboxvalue[8][4*16]={
		{0xe,0x4,0xd,0x1,0x2,0xf,0xb,0x8,0x3,0xa,0x6,0xc,0x5,0x9,0x0,0x7,
		0x0,0xf,0x7,0x4,0xe,0x2,0xd,0x1,0xa,0x6,0xc,0xb,0x9,0x5,0x3,0x8,
		0x4,0x1,0xe,0x8,0xd,0x6,0x2,0xb,0xf,0xc,0x9,0x7,0x3,0xa,0x5,0x0,
		0xf,0xc,0x8,0x2,0x4,0x9,0x1,0x7,0x5,0xb,0x3,0xe,0xa,0x0,0x6,0xd},

		{0xf,0x1,0x8,0xe,0x6,0xb,0x3,0x4,0x9,0x7,0x2,0xd,0xc,0x0,0x5,0xa,
		0x3,0xd,0x4,0x7,0xf,0x2,0x8,0xe,0xc,0x0,0x1,0xa,0x6,0x9,0xb,0x5,
		0x0,0xe,0x7,0xb,0xa,0x4,0xd,0x1,0x5,0x8,0xc,0x6,0x9,0x3,0x2,0xf,
		0xd,0x8,0xa,0x1,0x3,0xf,0x4,0x2,0xb,0x6,0x7,0xc,0x0,0x5,0xe,0x9},

		{0xa,0x0,0x9,0xe,0x6,0x3,0xf,0x5,0x1,0xd,0xc,0x7,0xb,0x4,0x2,0x8,
		0xd,0x7,0x0,0x9,0x3,0x4,0x6,0xa,0x2,0x8,0x5,0xe,0xc,0xb,0xf,0x1,
		0xd,0x6,0x4,0x9,0x8,0xf,0x3,0x0,0xb,0x1,0x2,0xc,0x5,0xa,0xe,0x7,
		0x1,0xa,0xd,0x0,0x6,0x9,0x8,0x7,0x4,0xf,0xe,0x3,0xb,0x5,0x2,0xc},

		{0x7,0xd,0xe,0x3,0x0,0x6,0x9,0xa,0x1,0x2,0x8,0x5,0xb,0xc,0x4,0xf,
		0xd,0x8,0xb,0x5,0x6,0xf,0x0,0x3,0x4,0x7,0x2,0xc,0x1,0xa,0xe,0x9,
		0xa,0x6,0x9,0x0,0xc,0xb,0x7,0xd,0xf,0x1,0x3,0xe,0x5,0x2,0x8,0x4,
		0x3,0xf,0x0,0x6,0xa,0x1,0xd,0x8,0x9,0x4,0x5,0xb,0xc,0x7,0x2,0xe},

		{0x2,0xc,0x4,0x1,0x7,0xa,0xb,0x6,0x8,0x5,0x3,0xf,0xd,0x0,0xe,0x9,
		0xe,0xb,0x2,0xc,0x4,0x7,0xd,0x1,0x5,0x0,0xf,0xa,0x3,0x9,0x8,0x6,
		0x4,0x2,0x1,0xb,0xa,0xd,0x7,0x8,0xf,0x9,0xc,0x5,0x6,0x3,0x0,0xe,
		0xb,0x8,0xc,0x7,0x1,0xe,0x2,0xd,0x6,0xf,0x0,0x9,0xa,0x4,0x5,0x3},

		{0xc,0x1,0xa,0xf,0x9,0x2,0x6,0x8,0x0,0xd,0x3,0x4,0xe,0x7,0x5,0xb,
		0xa,0xf,0x4,0x2,0x7,0xc,0x9,0x5,0x6,0x1,0xd,0xe,0x0,0xb,0x3,0x8,
		0x9,0xe,0xf,0x5,0x2,0x8,0xc,0x3,0x7,0x0,0x4,0xa,0x1,0xd,0xb,0x6,
		0x4,0x3,0x2,0xc,0x9,0x5,0xf,0xa,0xb,0xe,0x1,0x7,0x6,0x0,0x8,0xd},

		{0x4,0xb,0x2,0xe,0xf,0x0,0x8,0xd,0x3,0xc,0x9,0x7,0x5,0xa,0x6,0x1,
		0xd,0x0,0xb,0x7,0x4,0x9,0x1,0xa,0xe,0x3,0x5,0xc,0x2,0xf,0x8,0x6,
		0x1,0x4,0xb,0xd,0xc,0x3,0x7,0xe,0xa,0xf,0x6,0x8,0x0,0x5,0x9,0x2,
		0x6,0xb,0xd,0x8,0x1,0x4,0xa,0x7,0x9,0x5,0x0,0xf,0xe,0x2,0x3,0xc},

		{0xd,0x2,0x8,0x4,0x6,0xf,0xb,0x1,0xa,0x9,0x3,0xe,0x5,0x0,0xc,0x7,
		0x1,0xf,0xd,0x8,0xa,0x3,0x7,0x4,0xc,0x5,0x6,0xb,0x0,0xe,0x9,0x2,
		0x7,0xb,0x4,0x1,0x9,0xc,0xe,0x2,0x0,0x6,0xa,0xd,0xf,0x3,0x5,0x8,
		0x2,0x1,0xe,0x7,0x4,0xa,0x8,0xd,0xf,0xc,0x9,0x0,0x3,0x5,0x6,0xb}
	};
   unsigned char i;
   unsigned char buff48[48],buff[8],tmpchar;
   short x,y;//int x,y;
//   char disbuf[20];

   for (i=0;i<6;i++) ByteToBits(str48[i],buff48+i*8);

   for (i=0;i<8;i++) {
      x=buff48[i*6]*2+buff48[i*6+5];
      y=buff48[i*6+1]*8+buff48[i*6+2]*4+buff48[i*6+3]*2+buff48[i*6+4];
      tmpchar=sboxvalue[i][x*16+y];
      buff[i]=tmpchar;
   }
   for (i=0;i<4;i++) {
      tmpchar=((buff[i*2]<<4)&0xf0)+(buff[i*2+1]&0x0f);
      str32[i]=tmpchar;
   }
}

void Permute_P(unsigned char *text,unsigned char *text_p)
//��32λ������P�û�
//text-����Ϊ4�ֽ�;test_p-�û����32λ,����Ϊ4�ֽ�
{
   unsigned char byte_text[32],byte_p_text[32];
   unsigned char i;

   memset(byte_text,0,32);
   memset(byte_p_text,0,32);
   for (i=0;i<4;i++) ByteToBits(text[i],byte_text+i*8);

   byte_p_text[0]=byte_text[15];
   byte_p_text[1]=byte_text[6];
   byte_p_text[2]=byte_text[19];
   byte_p_text[3]=byte_text[20];

   byte_p_text[4]=byte_text[28];
   byte_p_text[5]=byte_text[11];
   byte_p_text[6]=byte_text[27];
   byte_p_text[7]=byte_text[16];

   byte_p_text[8]=byte_text[0];
   byte_p_text[9]=byte_text[14];
   byte_p_text[10]=byte_text[22];
   byte_p_text[11]=byte_text[25];

   byte_p_text[12]=byte_text[4];
   byte_p_text[13]=byte_text[17];
   byte_p_text[14]=byte_text[30];
   byte_p_text[15]=byte_text[9];

   byte_p_text[16]=byte_text[1];
   byte_p_text[17]=byte_text[7];
   byte_p_text[18]=byte_text[23];
   byte_p_text[19]=byte_text[13];

   byte_p_text[20]=byte_text[31];
   byte_p_text[21]=byte_text[26];
   byte_p_text[22]=byte_text[2];
   byte_p_text[23]=byte_text[8];

   byte_p_text[24]=byte_text[18];
   byte_p_text[25]=byte_text[12];
   byte_p_text[26]=byte_text[29];
   byte_p_text[27]=byte_text[5];

   byte_p_text[28]=byte_text[21];
   byte_p_text[29]=byte_text[10];
   byte_p_text[30]=byte_text[3];
   byte_p_text[31]=byte_text[24];

   for (i=0;i<4;i++) BitsToByte(byte_p_text+i*8,&text_p[i]);
}

void DES_encrypt(unsigned char *plain_text,unsigned char *key_text,unsigned char *encrypt_text)
//DES��������
//plain_text-����,8���ֽ�;key_text-��Կ,8���ֽ�;encrypt_text-����,8���ֽ�
{
   unsigned char i,j;
   unsigned char key16[16*6]; //���16��48λ����Կ
   unsigned char plain_ip[8],tmpbuff[4],buff48[6],tmp32[4];

   //��ʼ�û�
   Permute_IP(plain_text,plain_ip);

   //�õ�16������Կ
   GetCircleKey(key_text,key16);

   //16�ε���
   for (i=0;i<16;i++) {
      memcpy(tmpbuff,plain_ip+4,4); //L(i)=R(i-1)
      Select_E(plain_ip+4,buff48);  //R(i-1)��32λ��Ϊ48λ
	  
      for (j=0;j<6;j++) buff48[j]=buff48[j]^key16[i*6+j];  //48λģ2��
      Select_Si(buff48,tmp32);      //ͨ��ѡ������48λ��Ϊ32λ
      Permute_P(tmp32,buff48);      //P�û�
      memcpy(tmp32,buff48,4);
      for (j=0;j<4;j++) buff48[j]=tmp32[j]^plain_ip[j]; //32λģ2��
      memcpy(plain_ip,tmpbuff,4);          //L(i)
      memcpy(plain_ip+4,buff48,4);          //R(i)
   }
   //���齻��
   memcpy(tmpbuff,plain_ip+4,4);
   memcpy(plain_ip+4,plain_ip,4);
   memcpy(plain_ip,tmpbuff,4);

   Reverse_IP(plain_ip,encrypt_text);
}

void DES_decrypt(unsigned char *encrypt_text,unsigned char *key_text,unsigned char *plain_text)
//DES��������
//encrypt_text-����,8���ֽ�;key_text-��Կ,8���ֽ�;plain_text-����,8���ֽ�
{
   short i,j;
   unsigned char key16[16*6]; //���16��48λ����Կ
   unsigned char plain_ip[8],tmpbuff[4],buff48[6],tmp32[4];

   //��ʼ�û�
   Permute_IP(encrypt_text,plain_ip);

   //�õ�16������Կ
   GetCircleKey(key_text,key16);

   //16�ε���
   for (i=15;i>=0;i--) {
      memcpy(tmpbuff,plain_ip+4,4); //R(i-1)=L(i)
      Select_E(plain_ip+4,buff48);  //L(i)��32λ��Ϊ48λ
      for (j=0;j<6;j++) buff48[j]=buff48[j]^key16[i*6+j];  //48λģ2��
      Select_Si(buff48,tmp32);      //ͨ��ѡ������48λ��Ϊ32λ
      Permute_P(tmp32,buff48);      //P�û�
      memcpy(tmp32,buff48,4);
      for (j=0;j<4;j++) buff48[j]=tmp32[j]^plain_ip[j]; //32λģ2��
      memcpy(plain_ip,tmpbuff,4);          //L(i)
      memcpy(plain_ip+4,buff48,4);          //R(i)
   }
   memcpy(tmpbuff,plain_ip+4,4);
   memcpy(plain_ip+4,plain_ip,4);
   memcpy(plain_ip,tmpbuff,4);

   Reverse_IP(plain_ip,plain_text);
}

void DES3_encrypt(unsigned char *plain_text, const unsigned char *key_text,unsigned char *encrypt_text)
{
   unsigned char left_key[8],right_key[8];
   unsigned char encrypt1[8],encrypt2[8];

#ifdef _watchdog_open
   clr_wdt();
#endif

   memset(left_key,0,8);
   memset(right_key,0,8);
   memset(encrypt1,0,8);
   memset(encrypt2,0,8);
   memcpy(left_key,key_text,8);
   memcpy(right_key,key_text+8,8);

   DES_encrypt(plain_text,left_key,encrypt1);
   DES_decrypt(encrypt1,right_key,encrypt2);
   DES_encrypt(encrypt2,left_key,encrypt_text);
   return;
}

void DES3_decrypt(unsigned char *encrypt_text,unsigned char *key_text,unsigned char *plain_text)
{
   unsigned char left_key[8],right_key[8];
   unsigned char encrypt1[8],encrypt2[8];

   memset(left_key,0,8);
   memset(right_key,0,8);
   memset(encrypt1,0,8);
   memset(encrypt2,0,8);
   memcpy(left_key,key_text,8);
   memcpy(right_key,key_text+8,8);

   DES_decrypt(encrypt_text,left_key,encrypt1);
   DES_encrypt(encrypt1,right_key,encrypt2);
   DES_decrypt(encrypt2,left_key,plain_text);
   return;
}

void Xor_encrypt(unsigned char *Text1, unsigned char *Text2, unsigned char *Text3)
{
	Text3[0] = Text1[0] ^ Text2[0];
	Text3[1] = Text1[1] ^ Text2[1];
	Text3[2] = Text1[2] ^ Text2[2];
	Text3[3] = Text1[3] ^ Text2[3];
	Text3[4] = Text1[4] ^ Text2[4];
	Text3[5] = Text1[5] ^ Text2[5];
	Text3[6] = Text1[6] ^ Text2[6];
	Text3[7] = Text1[7] ^ Text2[7];
}

void DES3_decrypt_CBC(unsigned char *key,unsigned char *initdat,unsigned char *Sdat,unsigned int len ,unsigned char *outdat)
{
	int m;
	int i;
	int n;
	unsigned char inittext[8];
	unsigned char data[8];
	unsigned char Tdata[8];
	unsigned char Source[256];
	
	memcpy(inittext, initdat, 8);	//��ʼֵ

	if(len > 256)
		len = 256;
	memcpy(Source, Sdat, len);
	
	n = (len - len % 8) / 8;
	m = len % 8;
	if(m != 0){
		for(i = 0; i<(8-m); i++)
			Source[len + i] = 0x00;
		n++;
	}//����8������λ�ں�����0����8��������

	for(i = 0; i<n; i++){
		memcpy(data, Source+(8*i), 8);
		DES3_decrypt(data, key, Tdata);
		
		Xor_encrypt(inittext, Tdata, inittext);
		memcpy(outdat+(8*i), inittext, 8);
		memcpy(inittext, data, 8);
	}
	
	return;
}

void DES3_encrypt_CBC(unsigned char *key,unsigned char *initdat,unsigned char *Sdat,unsigned int len ,unsigned char *outdat)
{
	int m;
	int i;
	int n;
	unsigned char inittext[8];
	unsigned char data[8];
	unsigned char Tdata[8];
	unsigned char Source[256];
	
	memcpy(inittext, initdat, 8);	//��ʼֵ
	
	if(len > 256)
		len = 256;
	memcpy(Source, Sdat, len);
	
	n = (len - len % 8) / 8;
	m = len % 8;
	if(m != 0){
		for(i = 0; i<(8-m); i++)
			Source[len + i] = 0x00;
		n++;
	}//����8������λ�ں�����0����8��������
	
	for(i = 0; i<n; i++){
		memcpy(data, Source+(8*i), 8);
		Xor_encrypt(inittext, data, Tdata);
		DES3_encrypt(Tdata, key, inittext);
		
		memcpy(outdat+(8*i), inittext, 8);
	}
	
	return;
}

void CountMac1(unsigned char *Key,unsigned char *random,unsigned char *Sdat,unsigned int len ,unsigned char *Result)
{
    unsigned char tmp[16];
	unsigned char obuf[16];
    int i,j;
	unsigned char _data[512];
	
	if(len > 512)
		len = 512;
	memcpy(_data, Sdat, len);
	
	
	if(len%8 == 0)
	{
		;
	}
	else
	{
		memset(_data+len, 0, (8-len%8));//����8������λ�ں�����0����8��������
		len=len+8-(len%8);
	}
	
	memset(obuf, 0, 16);
	memset(tmp, 0x00, sizeof(tmp));
    memcpy(tmp,random,8);

    for(i=0;i<(int)len;i+=8)
	{
		for(j=0;j<8;j++){
			if((i+j)<(int)len)
				tmp[j]^=_data[i+j];	
		}
		DES3_encrypt(tmp,Key,obuf);
		memcpy(tmp, obuf, 8);
    }
    memcpy(Result,tmp,8);
    return;
}

extern void BCD2Ascii( unsigned char *bcd, unsigned char *Ascii, unsigned int len );
void CountMac_CUP(unsigned char *Key,unsigned char *random,unsigned char *Sdat,unsigned int len ,unsigned char *Result)
{
    unsigned char tmp[16];
	unsigned char obuf[16];
    unsigned int i,j;
	unsigned char _data[512];
	
	if(len > 512)
		len = 512;
	memcpy(_data, Sdat, len);
	
	
	if(len%8 == 0)
	{
		;
	}
	else
	{
		memset(_data+len, 0, (8-len%8));//����8������λ�ں�����0����8��������
		len=len+8-(len%8);
	}
	
	memset(obuf, 0, 16);
	memset(tmp, 0x00, sizeof(tmp));
    memcpy(tmp,random,8);
	
    for(i=0;i<len;i+=8)
	{
		for(j=0;j<8;j++){
			tmp[j]^=_data[i+j];	
		}
    }
	BCD2Ascii(tmp, _data, 8);
	memcpy(tmp, _data, 8);
	DES_encrypt(tmp,Key,obuf);
	for(j=0;j<8;j++){
		tmp[j] = _data[8+i] ^ obuf[i];
	}
	DES_encrypt(tmp,Key,obuf);
	
    memcpy(Result,obuf,8);
    return;
}

//16�ֽ���Կ
/************************************************************************/
/* key : ��Կ 16�ֽ�
   inittext : ��ʼֵ  8�ֽ�
   Source : ����ܵ�����
   le : �������ݵĳ���
   destin : ���ܺ�Ľ�� 4�ֽ�
                                                                     */
/************************************************************************/
void CountMac(const char *key,unsigned char *inittext,unsigned char *Sdat,unsigned int le ,unsigned char *destin)
{
	int m;
	int i;
	int n;
	unsigned char data[8];
	unsigned char Tdata[8];
	unsigned char keyA[8];
	unsigned char keyB[8];
	unsigned char Source[256];
	
	if(le > 256)
		le = 256;
	memcpy(Source, Sdat, le);

	n = (le - le % 8) / 8;
	m = le % 8;
	if(m == 0){
		Source[le + 0] = 0x80;
		Source[le + 1] = 0;
		Source[le + 2] = 0;
		Source[le + 3] = 0;
		Source[le + 4] = 0;
		Source[le + 5] = 0;
		Source[le + 6] = 0;
		Source[le + 7] = 0;
	}
	else{
		for(i = 1; i<(8-m); i++)
			Source[le + i] = 0x00;
		
		Source[le] = 0x80;
	}//����8������λ�ں�����0����8��������
	n++;
	keyA[0] = key[0];
	keyA[1] = key[1];
	keyA[2] = key[2];
	keyA[3] = key[3];
	keyA[4] = key[4];
	keyA[5] = key[5];
	keyA[6] = key[6];
	keyA[7] = key[7];
	
	keyB[0] = key[8];
	keyB[1] = key[9];
	keyB[2] = key[10];
	keyB[3] = key[11];
	keyB[4] = key[12];
	keyB[5] = key[13];
	keyB[6] = key[14];
	keyB[7] = key[15];
		
	for(i = 0; i<n; i++){
		data[0] = Source[0 + 8 * i];
		data[1] = Source[1 + 8 * i];
		data[2] = Source[2 + 8 * i];
		data[3] = Source[3 + 8 * i];
		data[4] = Source[4 + 8 * i];
		data[5] = Source[5 + 8 * i];
		data[6] = Source[6 + 8 * i];
		data[7] = Source[7 + 8 * i];
		Xor_encrypt(inittext, data, Tdata);
		DES_encrypt(Tdata, keyA,inittext);
	}
		
	DES_decrypt(inittext, keyB, Tdata);
	DES_encrypt(Tdata, keyA,inittext);

	destin[0] = inittext[0];
	destin[1] = inittext[1];
	destin[2] = inittext[2];
	destin[3] = inittext[3];
    return;
}

//8�ֽ���Կ
/************************************************************************/
/* key : ��Կ 8�ֽ�
   inittext : ��ʼֵ 8�ֽ�
   Source : ����ܵ�����
   le : �������ݵĳ���
   destin : ���ܺ�Ľ�� 4�ֽ�
                                                                     */
/************************************************************************/
void SCountMac(unsigned char *key,unsigned char *inittext,unsigned char *Sdat,unsigned int le ,unsigned char *destin)
{
	int m;
	int i;
	int n;
	unsigned char data[8];
	unsigned char Tdata[8];
	unsigned char keyA[8];
	unsigned char Source[256];
	
	if(le > 256)
		le = 256;
	memcpy(Source, Sdat, le);
	
	n = (le - le % 8) / 8;
	m = le % 8;
	if(m == 0){
		Source[le + 0] = 0x80;
		Source[le + 1] = 0;
		Source[le + 2] = 0;
		Source[le + 3] = 0;
		Source[le + 4] = 0;
		Source[le + 5] = 0;
		Source[le + 6] = 0;
		Source[le + 7] = 0;
	}
	else{
		for(i = 1; i<(8-m); i++)
			Source[le + i] = 0x00;
		
		Source[le] = 0x80;
	}//����8������λ�ں�����0����8��������
	n++;
	keyA[0] = key[0];
	keyA[1] = key[1];
	keyA[2] = key[2];
	keyA[3] = key[3];
	keyA[4] = key[4];
	keyA[5] = key[5];
	keyA[6] = key[6];
	keyA[7] = key[7];
	
	for(i = 0; i<n; i++){
		data[0] = Source[0 + 8 * i];
		data[1] = Source[1 + 8 * i];
		data[2] = Source[2 + 8 * i];
		data[3] = Source[3 + 8 * i];
		data[4] = Source[4 + 8 * i];
		data[5] = Source[5 + 8 * i];
		data[6] = Source[6 + 8 * i];
		data[7] = Source[7 + 8 * i];
		Xor_encrypt(inittext, data, Tdata);
		DES_encrypt(Tdata, keyA,inittext);
	}
	
	destin[0] = inittext[0];
	destin[1] = inittext[1];
	destin[2] = inittext[2];
	destin[3] = inittext[3];
    return;
}
/*
Key     ������SESLK��3DES����Կ��������Ȧ��ʱ����SESLKʱ��3DES����ԿΪ��Ȧ������Կ��(������˻���Ϣ)��
pInData1���μӼ���SESLK��Դ���ݣ�������Ȧ��ʱpInData1Ϊ  α�������ICC��||���Ӵ�������������Ż����Ǯ�������������||��8000��
pInData2���μӼ���MAC��Դ���ݣ�������Ȧ��ʱpInData2Ϊ  ���Ӵ���������ǰ�����ߵ���Ǯ��������ǰ��||���׽�� || �������ͱ�ʶ || �ն˻���š�
data2len��pInData2�ĳ��ȣ�
initData��8 bytes��0x00 ���飻
mac     ����Ҫ���������MACֵ�����
*/
void Mac(unsigned char *Key,unsigned char *pInData1,unsigned char *pInData2,int data2len,unsigned char *initData,unsigned char *mac)
{
    unsigned char tempin[8]={0};
	unsigned char tempout[8]={0};
	int i;

	// 	debugstring("KEY:");
	// 	debugdata(Key, 16, 1);
	// 	
	// 	debugstring("pInData1:");
	// 	debugdata(pInData1, 8, 1);
	// 	
	// 	debugstring("pInData2:");
	// 	debugdata(pInData2, data2len, 1);
	// 	
	// 	debugstring("initData:");
	// 	debugdata(initData, 8, 1);
	
	
	for(i=0;i<8;i++)
		tempin[i] = pInData1[i];
	DES_encrypt(tempin,Key,tempout);
	
	for(i=0;i<8;i++)
		tempin[i] = tempout[i];
	DES_decrypt(tempin,&Key[8],tempout);
	
	for(i=0;i<8;i++)
		tempin[i] = tempout[i];	
	DES_encrypt(tempin,Key,tempout);
	
	//Macshort(tempout,pInData2,data2len,initData,mac);
	SCountMac(tempout, initData, pInData2, data2len,mac);
	
}

//���貿��CPU����KEY���㣬�Ž��ĳ���//���貿��ISAM���㲻��KEYa,������Ĭ��ֵ��
unsigned char getJiuJiang_CPUKey(unsigned char *Sdata, unsigned char *Rkey)
{
	//	unsigned char snddat[16];
	unsigned char revbuf[32];
	const unsigned char snddat[16] = {0x07,0x92,0xAF,0xC7,0x9B,0x1F,0xB5,0x5A,0xD0,0x83,0xFC,0x78,0xE1,0x58,0x47,0x73};
	//	memcpy(snddat, "\x07\x92\xAF\xC7\x9B\x1F\xB5\x5A\xD0\x83\xFC\x78\xE1\x58\x47\x73", 16);//key
// #ifdef _debug_CPU_
// 	debugdata(Sdata, 16, 1);//+12
// 	debugdata((unsigned char*)snddat, 16, 1);//+12
// #endif
	DES3_encrypt(Sdata, snddat, revbuf);//
// #ifdef _debug_CPU_
// 	debugdata(revbuf, 16, 1);//+12
// #endif
	DES3_encrypt(Sdata+8, snddat, revbuf+8);//
// #ifdef _debug_CPU_
// 	debugdata(revbuf, 16, 1);//+12
// #endif
	
	memcpy(Rkey, revbuf, 16);
	return 0;
}
//ȡCPU����ά����Կ 2=����
void getCPUMAC(unsigned char mode, unsigned char *inittext,unsigned char *Source, unsigned int le ,unsigned char *destin)
{
	unsigned char AMKEY[16];
	
	if(mode == 2)//0310AFC79B1FB55AD083FC78E1584773
		memcpy(AMKEY, "\x03\x10\xAF\xC7\x9B\x1F\xB5\x5A\xD0\x83\xFC\x78\xE1\x58\x47\x73", 16);//����Ӧ��ά����Կ
	else{
		memset(destin, 0, 4);
		return;
	}
	CountMac((char*)AMKEY, inittext, Source, le, destin);
	return;
}

