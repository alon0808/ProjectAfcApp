#include "Macro_Proj.h"

#if WHICH_PLATFORM == _LINUX_PLATFORM_V
#include "xDev.h"



static TFuncShowInUI s_funcShowInUi = NULL;
/**
* @Description - init device
*
*
* @Return (int,ERet_Code) - fail when less than 0, else is successful
*/
int xDev_Init(TFuncShowInUI funcShow) {
    if (s_funcShowInUi == NULL) {
        s_funcShowInUi = funcShow;
    }
    return Ret_OK;
}

void ShowInUI(int row, int column, char msg[], int textPosition) {
    if (s_funcShowInUi != NULL) {
        s_funcShowInUi(row, column, msg, textPosition);
    }
}

#endif
