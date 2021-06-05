#include "Common.hpp"
#include "Loader.hpp"
#include "Util/Proc.hpp"
#include "Util/Util.hpp"

void* gShellCodePtr = NULL;
void* gStackPtr = NULL;
size_t gShellCodeSize = 0;
bool ShellCodeLoaded = false;
char ProcName[0x20] = { };

extern uint8_t LoaderShellCode[];
extern int32_t LoaderShellCodeSize;

bool Load_SPRX(proc* proc, const char* File)
{
    klog("Load_SPRX(): \"%s\"", File);

    size_t n;
    int err = 0;
    uint64_t thr_initial = 0;
    uint8_t ShellCodeComplete = 0;
    uint64_t ModuleHandle = 0;

    if(!proc) 
	{
        klog("Load_SPRX(): proc pointer was null.");
        return false;
    }

    if(proc->p_dynlibptr == NULL) 
    {
        klog("Load_SPRX(): p_dynlibptr is NULL.");
        return false;
    }

    /*
        Gets the Thread Initial for the shellcore thread. Also checking to make sure were not trying to load a prx already loaded.
    */

    dynlib* m_library = proc->p_dynlibptr->p_dynlib;
    while(m_library != 0)
	{
        if(!strcmp(basename(m_library->ModulePath), basename(File)))
        {
            klog("Load_SPRX(): Module %s is already loaded on proc %s...", basename(File), proc->p_comm);
            return false;
        }

        if(!strcmp(basename(m_library->ModulePath), "libkernel.sprx"))
			thr_initial = (uint64_t)m_library->codeBase + addr_thr_initial_libkernel;

        if(!strcmp(basename(m_library->ModulePath), "libkernel_web.sprx"))
			thr_initial = (uint64_t)m_library->codeBase + addr_thr_initial_libkernel_web;

        if(!strcmp(basename(m_library->ModulePath), "libkernel_sys.sprx"))
			thr_initial = (uint64_t)m_library->codeBase + addr_thr_initial_libkernel_sys;

        m_library = m_library->dynlib_next;
    }

    if(thr_initial == 0) 
    {
		klog("Load_SPRX(): Failed to resolve thr_initial.");

        return false;
    }

    /*
        Allocate some memory on the heap to temporarily store the shellcode while we set our data.
    */
    char* Loader_Temp = (char*)malloc(LoaderShellCodeSize, M_TEMP, 2);
    if(!Loader_Temp)
    {
        klog("Load_SPRX(): Failed to allocate heap space to store temp shellcode data...");

        return false;
    }

    //Setting the shellcode instructions
    memcpy(Loader_Temp, LoaderShellCode, LoaderShellCodeSize);

    //Setting thread initial and sprx path.
    *(uint64_t*)(Loader_Temp + offsetof(OrbisLoader_header, thr_initial)) = thr_initial;
    strcpy(Loader_Temp + offsetof(OrbisLoader_header, SPRXPath), (char*)File);

    /*
        Allocate space on the process for the shellcode and its threads stack.
    */
    gShellCodeSize = LoaderShellCodeSize;
	gShellCodeSize += (PAGE_SIZE - (gShellCodeSize % PAGE_SIZE));
	if(proc_allocate(proc, &gShellCodePtr, gShellCodeSize)) 
    {
        klog("Load_SPRX(): Failed to allocate ShellCode Memory.");

        free(Loader_Temp, M_TEMP);

        return false;
    }

    klog("Load_SPRX(): thr_initial = %llX", thr_initial);
    klog("Load_SPRX(): gShellCodePtr = %llX", gShellCodePtr);

    size_t StackSize = 0x80000;
	if(proc_allocate(proc, &gStackPtr, StackSize)) {
        klog("Load_SPRX(): Failed to allocate Stack Memory.");

        if (gShellCodePtr)
			proc_deallocate(proc, gShellCodePtr, gShellCodeSize);

        free(Loader_Temp, M_TEMP);

        return false;
    }

    /*
        Write the shellcode to the allocated memory on the process.
    */
    err = proc_rw_mem(proc, gShellCodePtr, LoaderShellCodeSize, (void *)Loader_Temp, &n, 1);
    if(err)
    {
        klog("Load_SPRX(): Failed to write Shellcode to Memory. Error: %d.", err);

        if (gShellCodePtr)
			proc_deallocate(proc, gShellCodePtr, gShellCodeSize);

		if (gStackPtr)
			proc_deallocate(proc, gStackPtr, 0x80000);

        free(Loader_Temp, M_TEMP);

        return false;
    }

    /*
        Free up the heap space for the temp shellcode as we dont need it anymore.
    */
    free(Loader_Temp, M_TEMP);

    /*
        Start a thread with the entry pointing to +0x4 in our shellcode because that holds the offset to the starting instructions.
    */
    klog("Load_SPRX(): Starting Shellcode Thread...");
    struct thread *thr = TAILQ_FIRST(&proc->p_threads);
	uint64_t ShellCodeEntry = (uint64_t)gShellCodePtr + *(uint64_t *)(LoaderShellCode + 4);
	create_thread(thr, NULL, (void*)ShellCodeEntry, NULL, (char*)gStackPtr, StackSize, NULL, NULL, NULL, 0, NULL);

    klog("Load_SPRX(): Thread Started!! Waiting for shellcode to complete...");

    /*
        Wait for the shellcode to complete by reading the byte that will be set to 1 on completion.
    */
    while (!ShellCodeComplete) 
	{
        err = proc_rw_mem(proc, gShellCodePtr + offsetof(OrbisLoader_header, ShellCodeComplete), sizeof(ShellCodeComplete), (void *)&ShellCodeComplete, &n, 0);
        if(err)
        {
            klog("Load_SPRX(): Failed to read ShellCodeComplete.");
            return false;
        }

        //klog("Waiting for ShellCode to compelete!\n");
        pause("", 100);
	}

    /*
        Grab the Module handle from the shellcode to see if the loading was a sucess or not.
    */
    err = proc_rw_mem(proc, gShellCodePtr + offsetof(OrbisLoader_header, ModuleHandle), sizeof(ModuleHandle), (void *)&ModuleHandle, &n, 0);
    if(err)
    {
        klog("Load_SPRX(): Failed to read ModuleHandle.");
        return false;
    }

    /*
        Clean up.
    */
    klog("Load_SPRX(): Freeing Shellcode Memory...");
    if (gShellCodePtr)
        proc_deallocate(proc, gShellCodePtr, gShellCodeSize);

    if (gStackPtr)
        proc_deallocate(proc, gStackPtr, 0x80000);

    gShellCodePtr = NULL;
    gStackPtr = NULL;

    if(ModuleHandle > 0 && ModuleHandle < 0x80000000)
    {
        klog("Load_SPRX(): Completed! Module Loaded with handle 0x%llX", ModuleHandle);
        return true;
    }
    else
    {
        klog("Load_SPRX(): Failed with error 0x%llX", ModuleHandle);
        return false;
    }
}