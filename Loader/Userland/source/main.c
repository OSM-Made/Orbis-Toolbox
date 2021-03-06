#include "Common.h"

extern char _binary_Resources_Kernel_bin_start[];
extern char _binary_Resources_Kernel_bin_end[];
struct Backup_Jail bkJail;
uint8_t* gKernelBase;

int install_elf(struct thread *td)
{
	//Get kernel Base.
    uint8_t* KernelBase = (uint8_t*)(__readmsr(0xC0000082) - addr_Xfast_syscall);

    if(!KernelBase)
		return 0;

    size_t msize = 0;
	if (elf_mapped_size(_binary_Resources_Kernel_bin_start, &msize)) {
		return 1;
	}

	uint64_t (*kmem_alloc)(vm_map_t map, uint64_t size) = (void *)(KernelBase + addr_kmem_alloc);
	vm_map_t kernel_map = *(vm_map_t *)(KernelBase + addr_kernel_map);

	int s = (msize + 0x3FFFull) & ~0x3FFFull;
	void *payloadbase = (void*)kmem_alloc(kernel_map, s);
	if (!payloadbase) {
		return 1;
	}

	klog("Kernel ELF:\nBase -> 0x%llX\nSize: -> %llX", _binary_Resources_Kernel_bin_start, (size_t)(_binary_Resources_Kernel_bin_end - _binary_Resources_Kernel_bin_start));

	int r = 0;
	int (*payload_entry)(uint64_t kernelbase);

	if ((r = load_elf(_binary_Resources_Kernel_bin_start, (size_t)(_binary_Resources_Kernel_bin_end - _binary_Resources_Kernel_bin_start), payloadbase, msize, (void **)&payload_entry))) {
		return r;
	}

	if (payload_entry(KernelBase)) {
		return 1;
	}

	klog("Loaded Kernel ELF Sucess!");

	//Restore the original jail for the current Process.
	RestoreJail(td->td_proc, bkJail);

	klog("RestoreJail() -> Sucess!");

	return 0;    
}

int jailbreak_proc(struct thread *td)
{
	//Get kernel Base.
    gKernelBase = (uint8_t*)(__readmsr(0xC0000082) - addr_Xfast_syscall);

    if(!gKernelBase)
		return 0;

    //Resolve Function Addresses and install Patches.
	Kern_Resolve();
	Install_Patches();

	klog("KernBase: 0x%llX", gKernelBase);

	//Jailbreak current Process.
	Jailbreak(td->td_proc, &bkJail);

	klog("Jailbreak() -> Sucess!");

	return 0;
}

int _main(void) 
{
	syscall(601, 7, "Hello World.\n", 0);

	//Jailbreak the current proc to write to root.
	syscall(11, jailbreak_proc);

    //Resolve userland Functions
	Userland_Resolve();

    //Copy Resources.
	Install_Resources();

    //load kernel elf.
	syscall(11, install_elf);

	Log("Loading Completed.");

    return 0;
}