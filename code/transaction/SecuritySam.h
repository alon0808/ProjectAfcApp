#ifndef _INCLUDED_SECURITY_SAM_H
#define _INCLUDED_SECURITY_SAM_H

#define SAM_CARD_0  0
#define SAM_CARD_1  1
#define SAM_CARD_2  2
#define SAM_CARD_3  3

#define SAM_SLOT_GS			SSLOT_SAM1
#define SAM_SLOT_ZJB		SSLOT_SAM2

#ifdef __cplusplus
extern "C"
{
#endif
	extern int SAM_Verify(int slot);
	extern int SAM_M1_GetMiKey(stDataBuffer *pCardId, stM1_PublicSecBlock4 *pblock4);

#ifdef __cplusplus
};
#endif


#endif



