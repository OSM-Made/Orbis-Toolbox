#include "Common.h"
#include "Util/Utils.h"

void Jailbreak(struct proc* proc, struct Backup_Jail* jail)
{
    if(proc)
    {
        struct ucred* cred = proc->p_ucred;
        struct filedesc* fd = proc->p_fd;

        if(!cred || !fd)
            return;

        if(jail)
        {
            jail->cr_prison = cred->cr_prison;
            jail->cr_uid = cred->cr_uid;
            jail->cr_ruid = cred->cr_ruid;
            jail->cr_rgid = cred->cr_rgid;
            jail->cr_groups = cred->cr_groups[0];

            jail->fd_jdir = fd->fd_jdir;
            jail->fd_rdir = fd->fd_rdir;
        }
        
        cred->cr_prison = *(struct prison**)kprison0;

        cred->cr_uid = 0;
        cred->cr_ruid = 0;
        cred->cr_rgid = 0;
        cred->cr_groups[0] = 0;

    
        fd->fd_jdir = *(struct vnode**)krootvnode;
        fd->fd_rdir = *(struct vnode**)krootvnode;
    }
}

void RestoreJail(struct proc* proc, struct Backup_Jail jail)
{
    if(proc)
    {
        struct ucred* cred = proc->p_ucred;
        struct filedesc* fd = proc->p_fd;

        if(!cred || !fd)
            return;

        cred->cr_prison = jail.cr_prison;
        cred->cr_uid = jail.cr_uid;
        cred->cr_ruid = jail.cr_ruid;
        cred->cr_rgid = jail.cr_rgid;
        cred->cr_groups[0] = jail.cr_groups;

        fd->fd_jdir = jail.fd_jdir;
        fd->fd_rdir = jail.fd_rdir;
    }
}

int klog(char* fmt, ...)
{
    char buffer[0x400] = { 0 };
	va_list args;
	va_start(args, fmt);
	kvsprintf(buffer, fmt, args);
    va_end(args);

    char buffer2[0x400] = { 0 };
    ksprintf(buffer2, "[Orbis Toolbox] %s\n", buffer);

    #if defined(SOFTWARE_VERSION_505) || defined(SOFTWARE_VERSION_NA)
    
    struct thread *td = curthread();

	struct sys_read_args {
		uint64_t unk;
		uint64_t msg;
		uint64_t unk2;
	};

    struct sysent* sysents = ksysvec->sv_table;

    int(*sys_read)(struct thread * td, struct sys_read_args * uap) = (void*)sysents[601].sy_call;

    td->td_retval[0] = 0;

	struct sys_read_args uap;
	uap.unk = 7;
	uap.msg = (uint64_t)buffer2;
	uap.unk2 = 0;

	int errorno = sys_read(td, &uap);

	if(errorno)
		return -errorno;

    return (int)td->td_retval[0];

    #endif

    #if defined(SOFTWARE_VERSION_672) || defined(SOFTWARE_VERSION_702) || defined(SOFTWARE_VERSION_755)

    kprintf(buffer2);

    return 0;

    #endif
}

void Log(char* fmt, ...)
{
    if(sceKernelDebugOutText)
    {
        char buffer[0x400] = { 0 };
        va_list args;
        va_start(args, fmt);
        vsprintf(buffer, fmt, args);
        va_end(args);

        char buffer2[0x400] = { 0 };
        sprintf(buffer2, "[Orbis Toolbox] %s\n", buffer);

        sceKernelDebugOutText(0, buffer2);
    }
}