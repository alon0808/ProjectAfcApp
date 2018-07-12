#ifndef UI_ACCESSINTERFACE_H
#define UI_ACCESSINTERFACE_H

typedef enum{
    FUNC_INVALID=0,           // ,0x0000
    FUNC_SHOWTEXT=1,          // 显示文本,0x0001
}emCardExeRet;

typedef enum{
    FPARAM_INVALID=0,         // ,0x0000
    FPARAM_ROW=1,             // tap card success,0x0001
    FPARAM_COLUMN=2,          // 重新寻卡,0x0002
    FPARAM_COLUMN=3,          // 黑名单卡标识,0x0003
    CARD_NO_SYSTEM=4,         // ,0x0004
    CARD_MONEY=5,             // m1 普通钱包处理,0x0005
    CARD_INVALID=6,           // 灰名单标识,0x0006
    CARD_MONTH=7,             // ,0x0007
}emFuncParam;

#ifdef __cplusplus
extern "C"{
#endif

extern int func_UiAccess(unsigned char* pParam);

#ifdef __cplusplus
}
#endif

#endif // UI_ACCESSINTERFACE_H

