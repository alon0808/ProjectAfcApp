
//

#define MAX_RSA_MODULUS_BITS 2048
#define MAX_RSA_MODULUS_LEN ((MAX_RSA_MODULUS_BITS+7)/8)
#define MAX_RSA_PUBLICE_LEN 3
/* Random structure.
 */
typedef struct R_RANDOM_STRUCT{
  unsigned int bytesNeeded;
  unsigned char state[16];
  unsigned int outputAvailable;
  unsigned char output[16];
} R_RANDOM_STRUCT;

/* RSA public and private key.
 */
typedef struct R_RSA_PUBLIC_KEY{
  unsigned int bits;                           /* length in bits of modulus */
  unsigned char modulus[MAX_RSA_MODULUS_LEN];                    /* modulus */
  unsigned char exponent[MAX_RSA_PUBLICE_LEN];           /* public exponent */
} R_RSA_PUBLIC_KEY;

extern int DigestInfo(unsigned char* pData, unsigned int ulDataLen, unsigned char *pDigest);

extern int RSAPublicEncrypt
  (unsigned char *output,unsigned int *outputLen,unsigned char *input,unsigned int inputLen,struct R_RSA_PUBLIC_KEY *publicKey, struct R_RANDOM_STRUCT *randomStruct);

//MD5加密，输出16字节。
extern unsigned char HASH_MD5(unsigned char *Input, unsigned int Ilen, unsigned char *Output);

//BASE64加密，输出在base64
extern char *base64_encode( const unsigned char * bindata, char * base64, int binlength );
//BASE64解密，输入base64为字符串\0结束,返回输出的长度
extern int base64_decodesl( const char * base64, unsigned char * bindata );

