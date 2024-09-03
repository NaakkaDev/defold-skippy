#include "skippy_private.h"
#include <dmsdk/sdk.h>

#if !defined(DM_PLATFORM_ANDROID)

int Skippy_PlatformStart()
{
    return 1;
}

int Skippy_PlatformStop()
{
    return 1;
}

int Skippy_GetSteps()
{
    return 1;
}

int Skippy_GetStepDetector()
{
    return 1;
}

#endif
