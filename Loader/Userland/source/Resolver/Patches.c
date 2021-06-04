#include "Common.h"
#include "Resolver/Patches.h"
#include "../../Common/Settings.h"

//
// Credits to Mira for the patches. Was just easier to slap them in :P
//

void Install_Patches()
{
    if(!gKernelBase)
        return;

    uint64_t CR0 = __readcr0();
	__writecr0(CR0 & ~CR0_WP);

    #if defined(SOFTWARE_VERSION_505) || defined(SOFTWARE_VERSION_NA)
    Install_505();
    #elif defined(SOFTWARE_VERSION_672)
    Install_672();
    #elif defined(SOFTWARE_VERSION_702)
    Install_702();
    #elif defined(SOFTWARE_VERSION_755)
    Install_755();
    #endif

    __writecr0(CR0);

    klog("Install_Patches() -> Sucess!");
}