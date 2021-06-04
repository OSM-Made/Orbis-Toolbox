#pragma once 

#if defined(SOFTWARE_VERSION_755)

/* Util */
#define addr_Xfast_syscall		                0x1C0
#define addr_ksysvec                            0x1564E80
#define addr_kprison0                           0x113B728
#define addr_krootvnode                         0x1B463E0

/* STD Lib */
#define addr_kmemcpy							0x28F800
#define addr_kmemset							0x8D6F0
#define addr_kmemcmp							0x31D250
#define addr_ksprintf                           0x26F980
#define addr_kvsprintf                          0x26FA10
#define addr_kprintf                            0x26F740

#define addr_kmem_alloc							0x1753E0
#define addr_kernel_map							0x21405B8

#endif