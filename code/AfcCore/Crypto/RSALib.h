//
//

#include "GLOBAL.H"
int RSAPublicEncryptRecovery
  (uchar *output,uint *outputLen,uchar *input,uint inputLen,uchar *modulebuf,uint modLen,uchar *exp,uint explen);

int Crypt_GenRSAKeyPair(
        char          *keyName,
        char          *password,
		unsigned char *DerPubkey,
		int           *DerPubkeyLen,
		unsigned char *DerPrikey,
		int           *DerPrikeyLen);

int Crypt_GetRSAPublicKey(
        char           *keyName,
		char           *password,
        unsigned char  *rsaPublicKey,
        int            *rsaPublicKeySize);



int	Crypt_Asyn_Enc_Data_By_Prikey(
							 unsigned char *indata,
							 int			indataLen,
							 unsigned char *outdata,
							 int		   *outdataLen,
							 unsigned char *PriKeyInfo,
							 int			PriKeyInfoSize);

int	Crypt_Asyn_Dec_Data_By_Pubkey(
							 unsigned char *indata,
							 int			indataLen,
							 unsigned char *outdata,
							 int		   *outdataLen,
							 unsigned char *PubKeyInfo,
							 int			PubKeyInfoSize);




int	Crypt_Asyn_Enc_Data_By_Pubkey(
							 unsigned char *indata,
							 int			indataLen,
							 unsigned char *outdata,
							 int		   *outdataLen,
							 unsigned char *PubKeyInfo,
							 int			PubKeyInfoSize);
int	   Pubkey_Enc(
							 unsigned char *indata,
							 int			indataLen,
							 unsigned char *outdata,
							 int		   *outdataLen,
							 unsigned char *PubKeyInfo,
							 int			PubKeyInfoSize);


int	Crypt_Asyn_Dec_Data_By_Prikey(
							 unsigned char *indata,
							 int			indataLen,
							 unsigned char *outdata,
							 int		   *outdataLen,
							 unsigned char *PriKeyInfo,
							 int			PriKeyInfoSize);



int PubkeyEncode(unsigned char *derkey,unsigned char *exp,
				 int exponent,unsigned char *modubuf,int modulus);
int	   Pubkey_Recovery(
					   unsigned char *indata,
					   int			indataLen,
					   unsigned char *outdata,
					   int		   *outdataLen,
					   unsigned char *modulubuf,
					   int			modLen,
					   unsigned char *exp,
							 int explen);