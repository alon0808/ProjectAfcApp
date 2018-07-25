#ifndef LTY_LOG_H
#define LTY_LOG_H

#include <stdio.h>
#include <syslog.h>

#define COLOR_RED       "\033[0;31m"
#define COLOR_GREEN     "\033[0;32m"
#define COLOR_YELLOW    "\033[0;33m"
#define COLOR_DARKBLUE  "\033[0;34m"
#define COLOR_PURPLE    "\033[0;35m"
#define COLOR_LIGHTBLUE "\033[0;36m"
#define COLOR_WHITE     "\033[0;39m"

#ifndef LTY_DBG_PREFIX
#define LTY_DBG_PREFIX ""
#endif

#ifndef LTY_LOG_PREFIX
#define LTY_LOG_PREFIX ""
#endif

extern void LtyLogOut(int priority, const char *fmt, ...);

#define ERROUT(fmt, args...)  do{LtyLogOut(LOG_ERR, "[ERR]" LTY_LOG_PREFIX fmt, ##args); \
                                printf(COLOR_RED LTY_LOG_PREFIX "%s: " fmt COLOR_WHITE, __func__, ##args);}while(0)
#define LOGOUT(fmt, args...)  do{LtyLogOut(LOG_INFO, "[LOG]" LTY_LOG_PREFIX fmt, ##args); \
                                printf(COLOR_GREEN LTY_LOG_PREFIX "%s: " fmt COLOR_WHITE, __func__, ##args);}while(0)
#define WAROUT(fmt, args...)  do{LtyLogOut(LOG_WARNING, "[WAR]" LTY_LOG_PREFIX fmt, ##args); \
                                printf(COLOR_YELLOW LTY_LOG_PREFIX "%s: " fmt COLOR_WHITE, __func__, ##args);}while(0)

#ifdef LTY_DEBUG
#define DBGOUT(fmt, args...)  printf(LTY_DBG_PREFIX "%s: " fmt, __func__, ##args)
#define DBGPOUT(fmt, args...) printf(COLOR_PURPLE LTY_DBG_PREFIX "%s: " fmt COLOR_WHITE, __func__, ##args)
#define DBGBOUT(fmt, args...) printf(COLOR_LIGHTBLUE LTY_DBG_PREFIX "%s: " fmt COLOR_WHITE, __func__, ##args)
#define DBGDOUT(fmt, args...) printf(COLOR_DARKBLUE LTY_DBG_PREFIX "%s: " fmt COLOR_WHITE, __func__, ##args)
#else
#define DBGOUT(fmt, args...)
#define DBGPOUT(fmt, args...)
#define DBGBOUT(fmt, args...)
#define DBGDOUT(fmt, args...)
#endif

#endif

