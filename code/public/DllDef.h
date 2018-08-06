#ifndef _INCLUDE_DLL_DEF_H
#define _INCLUDE_DLL_DEF_H

#ifdef WIN32
#ifdef SYSTEM_RELATION_EXPORTS
#define SYSTEM_RELATION_API __declspec(dllexport)
#else
#define SYSTEM_RELATION_API __declspec(dllimport)
#pragma comment(lib,"SystemRelation.lib")
#endif
#else //NON WINDOWS
#define SYSTEM_RELATION_API __attribute__ ((visibility ("default")))
#endif


#ifdef WIN32
#ifdef AFC_CORE_EXPORTS
#define AFC_CORE__API __declspec(dllexport)
#else
#define AFC_CORE__API __declspec(dllimport)
#pragma comment(lib,"AfcCore.lib")
#endif
#else //NON WINDOWS
#define AFC_CORE__API __attribute__ ((visibility ("default")))
#endif


#endif
