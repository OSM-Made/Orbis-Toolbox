#pragma once

#if defined(SOFTWARE_VERSION_672)

/* Util */
#define addr_Xfast_syscall		                0x1C0
#define addr_sysvec                             0x1564E80
#define addr_prison0                            0x113B728
#define addr_rootvnode                          0x1B463E0

/* STD Lib */
#define addr_M_TEMP                             0x1556DA0
#define addr_M_MOUNT                            0x1543A60
#define addr_malloc								0x1D6680
#define addr_free					    		0x2FC0C0
#define addr_memcpy								0x28F800
#define addr_memset								0x8D6F0
#define addr_memcmp								0x31D250
#define addr_strlen                             0x2E8BC0
#define addr_strcpy								0x46B0B0
#define addr_strncpy                            0x21B800
#define addr_strcmp                             0x104BA0
#define addr_strstr                             0x3B0250
#define addr_sprintf                            0x26F980
#define addr_snprintf                           0x26FA40
#define addr_vsprintf                           0x26FA10
#define addr_vprintf                            0x26F7B0
#define addr_sscanf                             0x1C4840
#define addr_strdup                             0x110FA0
#define addr_realloc                            0x1D69A0
#define addr_kprintf                            0x26F740

/* File IO */
#define addr_vn_fullpath                        0x2C3570
#define addr_kern_rmdir                         0xF9E90
#define addr_kern_mkdir                         0xF9B10
#define addr_kern_open                          0xF49E0
#define addr_kern_mount                         0x790D0
#define addr_mount_argf                         0x78F20

/* Event Handling */
#define addr_eventhandler_register              0xD3670
#define addr_eventhandler_deregister            0xD3A00
#define addr_eventhandler_find_list             0xD3C00

/* Proc */
#define addr_allproc						    0x213C828
#define addr_proc_kill                          0x45FF30
#define addr_proc_rwmem							0x361310
#define addr_create_thread                      0x47AB60

/* Kproc */
#define addr_kproc_create					    0xD8F0
#define addr_kproc_exit                         0xDB60
#define addr_kproc_kthread_add				    0xDE50
#define addr_kthread_exit						0xE140
#define addr_kthread_suspend_check				0xE550
#define addr_pause                              0x86E80

/* Module Offsets */
#define addr_thr_initial_libkernel              0x
#define addr_thr_initial_libkernel_web          0x
#define addr_thr_initial_libkernel_sys          0x

/* Virtual Memory */
#define addr_vmspace_acquire_ref				0x2FC290
#define addr_vmspace_free						0x2FC0C0
#define addr_vm_map_lock_read					0x2FC430
#define addr_vm_map_unlock_read					0x2FC480
#define addr_vm_map_lookup_entry				0x2FCA70
#define addr_vm_map_findspace					0x2FF560
#define addr_vm_map_insert						0x2FD640
#define addr_vm_map_lock						0x2FC2E0
#define addr_vm_map_unlock 						0x2FC350
#define addr_vm_map_delete						0x2FEFA0
#define addr_vm_map_protect						0x3012F0

#endif