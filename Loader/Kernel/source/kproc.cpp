#include "Common.hpp"
#include "kproc.hpp"
#include "Loader.hpp"
#include "Util/Proc.hpp"

#define RESUME_WAIT 17000
bool IsSystemResuming = false;

//Event Handlers
eventhandler_entry* SystemSuspendEvent;
eventhandler_entry* SystemResumeEvent;
eventhandler_entry* ProcessStartEvent;
eventhandler_entry* ProcessExitEvent;

void test_thread(void* arg)
{
    proc* p = (proc*)arg;

    klog("Hello From thread..");

    if(IsSystemResuming)
        pause("", 20000);
    else
    {
        //pause the thread for 20 seconds.
        #if defined(SOFTWARE_VERSION_505) || defined(SOFTWARE_VERSION_NA)
        pause("", 17000);
        #else
        pause("", 20000);
        #endif
    }

    //Jailbreak the process.
    Backup_Jail bkJail;
    Jailbreak(p, &bkJail);

    //Mount the dirs for ShellUI
    Mount_Dirs(p, bkJail.fd_jdir, true);

    klog("****Launching Toolbox...****");
    if(Load_SPRX(p, SPRX_PATH))
        klog("Launched Toolbox...");
    else
        klog("Failed to Launch Toolbox... Maybe you forgot to load HEN??");

    //Restore previous jail.
    RestoreJail(p, bkJail);

    IsSystemResuming = false;

    kthread_exit();
}

void OnSystemSuspend(void* arg)
{
    klog("System is Suspending...");
    IsSystemResuming = true;
}

void OnSystemResume(void* arg)
{
    klog("System is Resuming...");
    IsSystemResuming = true;
}

void OnProcessStart(void *arg, struct proc *p)
{
    if(!p)
        return;

    klog("OnProcessStart: %s(%s)", p->p_comm, p->titleId);

    if(!strcmp(p->titleId, "NPXS20001") && (!strcmp(p->p_comm, "SceShellUI") || !strcmp(p->p_comm, "eboot.bin")))
    {
        proc* kernel = proc_find_by_name("kernel");
        if(kernel)
            kproc_kthread_add(test_thread, p, &kernel, NULL, NULL, 0, "kernel", "Loader Thread");
    }
}

void OnProcessExit(void *arg, struct proc *p)
{
    if(!p)
        return;

    klog("OnProcessExit: %s(%s)", p->p_comm, p->titleId);

    //In the event of ui crash or reloading the ui unmount the file paths.
    if(!strcmp(p->titleId, "NPXS20001") && !strcmp(p->p_comm, "SceShellUI"))
    {
        //Jailbreak the process.
        Backup_Jail bkJail;
        Jailbreak(p, &bkJail);

        //Un-Mount the dirs for ShellUI
        Mount_Dirs(p, bkJail.fd_jdir, false);

        //Restore previous jail.
        RestoreJail(p, bkJail);
    }
}

void test2_thread(void* arg)
{
    klog("Hello From thread..");

    proc* p = proc_find_by_name("SceShellUI");

    if(!p)
    {
        kthread_exit();
        return;
    }
    
    //Jailbreak the process.
    Backup_Jail bkJail;
    Jailbreak(p, &bkJail);

    //Mount the dirs for ShellUI
    Mount_Dirs(p, bkJail.fd_jdir, true);

    klog("****Launching Toolbox...****");
    if(Load_SPRX(p, SPRX_PATH))
    {
        klog("Launched Toolbox Sucessfully.");

        klog("Registering Events...");
        //Register Events
        SystemSuspendEvent = EVENTHANDLER_REGISTER(system_suspend_phase1, (void*)OnSystemSuspend, nullptr, EVENTHANDLER_PRI_FIRST);
        SystemResumeEvent = EVENTHANDLER_REGISTER(system_resume_phase1, (void*)OnSystemResume, nullptr, EVENTHANDLER_PRI_FIRST);
        ProcessStartEvent = EVENTHANDLER_REGISTER(process_exec_end, (void*)OnProcessStart, nullptr, EVENTHANDLER_PRI_LAST);
        ProcessExitEvent = EVENTHANDLER_REGISTER(process_exit, (void*)OnProcessExit, nullptr, EVENTHANDLER_PRI_ANY);
        klog("Events Registered Sucessfully.");  
    }    
    else
        klog("Failed to Launch Toolbox... Maybe you forgot to load HEN??");

    //Restore previous jail.
    RestoreJail(p, bkJail);

    kthread_exit();
}

void kproc_Init()
{
    proc* kernel = proc_find_by_name("kernel");
    if(kernel)
        kproc_kthread_add(test2_thread, nullptr, &kernel, NULL, NULL, 0, "kernel", "Loader Thread");

    klog("kproc_Init() -> Sucess!"); 
}

void kproc_Term()
{
    EVENTHANDLER_DEREGISTER(system_suspend_phase1, SystemSuspendEvent);
    EVENTHANDLER_DEREGISTER(system_resume_phase1, SystemResumeEvent);
    EVENTHANDLER_DEREGISTER(process_exec_end, ProcessStartEvent);
    EVENTHANDLER_DEREGISTER(process_exit, ProcessExitEvent);
}