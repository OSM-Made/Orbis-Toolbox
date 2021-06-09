#include "Common.h"
#include "Resolver/Patches.h"

void Install_505()
{
	uint8_t *kmem;

	// Verbose Panics
	kmem = (uint8_t *)&gKernelBase[0x00171627];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x65;
	kmem[6] = 0x8B;
	kmem[7] = 0x34;
	
	// Enable rwx mapping
	kmem = (uint8_t *)&gKernelBase[0x000FCD48];
	kmem[0] = 0x07;
	
	kmem = (uint8_t *)&gKernelBase[0x000FCD56];
	kmem[0] = 0x07;

	// Patch copyin/copyout to allow userland + kernel addresses in both params
	kmem = (uint8_t *)&gKernelBase[0x001EA767];
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	
	kmem = (uint8_t *)&gKernelBase[0x001EA682];
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// Patch copyinstr
	kmem = (uint8_t *)&gKernelBase[0x001EAB93];
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t *)&gKernelBase[0x001EABC3];
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// Patch memcpy stack
	kmem = (uint8_t *)&gKernelBase[0x001EA53D];
	kmem[0] = 0xEB;
}