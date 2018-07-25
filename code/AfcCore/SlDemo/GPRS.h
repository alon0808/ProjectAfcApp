
#ifndef _INCLUDE_GPRS_H_
#define _INCLUDE_GPRS_H_

#ifdef _cplusplus
extern "C"
{
#endif

extern int GPRS_main(char *serv_ip, int serv_port, char *sendbuf, int sendlen, char *revbuf, int *revlen);

#ifdef _cplusplus
}
#endif

#endif