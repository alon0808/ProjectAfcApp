#ifndef __BN_H__
#define __BN_H__

//#define USE_MALLOC

//macro for common bn
#define WordLen						32
#define LSBOfWord					0x00000001
#define MSBOfWord					0x80000000
#define Plus						0x00000000
#define Minus						0x00000001

//macro for RSA
#define RSABNBitLen					1024
#define RSABNWordLen				32
#define RSAPrimeWordLen				16
#define MAXPrimeWordLen				32
/*#define RSABNBitLen					2048
#define RSABNWordLen				64
#define RSAPrimeWordLen				32
#define MAXPrimeWordLen				64*/
#define Ext_RSABNWordLen			RSABNWordLen + 2
#define BNMAXWordLen				2 * RSABNWordLen + 2

//macro for ECC
#define ECCBNBitLen					256
#define ECCBNWordLen				8
#define NAF_W						4
#define ECCPreTableSize				1 << (NAF_W - 2)
#define NAF_Flag					(0xffffffff << (WordLen - NAF_W)) >> (WordLen - NAF_W)
#define WordByteLen					4
#define MAXBNWordLen				8
#define MAXBNByteLen				MAXBNWordLen*WordByteLen
#define MAXBNBitLen					MAXBNByteLen*8

typedef struct _NAF
{
	unsigned long	count;					//count : the number of SWord occupied by NAF(k)		
	long	k[ECCBNBitLen + 1];		// every ki of NAF(k) store in this array 
} NAF, *pNAF;

typedef struct _A_Point
{
	unsigned long X[ECCBNWordLen];				// X坐标
	unsigned long Y[ECCBNWordLen];				// Y坐标
}A_Point, *pA_Point;					// struct of affine coordinate

typedef	struct _J_Point
{
	unsigned long X[ECCBNWordLen];				// X坐标
	unsigned long Y[ECCBNWordLen];				// Y坐标
	unsigned long Z[ECCBNWordLen];				// Z坐标
}J_Point, *pJ_Point;					// struct of projective coordinate

typedef struct _Jm_Point
{
	unsigned long X[ECCBNWordLen];				// X坐标
	unsigned long Y[ECCBNWordLen];				// Y坐标
	unsigned long Z[ECCBNWordLen];				// Z坐标
	unsigned long aZ4[ECCBNWordLen];				// aZ^4坐标
}Jm_Point, *pJm_Point;					// struct of modified projective coordinate

typedef struct _EC
{
	long BNWordLen;						// 椭圆曲线系统位长对应的字节数
	unsigned long EC_P[ECCBNWordLen];				// 椭圆曲线参数p
	unsigned long EC_N[ECCBNWordLen];				// 椭圆曲线阶 N 
	unsigned long EC_a[ECCBNWordLen];				// 椭圆曲线参数 a
	unsigned long EC_b[ECCBNWordLen];				// 椭圆曲线参数 b
	A_Point EC_G;						// 椭圆曲线基点G
	A_Point GArray[ECCPreTableSize];	// 预计算数据
}EC, *pEC;	

typedef struct _SCH_CTX
{
	unsigned long state[8];
	unsigned long count[2];
	unsigned char buffer[64];
} SCH_CTX;


#ifdef  __cplusplus
extern "C" {
#endif
	unsigned char SM2_ENC(unsigned char* pucPlainHex,unsigned long uiPlainLen,unsigned char* strPub,unsigned char*pucEncipher);
	unsigned char SM2_DEC(unsigned char* cipher,unsigned char* strPri,unsigned char *pucPlainHex);
	unsigned char SM2_Sign(unsigned char* pucData,unsigned long uiDataLen,unsigned char* strPri,unsigned char *strSign);
	unsigned char  SM2_Verify(unsigned char* pucData,unsigned long uiInLen,unsigned char* Signature,unsigned char* strPub);
	unsigned char PBOC_SM2_Verify(char *ID,unsigned char *pucPub,unsigned long uiPubicKeyLen,unsigned char *pucInData,unsigned long uiInLen,unsigned char *pucDigest);
	void SM3_Hash(unsigned char *pucInMessage,unsigned long uiInMessageLen,unsigned char *pucDigest);
	int SM3_Compute(unsigned char* InMessage,unsigned long uiInLen,unsigned char*strDigest);
	void SM4_Encrypt(unsigned char *pKey, unsigned char *pDataIn, unsigned char *pDataOut);
	void SM4_Decrypt(unsigned char *pKey, unsigned char *pDataIn, unsigned char *pDataOut);
	unsigned char ECPointDecompress(unsigned char *pucOutY, unsigned char *pucPub );
#ifdef  __cplusplus
}
#endif


#endif

