#include "Common.h"

/* Util */
#define addr_Xfast_syscall		                0x1C0
#define addr_sysvec                             0x19BBCD0
#define addr_prison0                            0x10986A0
#define addr_rootvnode                          0x22C1A70

/* STD Lib */
#define addr_memcpy								0x1EA530
#define addr_memset								0x3205C0
#define addr_memcmp								0x50AC0
#define addr_sprintf                            0x436280
#define addr_vsprintf                           0x436310
#define addr_kprintf                            0x0

#define addr_kmem_alloc							0xFCC80
#define addr_kernel_map							0x1AC60E0

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

#define NATIVE_RESOLVE(kernbase, _Ty) _Ty = (decltype(_Ty))(void*)(kernbase + addr_ ## _Ty);

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

static inline struct thread *curthread(void) {
	struct thread* td;
	__asm__ __volatile__ (
	    "mov %0, %%gs:0"
	    : "=r"(td)
	);

	return td;
}

static inline __attribute__((always_inline)) uint64_t __readmsr(uint32_t __register) {
	uint32_t __edx, __eax;

	__asm__ volatile (
	    "rdmsr"
	    : "=d"(__edx),
	    "=a"(__eax)
	    : "c"(__register)
	);

	return (((uint64_t)__edx) << 32) | (uint64_t)__eax;
}

int klog(char* fmt, ...)
{
    char buffer[0x400] = { 0 };
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
    va_end(args);

    char buffer2[0x400] = { 0 };
    sprintf(buffer2, "[Orbis Toolbox] %s\n", buffer);

    //#if defined(SOFTWARE_VERSION_505) || defined(SOFTWARE_VERSION_NA)
    
    struct thread *td = curthread();

	struct sys_read_args {
		uint64_t unk;
		uint64_t msg;
		uint64_t unk2;
	};

    struct sysent* sysents = sysvec->sv_table;

    auto sys_read = (int(*)(struct thread * td, struct sys_read_args * uap))sysents[601].sy_call;

    td->td_retval[0] = 0;

	struct sys_read_args uap;
	uap.unk = 7;
	uap.msg = (uint64_t)buffer2;
	uap.unk2 = 0;

	int errorno = sys_read(td, &uap);

	if(errorno)
		return -errorno;

    return (int)td->td_retval[0];

    /*#endif

    #if defined(SOFTWARE_VERSION_672) || defined(SOFTWARE_VERSION_702) || defined(SOFTWARE_VERSION_755)

    kprintf(buffer2);

    return 0;

    #endif*/
}

extern "C" void _main(uint64_t p)
{
    uint64_t KernelBase = (__readmsr(0xC0000082) - addr_Xfast_syscall);

    if(!KernelBase)
		return;

    Kern_Resolve(KernelBase);
    klog("!!! HELLO HELLO FROM KERNEL !!!");
}