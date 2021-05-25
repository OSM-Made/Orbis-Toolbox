#include "Common.hpp"
#include "kproc.hpp"
#include "Loader.hpp"
#include "Util/Proc.hpp"

//Event Handlers
eventhandler_entry* ProcessStartEvent;
eventhandler_entry* ProcessExitEvent;

void test_thread(void* arg)
{
    proc* p = (proc*)arg;

    klog("Hello From thread..");

    //pause the thread for 20 seconds.
    pause("", 20000);

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

void OnProcessStart(void *arg, struct proc *p)
{
    if(!p)
        return;

    klog("OnProcessStart: %s(%s)", p->p_comm, p->titleId);

    if(!strcmp(p->titleId, "NPXS20001") && (!strcmp(p->p_comm, "SceShellUI") || !strcmp(p->p_comm, "eboot.bin")))
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
        MountDir(td, s_SandboxPath, "/system", MNT_SYNCHRONOUS);
        MountDir(td, s_SandboxPath, "/data", MNT_SYNCHRONOUS);
        MountDir(td, s_SandboxPath, "/host", MNT_SYNCHRONOUS);
        MountDir(td, s_SandboxPath, "/hostapp", MNT_SYNCHRONOUS);

        //Restore previous jail.
        RestoreJail(p, bkJail);

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

void kproc_Init()
{
    //Register Events
    ProcessStartEvent = EVENTHANDLER_REGISTER(process_exec_end, (void*)OnProcessStart, nullptr, EVENTHANDLER_PRI_LAST);
    ProcessExitEvent = EVENTHANDLER_REGISTER(process_exit, (void*)OnProcessExit, nullptr, EVENTHANDLER_PRI_ANY);

    klog("kproc_Init() -> Sucess!");
}

void kproc_Term()
{
    EVENTHANDLER_DEREGISTER(process_exec_end, ProcessStartEvent);
    EVENTHANDLER_DEREGISTER(process_exit, ProcessExitEvent);
}