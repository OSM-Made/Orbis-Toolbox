#pragma once

#if defined(SOFTWARE_VERSION_900)

/* Util */
#define addr_Xfast_syscall		                0x000001C0
#define addr_sysvec                             0x01528E30
#define addr_prison0                            0x0111F870
#define addr_rootvnode                          0x021EFF20

/* STD Lib */
#define addr_M_TEMP                             0x015621E0
#define addr_M_MOUNT                            0x015279F0
#define addr_malloc								0x00301B20
#define addr_free					    		0x00301CE0
#define addr_memcpy								0x002714B0
#define addr_memset								0x001496C0
#define addr_memcmp								0x00271E20
#define addr_strlen                             0x0030F450
#define addr_strcpy								0x00189F80
#define addr_strncpy                            0x0041E380
#define addr_strcmp                             0x0040E700
#define addr_strstr                             0x00487AB0
#define addr_sprintf                            0x000B7C70
#define addr_snprintf                           0x000B7D30
#define addr_vsprintf                           0x000B7D00
#define addr_vprintf                            0x000B7AA0
#define addr_sscanf                             0x0026C8D0
#define addr_strdup                             0x00278540
#define addr_realloc                            0x00301DE0
#define addr_kprintf                            0x000B7A30

/* File IO */
#define addr_vn_fullpath                        0x002648C0
#define addr_kern_rmdir                         0x001DF3A0
#define addr_kern_mkdir                         0x001DF020
#define addr_kern_open                          0x001D9EE0
#define addr_kern_mount                         0x0004DF50
#define addr_mount_argf                         0x0004DDB0

/* Event Handling */
#define addr_eventhandler_register              0x000F8370
#define addr_eventhandler_deregister            0x000F8700
#define addr_eventhandler_find_list             0x000F88F0

/* Proc */
#define addr_allproc						    0x01B946E0
#define addr_proc_kill                          0x00029780
#define addr_proc_rwmem							0x0041EB00
#define addr_create_thread                      0x001ED670

/* Kproc */
#define addr_kproc_create					    0x000969E0
#define addr_kproc_exit                         0x00096C50
#define addr_kproc_kthread_add				    0x00097750
#define addr_kthread_exit						0x00097230
#define addr_kthread_suspend_check				0x00097640
#define addr_pause                              0x00453EA0

/* Module Offsets */
#define addr_thr_initial_libkernel              0x0008E430
#define addr_thr_initial_libkernel_web          0x0008E430
#define addr_thr_initial_libkernel_sys          0x0008E830

/* Virtual Memory */
#define addr_vmspace_acquire_ref				0x0007B9E0
#define addr_vmspace_free						0x0007B810
#define addr_vm_map_lock_read					0x0007BB80
#define addr_vm_map_unlock_read					0x0007BBD0
#define addr_vm_map_lookup_entry				0x0007C1C0
#define addr_vm_map_findspace					0x0007EC40
#define addr_vm_map_insert						0x0007CD80
#define addr_vm_map_lock						0x0007BA30
#define addr_vm_map_unlock 						0x0007BAA0
#define addr_vm_map_delete						0x0007E680
#define addr_vm_map_protect						0x000809C0

#endif
