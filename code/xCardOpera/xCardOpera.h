#ifndef _INCLUDED_CARD_OPERA_H
#define _INCLUDED_CARD_OPERA_H

#ifndef TYPEDEF_SCARD
#define TYPEDEF_SCARD
//----Error code for xScard----
enum EScardRetCode	// smart card return code
{
	XSCARD_S_SUCCESS = (TINT32)0,					// no error
	XSCARD_E_EXCLUSIVE = (TINT32)0xF0000001L,		// can not access because of exclusive 
	XSCARD_E_NO_MEMORY = (TINT32)0x80100006L,		//Not enough storage is available to complete this operation.
	XSCARD_E_TIMEOUT = (TINT32)0x8010000AL,		// process timeout
	XSCARD_E_PROTO_MISMATCH = (TINT32)0x8010000FL,		// protocol mismatch
	XSCARD_F_UNKNOWN_ERROR = (TINT32)0x80100014L,		// unknown error
	XSCARD_E_MULTI_CARD = (TINT32)0xF0100001L,	// multi cards
	XSCARD_ERROR = (TINT32)-32768,			// error when invoke function
};


//----For Wait smart card status----
enum ESCardStatus	// card state
{
	eXSCardStatus_Unknown = 0,		// unknown state
	eXSCardStatus_NoCard = 1,		// no card in reader
	eXSCardStatus_PresentCard = 2,	// at least one card in reader
	eXSCardStatus_MultiCard = 3,	// more than one card
	eXSCardStatus_WaitTimeout = 4	// operate timeout
};

enum ESCardType	// sak value
{
	eXSCardType_Invalid = 0x00,	// invalid
	eXSCardType_M1,		// M1	
	eXSCardType_CPU_M1,		// CPU as M1
	eXSCardType_CPU,	// CPU card
	eXSCardType_BANK,	// Bank card
	eXSCardType_SAM,	// SAM card
	eXSCardType_Unkown	// unknown card
};

enum emScardSlot {
	SSLOT_INVALID = 0,
	SSLOT_CONTACTLESS,
	SSLOT_SAM1,
	SSLOT_SAM2,
	SSLOT_SAM3,
	SSLOT_CONTACT,	// reserve for furture use
#if WHICH_PLATFORM == _WIN32_PLATFORM_V
	SSLOT_MAX_LEN = SSLOT_SAM1,
#else
	SSLOT_MAX_LEN = SSLOT_SAM3,
#endif

	SSLOT_SAM1_REL = 0x01,
	SSLOT_SAM2_REL = 0x02,
	SSLOT_CONTACT_REL = 0X0C,
	SSLOT_CONTACTLESS_REL = 0xFF
};

enum emScardM1Method {
	SMM_Invalid = 0,
	SMM_AuthBlock,
	SMM_ReadBlock,
	SMM_WriteBlock,
};

#endif

#define SW1SW2_SUCCESS	0x9000

#define BLOCK_SIZE	16	// size of M1 card data block
#define CARDID_SIZE	4	// size of M1 card data block

// format of buffer stored
#define SCARD_POS_LEN		0
#define SCARD_POS_VALUE		1

typedef struct {
	int hCard;
	unsigned char cardId[17];	// len + value
	//unsigned char slot_real;	// real slot use by device
	unsigned char slot;			// index of slot in array
	unsigned char sak;
	unsigned char cardtype;
	unsigned char ats_atr[33];	// len + value
}stCardInfor;

#ifndef _XCARD_OPERA_N_FUNCTION_C

#ifdef __cplusplus
extern "C"
{
#endif

	extern int xSCardInit(void);
	extern int xSCardGetReaderList(TINT8 *pReaders, TINT32 *pnReaders);
	extern int xSCardSetReaderName(const char *name);
	extern int xSCardWaitForPresentCard(TUINT8 slot, int baund, TUINT32 dwTimeout);
	extern TINT32 xSCardConnect(int slot);
	extern stCardInfor *xScardGetCardInfor(TUINT8 slot);
	extern int xSCardTransmit(TUINT8 slot, const stDataBuffer *pSend, stDataBuffer *pRecv, TUINT16 *pSw1sw2);
	extern int xSCardTransmit_M1(TUINT8 slot, int func, int block, TUINT8 isTypeA, const stDataBuffer *pKey, const stDataBuffer *pSend, stDataBuffer *pRecv);
#ifdef __cplusplus
};
#endif

#endif

#endif



