#include "i_interface.h"
#include "st_start.h"

#ifndef __MOBILE__
static_assert(sizeof(void*) == 8, "32 builds are not supported");
#endif

// Some global engine variables taken out of the backend code.
FStartupScreen* StartWindow;
SystemCallbacks sysCallbacks;
FString endoomName;
bool batchrun;
float menuBlurAmount;

