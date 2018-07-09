#include "Macro_Proj.h"
#include "ManageThread.h"
#include "UtilityProc.h"
#include "xSys.h"

#include <string.h>
#include <stdio.h>



typedef struct  {
	TUINT32 threadId;
	TLPVOID threadHand;
	TUINT32 threadStatus;
}sttThreadUnit;



#if SPLIT_BAR	// other buffer
#endif

