#include "Common.hpp"
#include "kproc.hpp"
#include "Loader.hpp"
#include "Util/Proc.hpp"

#define RESUME_WAIT 17000

//Event Handlers
eventhandler_entry* SystemSuspend;
eventhandler_entry* SystemResume;

eventhandler_entry* ProcessStartEvent;
eventhandler_entry* ProcessExitEvent;

bool IsSystemResuming = false;
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

    //Get first thread in proc.
    thread* td = p->p_threads.tqh_first;

    //Get the sandbox path.
    char* s_SandboxPath = nullptr;
    char* s_Freepath = nullptr;
    vn_fullpath(td, bkJail.fd_jdir, &s_SandboxPath, &s_Freepath);
    klog("%s -> %s\n", p->p_comm, s_SandboxPath);

    //Mount dirs.
    MountDir(td, s_SandboxPath, "/system", MNT_SYNCHRONOUS);
    MountDir(td, s_SandboxPath, "/data", MNT_SYNCHRONOUS);
    MountDir(td, s_SandboxPath, "/host", MNT_SYNCHRONOUS);
    MountDir(td, s_SandboxPath, "/hostapp", MNT_SYNCHRONOUS);

    //Restore previous jail.
    RestoreJail(p, bkJail);
    
    //Check if Toolbox is Loaded.
    bool Toolbox_Loaded = false;
    dynlib* m_library = p->p_dynlibptr->p_dynlib;
    while(m_library != 0)
    {
        if(!strcmp(basename(m_library->ModulePath), "Orbis Toolbox.sprx"))
        {
            Toolbox_Loaded = true;
            break;
        }
        m_library = m_library->dynlib_next;
    }

    //If the tool box is not loaded init the loader and load the module.
    if(!Toolbox_Loaded)
    {
        klog("****\nLaunching Toolbox...\n****");
        
        Loader_Init(p);

        Load_SPRX(SPRX_PATH);

        Loader_Term();

        klog("Launched Toolbox...\n");
    }

    IsSystemResuming = false;

    kthread_exit();
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

        //Get first thread in proc.
        thread* td = p->p_threads.tqh_first;

        //Get the sandbox path.
        char* s_SandboxPath = nullptr;
        char* s_Freepath = nullptr;
        vn_fullpath(td, bkJail.fd_jdir, &s_SandboxPath, &s_Freepath);
        klog("%s -> %s\n", p->p_comm, s_SandboxPath);

        //Mount dirs.
        UnMountDir(td, s_SandboxPath, "/system", MNT_FORCE);
        UnMountDir(td, s_SandboxPath, "/data", MNT_FORCE);
        UnMountDir(td, s_SandboxPath, "/host", MNT_FORCE);
        UnMountDir(td, s_SandboxPath, "/hostapp", MNT_FORCE);

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

    //Get first thread in proc.
    thread* td = curthread();// p->p_threads.tqh_first;

    //Get the sandbox path.
    char* s_SandboxPath = nullptr;
    char* s_Freepath = nullptr;
    vn_fullpath(td, bkJail.fd_jdir, &s_SandboxPath, &s_Freepath);
    klog("%s -> %s\n", p->p_comm, s_SandboxPath);

    //Mount dirs.
    MountDir(td, s_SandboxPath, "/system", MNT_SYNCHRONOUS);
    MountDir(td, s_SandboxPath, "/data", MNT_SYNCHRONOUS);
    MountDir(td, s_SandboxPath, "/host", MNT_SYNCHRONOUS);
    MountDir(td, s_SandboxPath, "/hostapp", MNT_SYNCHRONOUS);

    //Restore previous jail.
    RestoreJail(p, bkJail);

    //Check if Toolbox is Loaded.
    bool Toolbox_Loaded = false;
    dynlib* m_library = p->p_dynlibptr->p_dynlib;
    while(m_library != 0)
    {
        if(!strcmp(basename(m_library->ModulePath), "Orbis Toolbox.sprx"))
        {
            Toolbox_Loaded = true;
            break;
        }
        m_library = m_library->dynlib_next;
    }

    //If the tool box is not loaded init the loader and load the module.
    if(!Toolbox_Loaded)
    {
        klog("****\nLaunching Toolbox...\n****");
        
        Loader_Init(p);

        Load_SPRX(SPRX_PATH);

        Loader_Term();

        klog("Launched Toolbox...\n");
    }

    kthread_exit();
}

void kproc_Init()
{
    //Register Events
    SystemSuspend = EVENTHANDLER_REGISTER(system_suspend_phase1, (void*)OnProcessStart, nullptr, EVENTHANDLER_PRI_FIRST);
    SystemResume = EVENTHANDLER_REGISTER(system_resume_phase1, (void*)OnProcessStart, nullptr, EVENTHANDLER_PRI_FIRST);

    ProcessStartEvent = EVENTHANDLER_REGISTER(process_exec_end, (void*)OnProcessStart, nullptr, EVENTHANDLER_PRI_LAST);
    ProcessExitEvent = EVENTHANDLER_REGISTER(process_exit, (void*)OnProcessExit, nullptr, EVENTHANDLER_PRI_ANY);

    proc* kernel = proc_find_by_name("kernel");
        if(kernel)
            kproc_kthread_add(test2_thread, nullptr, &kernel, NULL, NULL, 0, "kernel", "Loader Thread");

    klog("kproc_Init() -> Sucess!");
}

void kproc_Term()
{
    EVENTHANDLER_DEREGISTER(process_exec_end, ProcessStartEvent);
    EVENTHANDLER_DEREGISTER(process_exit, ProcessExitEvent);
}