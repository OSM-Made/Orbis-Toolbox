#include "Common.hpp"
#include "Resolver/Resolver.hpp"

/* Util */
struct sysentvec* sysvec;
struct prison* prison0;
struct vnode* rootvnode;

/* STD Lib */
void* M_TEMP = 0;
void* M_MOUNT = 0;
void *(*malloc)(unsigned long size, void *type, int flags) = 0;
void (*free)(void *addr, void *type) = 0;
void (*memcpy)(void *dst, const void *src, size_t len) = 0;
void *(*memset)(void *ptr, int value, size_t num) = 0;
int (*memcmp)(const void *ptr1, const void *ptr2, size_t num) = 0;
size_t (*strlen)(const char *str) = 0;
int (*strcpy)(char * str1, char * str2) = 0;
char* (*strncpy)(char *destination, const char *source, size_t num) = 0;
int (*strcmp)(const char * str1, const char * str2) = 0;
char* (*strstr)(const char * str1, const char * str2) = 0;
int (*sprintf)(char* dst, const char *fmt, ...) = 0;
int (*snprintf)(char *str, size_t size, const char *format, ...) = 0;
int (*vsprintf)(char* dst, const char* fmt, va_list ap);
int (*vprintf)(const char *fmt, va_list arg) = 0;
int(*sscanf)(const char *str, const char *format, ...) = 0;
char *(*strdup)(const char *s, void* type) = 0;
char *(*realloc)(void *addr, unsigned long size, void* mtp, int flags) = 0;
void(*kprintf)(const char* fmt, ...) = nullptr;

/* FileIO */
int (*vn_fullpath)(struct thread *td, struct vnode *vp, char **retbuf, char **freebuf) = nullptr;
int (*kern_rmdir)(thread* td, char *path, int flags) = nullptr;
int (*kern_mkdir)(thread* td, char *path, int pathseg, int mode) = nullptr;
int (*kern_open)(thread* td, char *path, int pathseg, int flags, int mode) = nullptr;
int (*kern_mount)(struct mntarg	*ma, int flags) = nullptr;
struct mntarg*(*mount_argf)(struct mntarg *ma, const char *name, const char *fmt, ...) = nullptr;

/* Event Handling */
#ifdef SOFTWARE_VERSION_505 || SOFTWARE_VERSION_NA
eventhandler_tag (*eventhandler_register)(eventhandler_list *list, const char *name, void *func, void *arg, int priority);
#endif
#if defined(SOFTWARE_VERSION_672) || defined(SOFTWARE_VERSION_702) || defined(SOFTWARE_VERSION_755) 
eventhandler_tag (*eventhandler_register)(eventhandler_list *list, const char *name, void *func, const char* unk, void *arg, int priority);
#endif
void (*eventhandler_deregister)(eventhandler_list* a, eventhandler_entry* b);
eventhandler_list* (*eventhandler_find_list)(const char *name);

/* Proc */
proc *allproc = 0;
int (*proc_rwmem)(proc* p, uio* uio) = 0;
int (*create_thread)(thread* td, uint64_t ctx, void* start_func, void *arg, char *stack_base, size_t stack_size, char *tls_base, long * child_tid, long * parent_tid, uint64_t flags, uint64_t rtp) = 0;

/* kproc */
int (*kproc_create)(void (*func)(void *), void *arg, proc **newpp, int flags, int pages, const char *fmt, ...);
int (*kproc_exit)(int code);
int (*kproc_kthread_add)(void (*func)(void *), void *arg, proc **procptr, thread **tdptr, int flags, int pages, char * procname, const char *fmt, ...);
void (*kthread_exit)(void);
void (*kthread_suspend_check)(void);
void (*pause)(const char *wmesg, int timo);

/* Virtual memory */
vmspace *(*vmspace_acquire_ref)(proc *p) = nullptr;
void (*vmspace_free)(vmspace* vm) = nullptr;
void (*vm_map_lock_read)(vm_map* map) = nullptr;
void (*vm_map_unlock_read)(vm_map* map) = nullptr;
int (*vm_map_lookup_entry)(vm_map* map, uint64_t address, vm_map_entry **entries) = nullptr;
int (*vm_map_findspace)(vm_map* map, uint64_t start, uint64_t length, uint64_t *addr) = nullptr;
int (*vm_map_insert)(vm_map* map, uint64_t object, uint64_t offset, uint64_t start, uint64_t end, int prot, int max, int cow) = nullptr;
void (*vm_map_lock)(vm_map* map) = nullptr;
void (*vm_map_unlock)(vm_map* map) = nullptr;
int (*vm_map_delete)(vm_map* map, uint64_t start, uint64_t end) = nullptr;
int (*vm_map_protect)(vm_map* map, uint64_t start, uint64_t end, int new_prot, uint64_t set_max) = nullptr;

