#pragma once
#include "../../Common/Settings.h"

#include "Resolver-505.hpp"
#include "Resolver-672.hpp"
//#include "Resolver-702.hpp"
//#include "Resolver-755.hpp"

/* Util */
extern struct sysentvec* sysvec;
extern struct prison* prison0;
extern struct vnode* rootvnode;

/* STD Lib */
extern void *M_TEMP;
extern void* M_MOUNT;
extern void *(*malloc)(unsigned long size, void *type, int flags);
extern void (*free)(void *addr, void *type);
extern void (*memcpy)(void *dst, const void *src, size_t len);
extern void *(*memset)(void *ptr, int value, size_t num);
extern int (*memcmp)(const void *ptr1, const void *ptr2, size_t num);
extern size_t (*strlen)(const char *str);
extern int (*strcpy)(char * str1, char * str2);
extern char* (*strncpy)(char *destination, const char *source, size_t num);
extern int (*strcmp)(const char * str1, const char * str2);
extern char* (*strstr)(const char * str1, const char * str2);
extern int (*sprintf)(char* dst, const char *fmt, ...);
extern int (*snprintf)(char *str, size_t size, const char *format, ...);
extern int (*vsprintf)(char* dst, const char* fmt, va_list ap);
extern int (*vprintf)(const char *fmt, va_list arg);
extern int(*sscanf)(const char *str, const char *format, ...);
extern char *(*strdup)(const char *s, void* type);
extern char *(*realloc)(void *addr, unsigned long size, void* mtp, int flags);
extern void(*kprintf)(const char* fmt, ...);

/* FileIO */
extern int (*vn_fullpath)(struct thread *td, struct vnode *vp, char **retbuf, char **freebuf);
extern int (*kern_rmdir)(thread* td, char *path, int flags);
extern int (*kern_mkdir)(thread* td, char *path, int pathseg, int mode);
extern int (*kern_open)(thread* td, char *path, int pathseg, int flags, int mode);
extern int (*kern_mount)(struct mntarg	*ma, int flags);
extern struct mntarg*(*mount_argf)(struct mntarg *ma, const char *name, const char *fmt, ...);

/* Event Handling */
#ifdef SOFTWARE_VERSION_505 || SOFTWARE_VERSION_NA
extern eventhandler_tag (*eventhandler_register)(eventhandler_list *list, const char *name, void *func, void *arg, int priority);
#endif
#if defined(SOFTWARE_VERSION_672) || defined(SOFTWARE_VERSION_702) || defined(SOFTWARE_VERSION_755) 
extern eventhandler_tag (*eventhandler_register)(eventhandler_list *list, const char *name, void *func, const char* unk, void *arg, int priority);
#endif
extern void (*eventhandler_deregister)(eventhandler_list* a, eventhandler_entry* b);
extern eventhandler_list* (*eventhandler_find_list)(const char *name);

/*
	process_exec
	process_exit

	system_suspend_phase1
	system_resume_phase1
	shutdown_pre_sync
*/

#ifdef SOFTWARE_VERSION_505
#define EVENTHANDLER_REGISTER(name, func, arg, priority)		\
	eventhandler_register(NULL, #name, func, arg, priority)
#endif
#if defined(SOFTWARE_VERSION_672) || defined(SOFTWARE_VERSION_702) || defined(SOFTWARE_VERSION_755) 
#define EVENTHANDLER_REGISTER(name, func, arg, priority)		\
	eventhandler_register(NULL, #name, func, "", arg, priority)
#endif


#define EVENTHANDLER_DEREGISTER(name, tag) 				\
do {									\
	struct eventhandler_list *_el;					\
									\
	if ((_el = eventhandler_find_list(#name)) != NULL)		\
		eventhandler_deregister(_el, tag);			\
} while(0)

/* Proc */
extern proc *allproc;
extern int (*proc_kill)(proc *p, char* why);
extern int (*proc_rwmem)(proc *p, uio *uio);
extern int (*create_thread)(thread * td, uint64_t ctx, void* start_func, void *arg, char *stack_base, size_t stack_size, char *tls_base, long * child_tid, long * parent_tid, uint64_t flags, uint64_t rtp);

/* kproc */
extern int (*kproc_create)(void (*func)(void *), void *arg, proc **newpp, int flags, int pages, const char *fmt, ...);
extern int (*kproc_exit)(int code);
extern int (*kproc_kthread_add)(void (*func)(void *), void *arg, proc **procptr, thread **tdptr, int flags, int pages, char * procname, const char *fmt, ...);
extern void (*kthread_exit)(void);
extern void (*kthread_suspend_check)(void);
extern void (*pause)(const char *wmesg, int timo);

/* Virtual Memory */
extern vmspace *(*vmspace_acquire_ref)(proc* p);
extern void (*vmspace_free)(vmspace* vm);
extern void (*vm_map_lock_read)(vm_map* map);
extern void (*vm_map_unlock_read)(vm_map* map);
extern int (*vm_map_lookup_entry)(vm_map* map, uint64_t address, vm_map_entry **entries);
extern int (*vm_map_findspace)(vm_map* map, uint64_t start, uint64_t length, uint64_t *addr);
extern int (*vm_map_insert)(vm_map* map, uint64_t object, uint64_t offset, uint64_t start, uint64_t end, int prot, int max, int cow);
extern void (*vm_map_lock)(vm_map* map);
extern void (*vm_map_unlock)(vm_map* map);
extern int (*vm_map_delete)(vm_map* map, uint64_t start, uint64_t end);
extern int (*vm_map_protect)(vm_map* map, uint64_t start, uint64_t end, int new_prot, uint64_t set_max);

void Resolve(uint64_t Kernel_Base);