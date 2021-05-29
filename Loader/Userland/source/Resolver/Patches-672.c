#include "Common.h"
#include "Resolver/Patches.h"

void Install_672()
{
    uint8_t *kmem;
    
    // Patch dynlib_dlsym
	kmem = (uint8_t*)&gKernelBase[0x1D895A];
	kmem[0] = 0xE9;
	kmem[1] = 0xC7;
	kmem[2] = 0x01;
	kmem[3] = 0x00;
	kmem[4] = 0x00;

	// Patch a function called by dynlib_dlsym
	kmem = (uint8_t*)&gKernelBase[0x0041A2D0];
	kmem[0] = 0x31; // xor eax, eax
	kmem[1] = 0xC0;
	kmem[2] = 0xC3;	// ret

	// Patch sys_mmap
	kmem = (uint8_t*)&gKernelBase[0x000AB57A];
	kmem[0] = 0x37; // mov     [rbp+var_61], 33h ; '3'
	kmem[3] = 0x37; // mov     sil, 33h ; '3'

	// patch sys_setuid
	kmem = (uint8_t*)&gKernelBase[0x0010BED0]; // call    priv_check_cred; overwrite with mov eax, 0
	kmem[0] = 0xB8; // mov eax, 0
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;

	// patch sys_mprotect
	kmem = (uint8_t*)&gKernelBase[0x00451DB8]; // jnz     loc_FFFFFFFF82652426; nop it out
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// Enable rwx mapping in kmem_alloc
	kmem = (uint8_t *)&gKernelBase[0x002507F5];
	kmem[0] = 0x07; // set maxprot to RWX

	kmem = (uint8_t *)&gKernelBase[0x00250803];
	kmem[0] = 0x07; // set maxprot to RWX

	// Patch copyin/copyout to allow userland + kernel addresses in both params
	// copyin
	kmem = (uint8_t *)&gKernelBase[0x003C17F7];
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t *)&gKernelBase[0x003C1803];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	// copyout
	kmem = (uint8_t *)&gKernelBase[0x003C1702];
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t *)&gKernelBase[0x003C170E];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	// Enable MAP_SELF

	// Patches: sceSblACMgrHasMmapSelfCapability
	kmem = (uint8_t *)&gKernelBase[0x00233C40];
	kmem[0] = 0xB8;
	kmem[1] = 0x01;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;
	kmem[5] = 0xC3;

	// Patches: sceSblACMgrIsAllowedToMmapSelf
	kmem = (uint8_t *)&gKernelBase[0x00233C50];
	kmem[0] = 0xB8;
	kmem[1] = 0x01;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;
	kmem[5] = 0xC3;

	// Patches: call    sceSblAuthMgrIsLoadable in vm_mmap2 (right above the only call to allowed to mmap self)
	kmem = (uint8_t *)&gKernelBase[0x000AD2E4]; // xor eax, eax; nop; nop;
	kmem[0] = 0x31;
	kmem[1] = 0xC0;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;

	// Patch copyinstr
	kmem = (uint8_t *)&gKernelBase[0x003C1CA3];
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t *)&gKernelBase[0x003C1CAF];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	// Patch memcpy stack
	kmem = (uint8_t *)&gKernelBase[0x003C15BD];
	kmem[0] = 0xEB;

	// ptrace patches
	kmem = (uint8_t *)&gKernelBase[0x0010F879];
	kmem[0] = 0xEB;

	// Enable debug rif's
	kmem = (uint8_t*)&gKernelBase[0x66AEB0];
	kmem[0] = 0xB0;
	kmem[1] = 0x01;
	kmem[2] = 0xC3;
	kmem[3] = 0x90;

	// Enable debug rifs 2
	kmem = (uint8_t*)&gKernelBase[0x66AEE0];
	kmem[0] = 0xB0;
	kmem[1] = 0x01;
	kmem[2] = 0xC3;
	kmem[3] = 0x90;

	// Disable pfs checks
	kmem = (uint8_t*)&gKernelBase[0x6A8EB0];
	kmem[0] = 0x31;
	kmem[1] = 0xC0;
	kmem[2] = 0xC3;
	kmem[3] = 0x90;

	// Enable *all* debugging logs (in vprintf)
	// Patch by: SiSTRo
	kmem = (uint8_t*)&gKernelBase[0x00123367];
	kmem[0] = 0xEB; // jmp +0x3D
	kmem[1] = 0x3B;
}