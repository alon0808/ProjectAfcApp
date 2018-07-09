#include "Macro_Proj.h"
#include "ExchangeData.h"
#include "UtilityProc.h"
#include "xSys.h"

#include <string.h>
#include <stdio.h>


static TLPVOID s_threadDisplay = NULL;

void initExchange() {
	shm_init(MNT_DISK0"/afc/bin", 0, 10); //ipc with watch dog process(wtd)
	shm_init(MNT_DISK0"/afc/bin", 5, 64); //ipc with audio play process(ipc_host)
}



#if SPLIT_BAR	// other buffer
#endif

