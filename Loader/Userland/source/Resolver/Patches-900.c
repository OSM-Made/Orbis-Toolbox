#include "Common.h"
#include "Resolver/Patches.h"

void Install_900()
{
    // Use "kmem" for all patches
	uint8_t *kmem;

    // Enable UART
	kmem = (uint8_t *)&gKernelBase[0x0152BF5D];
	kmem[0] = 0x00;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;

	// Patch sys_dynlib_dlsym: Allow from anywhere
	kmem = (uint8_t *)&gKernelBase[0x0023B67F];
	kmem[0] = 0xEB;
	kmem[1] = 0x4C;

	kmem = (uint8_t *)&gKernelBase[0x00221B40];
	kmem[0] = 0x31;
	kmem[1] = 0xC0;
	kmem[2] = 0xC3;

	// Patch sys_mmap: Allow RWX (read-write-execute) mapping
	kmem = (uint8_t *)&gKernelBase[0x0016632A];
	kmem[0] = 0x37;
	kmem[3] = 0x37;

	// Patch setuid: Don't run kernel exploit more than once/privilege escalation
	kmem = (uint8_t *)&gKernelBase[0x000019FF];
	kmem[0] = 0xB8;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;

	// Enable RWX (kmem_alloc) mapping
	kmem = (uint8_t *)&gKernelBase[0x0037BF3C];
	kmem[0] = 0x07;

	kmem = (uint8_t *)&gKernelBase[0x0037BF44];
	kmem[0] = 0x07;

	// Patch copyin/copyout: Allow userland + kernel addresses in both params
	// copyin
	kmem = (uint8_t *)&gKernelBase[0x002716F7];
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t *)&gKernelBase[0x00271703];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	// copyout
	kmem = (uint8_t *)&gKernelBase[0x00271602];
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t *)&gKernelBase[0x0027160E];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	// Patch copyinstr
	kmem = (uint8_t *)&gKernelBase[0x00271BA3];
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t *)&gKernelBase[0x00271BAF];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	kmem = (uint8_t *)&gKernelBase[0x00271BE0];
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// Patch memcpy stack
	kmem = (uint8_t *)&gKernelBase[0x002714BD];
	kmem[0] = 0xEB;

	// Patch mprotect: Allow RWX (mprotect) mapping
	kmem = (uint8_t *)&gKernelBase[0x00080B8B];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;
}