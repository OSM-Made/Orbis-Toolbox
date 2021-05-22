#include "Common.h"
#include "Resolver/Resolver.h"

/* Util */
struct sysentvec* ksysvec = 0;
struct prison* kprison0 = 0;
struct vnode* krootvnode = 0;

/* STD Lib */
void (*kmemcpy)(void *dst, const void *src, size_t len) = 0;
void *(*kmemset)(void * ptr, int value, size_t num) = 0;
int (*kmemcmp)(const void * ptr1, const void * ptr2, size_t num) = 0;
int (*ksprintf)(char* dst, const char *fmt, ...) = 0;
int (*kvsprintf)(char* dst, const char* fmt, va_list ap) = 0;
void(*kprintf)(const char* fmt, ...) = 0;

#define NATIVE_RESOLVE(kernbase, _Ty) _Ty = (void*)(kernbase + addr_ ## _Ty);

void Kern_Resolve(uint64_t kernbase)
{
    /* Util */
    NATIVE_RESOLVE(kernbase, ksysvec);
    NATIVE_RESOLVE(kernbase, kprison0);
    NATIVE_RESOLVE(kernbase, krootvnode);

    /* STD Lib */
    NATIVE_RESOLVE(kernbase, kmemcpy);
    NATIVE_RESOLVE(kernbase, kmemset);
    NATIVE_RESOLVE(kernbase, kmemcmp);
    NATIVE_RESOLVE(kernbase, ksprintf);
    NATIVE_RESOLVE(kernbase, kvsprintf);
    NATIVE_RESOLVE(kernbase, kprintf);

    klog("Kern_Resolve() -> Sucess!");
}

int sys_dynlib_dlsym(int loadedModuleID, const char *name, void *destination) {
	return syscall(591, loadedModuleID, name, destination);
}

int sys_dynlib_load_prx(const char *name, int *idDestination) {
	return syscall(594, name, 0, idDestination, 0);
}

//kernel
int(*sceKernelDebugOutText)(int dbg_channel, const char* text);

//libc
void *(*malloc)(size_t size);
void(*free)(void *ptr);
void *(*calloc)(size_t num, size_t size);
void *(*realloc)(void* ptr, size_t size);
void *(*memset)(void *destination, int value, size_t num);
void *(*memcpy)(void *destination, const void *source, size_t num);
int(*memcmp)(const void *s1, const void *s2, size_t n);
char *(*strcpy)(char *destination, const char *source);
char *(*strncpy)(char *destination, const char *source, size_t num);
char *(*strcat)(char *dest, const char *src);
char *(*strncat)(char *dest, const char *src, size_t n);
size_t(*strlen)(const char *s);
int(*strcmp)(const char *s1, const char *s2);
int(*strncmp)(const char *s1, const char *s2, size_t n);
int(*sprintf)(char *str, const char *format, ...);
int(*snprintf)(char *str, size_t size, const char *format, ...);
int(*vsprintf)(char * s, const char * format, va_list arg);
int(*sscanf)(const char *str, const char *format, ...);
char *(*strchr)(const char *s, int c);
char *(*strrchr)(const char *s, int c);
char *(*strstr)(char *str1, char *str2);
int(*atoi)(const char * str);
double(*atof)(const char * str);
double(*sin)(double x);
double(*cos)(double x);
double(*atan2)(double x, double y);
double(*sqrt)(double vec);
char *(*strdup)(const char *s);

void Userland_Resolve()
{
    int Libkernel_library = 0;
    if (sys_dynlib_load_prx("libkernel.sprx", &Libkernel_library))
		if (sys_dynlib_load_prx("libkernel_web.sprx", &Libkernel_library))
			sys_dynlib_load_prx("libkernel_sys.sprx", &Libkernel_library);

    sys_dynlib_dlsym(Libkernel_library, "sceKernelDebugOutText", &sceKernelDebugOutText);

    int Libc_Library = 0;
	sys_dynlib_load_prx("libSceLibcInternal.sprx", &Libc_Library);
	sys_dynlib_dlsym(Libc_Library, "malloc", &malloc);
	sys_dynlib_dlsym(Libc_Library, "free", &free);
	sys_dynlib_dlsym(Libc_Library, "calloc", &calloc);
	sys_dynlib_dlsym(Libc_Library, "realloc", &realloc);
	sys_dynlib_dlsym(Libc_Library, "memset", &memset);
	sys_dynlib_dlsym(Libc_Library, "memcpy", &memcpy);
	sys_dynlib_dlsym(Libc_Library, "memcmp", &memcmp);
	sys_dynlib_dlsym(Libc_Library, "strcpy", &strcpy);
	sys_dynlib_dlsym(Libc_Library, "strncpy", &strncpy);
	sys_dynlib_dlsym(Libc_Library, "strcat", &strcat);
	sys_dynlib_dlsym(Libc_Library, "strncat", &strncat);
	sys_dynlib_dlsym(Libc_Library, "strlen", &strlen);
	sys_dynlib_dlsym(Libc_Library, "strcmp", &strcmp);
	sys_dynlib_dlsym(Libc_Library, "strncmp", &strncmp);
	sys_dynlib_dlsym(Libc_Library, "sprintf", &sprintf);
	sys_dynlib_dlsym(Libc_Library, "snprintf", &snprintf);
	sys_dynlib_dlsym(Libc_Library, "vsprintf", &vsprintf);
	sys_dynlib_dlsym(Libc_Library, "sscanf", &sscanf);
	sys_dynlib_dlsym(Libc_Library, "strchr", &strchr);
	sys_dynlib_dlsym(Libc_Library, "strrchr", &strrchr);
	sys_dynlib_dlsym(Libc_Library, "strstr", &strstr);
	sys_dynlib_dlsym(Libc_Library, "atoi", &atoi);
	sys_dynlib_dlsym(Libc_Library, "atof", &atof);
	sys_dynlib_dlsym(Libc_Library, "sin", &sin);
	sys_dynlib_dlsym(Libc_Library, "cos", &cos);
	sys_dynlib_dlsym(Libc_Library, "atan2", &atan2);
	sys_dynlib_dlsym(Libc_Library, "sqrt", &sqrt);
	sys_dynlib_dlsym(Libc_Library, "strdup", &strdup);

    Log("Userland_Resolve() -> Sucess!");
}