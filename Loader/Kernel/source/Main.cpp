#include "Common.hpp"
#include "kproc.hpp"
#include "Loader.hpp"
#include "Util/Proc.hpp"

extern "C" void _main(uint64_t arg)
{
  //Get kernel base address.
  uint64_t KernelBase = (__readmsr(0xC0000082) - addr_Xfast_syscall);

  if(!KernelBase)
  return;

  //Use kernel base address to resolve function addresses.
  Resolve(KernelBase);

  klog("!!! HELLO HELLO FROM KERNEL !!!");

  //Init kernel process.
  kproc_Init();

  klog("Restarting UI...");
  proc* ShellUI = proc_find_by_name("SceShellUI");
  if(ShellUI)
  {
    //proc_kill(ShellUI, "Orbis Toolbox");
  }
}