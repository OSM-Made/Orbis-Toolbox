#pragma once

#if defined(SOFTWARE_VERSION_505) || defined(SOFTWARE_VERSION_NA)

/* Util */
#define addr_Xfast_syscall		                0x1C0
#define addr_sysvec                             0x19BBCD0
#define addr_prison0                            0x10986A0
#define addr_rootvnode                          0x22C1A70

/* STD Lib */
#define addr_memcpy								0x1EA530
#define addr_memset								0x3205C0
#define addr_memcmp								0x50AC0
#define addr_sprintf                            0x436280
#define addr_vsprintf                           0x436310
#define addr_kprintf                            0x0

#define addr_kmem_alloc							0xFCC80
#define addr_kernel_map							0x1AC60E0

#endif