#ifndef _INCLUDED_xAppConfig_H
#define _INCLUDED_xAppConfig_H

#if WHICH_PLATFORM == _WIN32_PLATFORM_V
#define MNT_DISK0  "/mnt/disk0"
#define MNT_DISK1  "/mnt/disk1"
#define MNT_DISK3  "/mnt/disk3"
#else
#define MNT_DISK0  "/mnt/disk0"
#define MNT_DISK1  "/mnt/disk1"
#define MNT_DISK3  "/mnt/disk3"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	extern int installAfcParam(char curPath[]);

#ifdef __cplusplus
}
#endif

#endif
