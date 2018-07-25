#ifndef LTY_TYPE_DEF_H
#define LTY_TYPE_DEF_H

typedef signed char    LTY_S8;
typedef unsigned char  LTY_U8;
typedef signed short   LTY_S16;
typedef unsigned short LTY_U16;
typedef signed int     LTY_S32;
typedef unsigned int   LTY_U32;
typedef char           LTY_CHAR;
typedef char*          LTY_STRING;
typedef void           LTY_VOID;
typedef void*          LTY_LPVOID;
typedef unsigned long long int LTY_U64;
typedef signed long long int   LTY_S64;

typedef enum
{
    LTY_FALSE = 0,
    LTY_TRUE  = 1
}LTY_BOOL;

typedef struct lty_buf_s
{
    LTY_LPVOID pBuf;
    LTY_U32 u32Size;

}lty_buf_t;

typedef LTY_S32 LTY_CHN;

#endif

