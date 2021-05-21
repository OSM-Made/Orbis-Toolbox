#include "Common.h"
#include "Resolver/Resolver.h"

/* Util */
struct sysentvec* sysvec = 0;
struct prison* prison0 = 0;
struct vnode* rootvnode = 0;

/* STD Lib */
void (*memcpy)(void *dst, const void *src, size_t len) = 0;
void *(*memset)(void * ptr, int value, size_t num) = 0;
int (*memcmp)(const void * ptr1, const void * ptr2, size_t num) = 0;
int (*sprintf)(char* dst, const char *fmt, ...) = 0;
int (*vsprintf)(char* dst, const char* fmt, va_list ap) = 0;
void(*kprintf)(const char* fmt, ...) = 0;

#define NATIVE_RESOLVE(kernbase, _Ty) _Ty = (void*)(kernbase + addr_ ## _Ty);

void Kern_Resolve(uint64_t kernbase)
{
    /* Util */
    NATIVE_RESOLVE(kernbase, sysvec);
    NATIVE_RESOLVE(kernbase, prison0);
    NATIVE_RESOLVE(kernbase, rootvnode);

    /* STD Lib */
    NATIVE_RESOLVE(kernbase, memcpy);
    NATIVE_RESOLVE(kernbase, memset);
    NATIVE_RESOLVE(kernbase, memcmp);
    NATIVE_RESOLVE(kernbase, sprintf);
    NATIVE_RESOLVE(kernbase, vsprintf);
    NATIVE_RESOLVE(kernbase, kprintf);
}

void Userland_Resolve()
{
    
}