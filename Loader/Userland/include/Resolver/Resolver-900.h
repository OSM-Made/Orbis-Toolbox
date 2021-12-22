#pragma once 

#if defined(SOFTWARE_VERSION_900)

/* Util */
#define addr_Xfast_syscall		                0x000001C0
#define addr_ksysvec                            0x01528E30
#define addr_kprison0                           0x0111F870
#define addr_krootvnode                         0x021EFF20

/* STD Lib */
#define addr_kmemcpy							0x002714B0
#define addr_kmemset							0x001496C0
#define addr_kmemcmp							0x00271E20
#define addr_ksprintf                           0x000B7C70
#define addr_kvsprintf                          0x000B7D00
#define addr_kprintf                            0x000B7A30

#define addr_kmem_alloc							0x0037BE70
#define addr_kernel_map							0x02268D48

#endif