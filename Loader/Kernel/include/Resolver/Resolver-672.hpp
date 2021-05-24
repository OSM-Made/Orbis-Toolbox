#pragma once

#if defined(SOFTWARE_VERSION_672)

/* Util */
#define addr_Xfast_syscall		                0x1C0
#define addr_sysvec                             0x1A8A398
#define addr_prison0                            0x113E518
#define addr_rootvnode                          0x2300320

/* STD Lib */
#define addr_M_TEMP                             0x1540EB0
#define addr_M_MOUNT                            0x1A90CA0
#define addr_malloc								0xD7A0
#define addr_free					    		0xD9A0
#define addr_memcpy								0x3C15B0
#define addr_memset								0x1687D0
#define addr_memcmp								0x207E40
#define addr_strlen                             0x2433E0
#define addr_strcpy								0x2390C0
#define addr_strncpy                            0x329010
#define addr_strcmp                             0x341810
#define addr_strstr                             0x4817F0
#define addr_sprintf                            0x1234C0
#define addr_snprintf                           0x123590
#define addr_vsprintf                           0x123550
#define addr_vprintf                            0x1232F0
#define addr_sscanf                             0x243810
#define addr_strdup                             0x2504C0
#define addr_realloc                            0xDAD0
#define addr_kprintf                             0x123280

/* File IO */
#define addr_vn_fullpath                        0x2F0C40
#define addr_kern_rmdir                         0x4A3DF0
#define addr_kern_mkdir                         0x4A3A80
#define addr_kern_open                          0x49E990
#define addr_kern_mount                         0x442F90
#define addr_mount_argf                         0x442DE0

/* Event Handling */
#define addr_eventhandler_register              0x402E80
#define addr_eventhandler_deregister            0x403220
#define addr_eventhandler_find_list             0x403420

/* Proc */
#define addr_allproc						    0x22BBE80
#define addr_proc_rwmem							0x10EE10
#define addr_create_thread                      0x4A6FB0

/* Kproc */
#define addr_kproc_create					    0x8A0A0
#define addr_kproc_exit                         0x8A310
#define addr_kproc_kthread_add				    0x8AE20
#define addr_kthread_exit						0x8A8F0
#define addr_kthread_suspend_check				0x8AD10
#define addr_pause                              0x22A080

/* Module Offsets */
#define addr_thr_initial_libkernel              0x435420
#define addr_thr_initial_libkernel_web          0x435420
#define addr_thr_initial_libkernel_sys          0x435830

/* Virtual Memory */
#define addr_vmspace_acquire_ref				0x44CB90
#define addr_vmspace_free						0x44C9C0
#define addr_vm_map_lock_read					0x44CD40
#define addr_vm_map_unlock_read					0x44CD90
#define addr_vm_map_lookup_entry				0x44D330
#define addr_vm_map_findspace					0x44FE60
#define addr_vm_map_insert						0x44DEF0
#define addr_vm_map_lock						0x44CBF0
#define addr_vm_map_unlock 						0x44CC60
#define addr_vm_map_delete						0x44F8A0
#define addr_vm_map_protect						0x451BF0

#endif