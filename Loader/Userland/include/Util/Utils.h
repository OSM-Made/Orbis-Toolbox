#pragma once

#include <sys/sysent.h>
#include <sys/proc.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/uio.h>

#include <sys/mman.h>
#include <sys/pcpu.h>
#include <vm/vm.h>
#include <vm/pmap.h>
#include <vm/vm_map.h>

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

struct Backup_Jail
{
    struct prison* cr_prison;
    uid_t cr_uid;
    uid_t cr_ruid;
    gid_t cr_rgid;
    gid_t cr_groups;

    struct vnode* fd_jdir;
    struct vnode* fd_rdir;
};

void Jailbreak(struct proc* proc, struct Backup_Jail* jail);
void RestoreJail(struct proc* proc, struct Backup_Jail jail);
int klog(char* fmt, ...);
