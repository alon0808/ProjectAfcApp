#ifndef _INCLUDE_DEBUG_INFOR_H_
#define _INCLUDE_DEBUG_INFOR_H_


#ifdef __cplusplus
extern "C" {
#endif

	extern void debugstring(const char *str);
	extern void debugdata(unsigned char *value, unsigned int uclen, unsigned char mode);

#ifdef __cplusplus
}
#endif


#endif
