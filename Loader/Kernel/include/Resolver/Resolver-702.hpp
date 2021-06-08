#pragma once

#if defined(SOFTWARE_VERSION_702)

/* Util */
#define addr_Xfast_syscall		                0x1C0
#define addr_sysvec                             0x1A4F460
#define addr_prison0                            0x113E398
#define addr_rootvnode                          0x22C5750

/* STD Lib */
#define addr_M_TEMP                             0x1A7AE50
#define addr_M_MOUNT                            0x1A71A70
#define addr_malloc								0x301840
#define addr_free					    		0x301A40
#define addr_memcpy								0x2F040
#define addr_memset								0x2DFC20
#define addr_memcmp								0x207500
#define addr_strlen                             0x93FF0
#define addr_strcpy								0x2CC70
#define addr_strncpy                            0xF9E40
#define addr_strcmp                             0x43B5F0
#define addr_strstr                             0x5740
#define addr_sprintf                            0xBC970
#define addr_snprintf                           0xBCA30
#define addr_vsprintf                           0xBCA00
#define addr_vprintf                            0xBC7A0
#define addr_sscanf                             0x2077A0
#define addr_strdup                             0x382B0
#define addr_realloc                            0x301B70
#define addr_kprintf                            0xBC730

/* File IO */
#define addr_vn_fullpath                        0x15F470
#define addr_kern_rmdir                         0x35ADE0
#define addr_kern_mkdir                         0x35AA60
#define addr_kern_open                          0x355960
#define addr_kern_mount                         0x299080
#define addr_mount_argf                         0x298ED0

/* Event Handling */
#define addr_eventhandler_register              0x483810
#define addr_eventhandler_deregister            0x483BB0
#define addr_eventhandler_find_list             0x483DB0

/* Proc */
#define addr_allproc						    0x1B48318
#define addr_proc_kill                          0x313B90
#define addr_proc_rwmem							0x43E80
#define addr_create_thread                      0x842E0

/* Kproc */
#define addr_kproc_create					    0xC4170
#define addr_kproc_exit                         0xC43E0
#define addr_kproc_kthread_add				    0xC4EE0
#define addr_kthread_exit						0xC49C0
#define addr_kthread_suspend_check				0xC4DD0
#define addr_pause                              0x16EEE0

/* Module Offsets */
#define addr_thr_initial_libkernel              0x8D420
#define addr_thr_initial_libkernel_web          0x8D420
#define addr_thr_initial_libkernel_sys          0x8D830

/* Virtual Memory */
#define addr_vmspace_acquire_ref				0x25F9F0
#define addr_vmspace_free						0x25F820
#define addr_vm_map_lock_read					0x25FB90
#define addr_vm_map_unlock_read					0x25FBE0
#define addr_vm_map_lookup_entry				0x260190
#define addr_vm_map_findspace					0x262CC0
#define addr_vm_map_insert						0x260D60
#define addr_vm_map_lock						0x25FA50
#define addr_vm_map_unlock 						0x25FAB0
#define addr_vm_map_delete						0x262700
#define addr_vm_map_protect						0x264A50

#endif