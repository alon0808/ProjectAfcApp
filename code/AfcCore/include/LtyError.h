#ifndef LTY_ERROR_H
#define LTY_ERROR_H

#include <errno.h>
#include <string.h>

typedef int LTY_RESULT;

typedef struct lty_err_s
{
    LTY_RESULT no;
    const char *str;
}lty_err_t;

#define LTY_SUCCESS   0
#define LTY_FAILURE   1001
#define LTY_IGNORE    1002

#define ESNDTOUT      1003
#define ERCVTOUT      1004
#define ERETRYMAX     1005
#define ENOTINIT      1006
#define EFILEHEAD     1007
#define ENOTSUPPORT   1008
#define ECONNETFAIL   1009
#define ESETOPTION    1010
#define ENOLOGIN      1011
#define EGETSIZE      1012
#define EGETFILE      1013
#define ECREATETHREAD 1014
#define EDATALEN      1015
#define EREINIT       1016
#define EVERIFY       1017
#define ENOCONN       1018
#define ELOGINSVR     1019
#define EINVMSG       1020
#define ENOINIT       1021
#define EINITFAIL     1022
#define EREAD         1023
#define ECREATE       1024
#define EWRITE        1025
#define ESEEK         1026
#define EMOUNT        1027
#define ETHREADEXIT   1028
#define ELACKPARAM    1029
// TTS error
#define ETEXTENC    2000 // 文本转化错误
#define ETTSGEN     2001 // TTS语音生成错误

extern char *LtyErrStr(LTY_RESULT ret);
extern char *LtyErrScanStr(lty_err_t err[], int cnt, LTY_RESULT ret);

#endif