#define NATIVE_RESOLVE(kernbase, _Ty) _Ty = (decltype(_Ty))(kernbase + addr_ ## _Ty);

void Resolve(uint64_t Kernel_Base)
{
    /* Util */
    NATIVE_RESOLVE(Kernel_Base, sysvec);
    NATIVE_RESOLVE(Kernel_Base, prison0);
    NATIVE_RESOLVE(Kernel_Base, rootvnode);

    /* STD Lib */
    NATIVE_RESOLVE(Kernel_Base, M_TEMP);
    NATIVE_RESOLVE(Kernel_Base, M_MOUNT);
    NATIVE_RESOLVE(Kernel_Base, malloc);
    NATIVE_RESOLVE(Kernel_Base, free);
    NATIVE_RESOLVE(Kernel_Base, memcpy);
    NATIVE_RESOLVE(Kernel_Base, memset);
    NATIVE_RESOLVE(Kernel_Base, memcmp);
    NATIVE_RESOLVE(Kernel_Base, strlen);
    NATIVE_RESOLVE(Kernel_Base, strcpy);
    NATIVE_RESOLVE(Kernel_Base, strncpy);
    NATIVE_RESOLVE(Kernel_Base, strcmp);
    NATIVE_RESOLVE(Kernel_Base, strstr);
    NATIVE_RESOLVE(Kernel_Base, sprintf);
    NATIVE_RESOLVE(Kernel_Base, snprintf);
    NATIVE_RESOLVE(Kernel_Base, vsprintf);
    NATIVE_RESOLVE(Kernel_Base, vprintf);
    NATIVE_RESOLVE(Kernel_Base, sscanf);
    NATIVE_RESOLVE(Kernel_Base, strdup);
    NATIVE_RESOLVE(Kernel_Base, realloc);
    NATIVE_RESOLVE(Kernel_Base, kprintf);
    
    /* FileIO */
    NATIVE_RESOLVE(Kernel_Base, vn_fullpath);
    NATIVE_RESOLVE(Kernel_Base, kern_rmdir);
    NATIVE_RESOLVE(Kernel_Base, kern_mkdir);
    NATIVE_RESOLVE(Kernel_Base, kern_open);
    NATIVE_RESOLVE(Kernel_Base, kern_mount);
    NATIVE_RESOLVE(Kernel_Base, mount_argf);

    /* Event Handling */
    NATIVE_RESOLVE(Kernel_Base, eventhandler_register);
    NATIVE_RESOLVE(Kernel_Base, eventhandler_deregister);
    NATIVE_RESOLVE(Kernel_Base, eventhandler_find_list);

    /* Proc */
    NATIVE_RESOLVE(Kernel_Base, allproc);
    NATIVE_RESOLVE(Kernel_Base, proc_rwmem);
    NATIVE_RESOLVE(Kernel_Base, create_thread);

    /* kproc */
    NATIVE_RESOLVE(Kernel_Base, kproc_create);
    NATIVE_RESOLVE(Kernel_Base, kproc_exit);
    NATIVE_RESOLVE(Kernel_Base, kproc_kthread_add);
    NATIVE_RESOLVE(Kernel_Base, kthread_exit);
    NATIVE_RESOLVE(Kernel_Base, kthread_suspend_check);
    NATIVE_RESOLVE(Kernel_Base, pause);

    /* Virtual Memory */
    NATIVE_RESOLVE(Kernel_Base, vmspace_acquire_ref);
    NATIVE_RESOLVE(Kernel_Base, vmspace_free);
    NATIVE_RESOLVE(Kernel_Base, vm_map_lock_read);
    NATIVE_RESOLVE(Kernel_Base, vm_map_unlock_read);
    NATIVE_RESOLVE(Kernel_Base, vm_map_lookup_entry);
    NATIVE_RESOLVE(Kernel_Base, vm_map_findspace);
    NATIVE_RESOLVE(Kernel_Base, vm_map_insert);
    NATIVE_RESOLVE(Kernel_Base, vm_map_lock);
    NATIVE_RESOLVE(Kernel_Base, vm_map_unlock);
    NATIVE_RESOLVE(Kernel_Base, vm_map_delete);
    NATIVE_RESOLVE(Kernel_Base, vm_map_protect);
}