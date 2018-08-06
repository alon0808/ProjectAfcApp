#ifndef HANDANCORE_H
#define HANDANCORE_H

#include "handancore_global.h"
#include "DllDef.h"

#ifdef __cplusplus
extern "C" {
#endif

extern AFC_CORE__API int StartApp(int argc, const char **argv);

class HANDANCORESHARED_EXPORT HandanCore
{

public:
    HandanCore();
};

#ifdef __cplusplus
}
#endif

#endif // HANDANCORE_H
