#include "Common.h"
#include "Resolver/Patches.h"
#include "../../Common/Settings.h"

void Install_Patches(uint64_t kernbase)
{
    if(!kernbase)
        return;

    #if defined(SOFTWARE_VERSION_505) || defined(SOFTWARE_VERSION_NA)
    Install_505(kernbase);
    #elif defined(SOFTWARE_VERSION_672)
    Install_672((uint8_t*)kernbase);
    #elif defined(SOFTWARE_VERSION_702)
    Install_702(kernbase);
    #elif defined(SOFTWARE_VERSION_755)
    Install_755(kernbase);
    #endif
}