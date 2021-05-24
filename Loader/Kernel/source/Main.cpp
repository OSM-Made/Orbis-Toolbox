#include "Common.hpp"
#include "kproc.hpp"
#include "Loader.hpp"

extern "C" void _main(uint64_t p)
{
    //Get kernel base address.
    uint64_t KernelBase = (__readmsr(0xC0000082) - addr_Xfast_syscall);

    if(!KernelBase)
		return;

    //Use kernel base address to resolve function addresses.
    Resolve(KernelBase);

    //Init kernel process.
    kproc_Init();

    //Init Loader.
    Loader_Init("SceShellUI");

    //Load Toolbox to shellui Initially.
    Load_SPRX(SPRX_PATH);

    //destroy the loader.
    Loader_Term();

    klog("!!! HELLO HELLO FROM KERNEL !!!");
}