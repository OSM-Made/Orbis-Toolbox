#pragma once 

#if defined(SOFTWARE_VERSION_702)

/* Util */
#define addr_Xfast_syscall		                0x1C0
#define addr_ksysvec                            0x1A4F460
#define addr_kprison0                           0x113E398
#define addr_krootvnode                         0x22C5750

/* STD Lib */
#define addr_kmemcpy							0x2F040
#define addr_kmemset							0x2DFC20
#define addr_kmemcmp							0x207500
#define addr_ksprintf                           0xBC970
#define addr_kvsprintf                          0xBCA00
#define addr_kprintf                            0xBC730

#define addr_kmem_alloc							0x1170F0
#define addr_kernel_map							0x21C8EE0

#endif