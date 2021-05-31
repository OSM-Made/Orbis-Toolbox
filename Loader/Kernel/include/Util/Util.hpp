#pragma once

#define offsetof(st, m) ((size_t)((char *)&((st *)(0))->m - (char *)0))
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

static inline thread *curthread(void) {
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

static inline __attribute__((always_inline)) uint64_t __readcr0(void) {
	uint64_t cr0;

	__asm__ volatile (
	    "movq %0, %%cr0"
	    : "=r" (cr0)
	    : : "memory"
	);

	return cr0;
}
static inline __attribute__((always_inline)) void __writecr0(uint64_t cr0) {
	__asm__ volatile (
	    "movq %%cr0, %0"
	    : : "r" (cr0)
	    : "memory"
	);
}

bool MountDir(thread* td, char* Sandbox, char* what, int flags);
bool UnMountDir(thread* td, char* Sandbox, char* what, int flags);
bool Mount_Dirs(proc* p, vnode* jdir, bool Mount);

char* strrchr(const char *cp, int ch);
char* strchr(const char *s, int c);
char* basename(const char *filename);

struct Backup_Jail
{
    prison* cr_prison;
    uid_t cr_uid;
    uid_t cr_ruid;
    gid_t cr_rgid;
    gid_t cr_groups;

    vnode* fd_jdir;
    vnode* fd_rdir;
};

void Jailbreak(proc* proc, Backup_Jail* jail);
void RestoreJail(proc* proc, Backup_Jail jail);

int klog(char* fmt, ...);
int sys_unmount(const char *dir, int flags);