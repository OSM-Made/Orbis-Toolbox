#include "Common.hpp"
#include "kproc.hpp"
#include "Loader.hpp"
#include "Util/Proc.hpp"

extern "C" int _main(uint64_t arg)
{
  //Get kernel base address.
  uint64_t KernelBase = (__readmsr(0xC0000082) - addr_Xfast_syscall);

  if(!KernelBase)
    return 1;

  //Use kernel base address to resolve function addresses.
  Resolve(KernelBase);

  klog("!!! HELLO HELLO FROM KERNEL !!!");

  //Init kernel process.
  kproc_Init();

  return 0;
}