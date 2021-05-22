#pragma once 

#if defined(SOFTWARE_VERSION_672)

/* Util */
#define addr_Xfast_syscall		                0x1C0
#define addr_ksysvec                            0x1A8A398
#define addr_kprison0                           0x113E518
#define addr_krootvnode                         0x2300320

/* STD Lib */
#define addr_kmemcpy							0x3C15B0
#define addr_kmemset							0x1687D0
#define addr_kmemcmp							0x207E40
#define addr_ksprintf                           0x1234C0
#define addr_kvsprintf                          0x123550
#define addr_kprintf                            0x123280

#define addr_kmem_alloc							0x250730
#define addr_kernel_map							0x220DFC0

#endif