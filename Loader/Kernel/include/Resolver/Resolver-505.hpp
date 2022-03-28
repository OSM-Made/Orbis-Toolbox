#pragma once

#if defined(SOFTWARE_VERSION_505) || defined(SOFTWARE_VERSION_NA)

/* Util */
#define addr_Xfast_syscall		                0x1C0
#define addr_sysvec                             0x19BBCD0
#define addr_prison0                            0x10986A0
#define addr_rootvnode                          0x22C1A70

/* STD Lib */
#define addr_M_TEMP                             0x14B4110
#define addr_M_MOUNT                            0x19BF300
#define addr_malloc								0x10E250
#define addr_free					    		0x10E460
#define addr_memcpy								0x1EA530
#define addr_memset								0x3205C0
#define addr_memcmp								0x50AC0
#define addr_strlen                             0x3B71A0
#define addr_strcpy								0x8F250
#define addr_strncpy                            0x3C0B0
#define addr_strcmp                             0x1D0FD0
#define addr_strstr                             0x17DFB0
#define addr_sprintf                            0x436280
#define addr_snprintf                           0x436350
#define addr_vsprintf                           0x436310
#define addr_vprintf                            0x4360B0
#define addr_sscanf                             0x175900
#define addr_strdup                             0x1C1C30
#define addr_realloc                            0x10E590
#define addr_kprintf                            0x436040

/* File IO */
#define addr_vn_fullpath                        0xA11A0
#define addr_kern_rmdir                         0x340EE0
#define addr_kern_mkdir                         0x340B70
#define addr_kern_open                          0x33B9B0
#define addr_kern_mount                         0x1E1920
#define addr_mount_argf                         0x1E1780

/* Event Handling */
#define addr_eventhandler_register              0x1EC400
#define addr_eventhandler_deregister            0x1EC790
#define addr_eventhandler_find_list             0x1EC980

/* Proc */
#define addr_allproc						    0x2382FF8
#define addr_proc_kill                          0xD41C0
#define addr_proc_rwmem							0x30D150
#define addr_create_thread                      0x1BE1F0

/* Kproc */
#define addr_kproc_create					    0x137DF0
#define addr_kproc_exit                         0x138060
#define addr_kproc_kthread_add				    0x138B70
#define addr_kthread_exit						0x138640
#define addr_kthread_suspend_check				0x138A60
#define addr_pause                              0x3FB920

/* Module Offsets */
#define addr_thr_initial_libkernel              0x84C20
#define addr_thr_initial_libkernel_web          0x84C20
#define addr_thr_initial_libkernel_sys          0x89030

/* Virtual Memory */
#define addr_vmspace_acquire_ref				0x19EF90
#define addr_vmspace_free						0x19EDC0
#define addr_vm_map_lock_read					0x19F140
#define addr_vm_map_unlock_read					0x19F190
#define addr_vm_map_lookup_entry				0x19F760
#define addr_vm_map_findspace					0x1A1F60
#define addr_vm_map_insert						0x1A0280
#define addr_vm_map_lock						0x19EFF0
#define addr_vm_map_unlock 						0x19F060
#define addr_vm_map_delete						0x1A19D0
#define addr_vm_map_protect						0x1A3A50

#endif
