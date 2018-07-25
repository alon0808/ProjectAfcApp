/// @file debug_.h
/// @brief 
/// @author Jesse Xu <zhimo811@gmail.com>
/// 0.01
/// @date 2017-09-13

#include <stdio.h>

#define _DEBUG_
#ifdef _DEBUG_
#define _DBG_(format,...) printf("\033[44;37m [XU]%s : %d: "format"\033[0m \n", __func__, __LINE__, ##__VA_ARGS__)
#define _WARN_(format,...) printf("\033[43;37m [XU]%s: %d: "format"\033[0m \n", __func__, __LINE__, ##__VA_ARGS__)
#define _ERR_(format,...) printf("\033[41;37m [XU]%s: %d: "format"\033[0m \n", __func__, __LINE__, ##__VA_ARGS__)
#else
#define _DBG_(format,...) 
#define _WARN_(format,...)
#define _ERR_(format,...) 
#endif
