#pragma once
#include "sys/types.h"

#ifndef NULL
#define NULL 0
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#include <stdint.h>
#include <stdarg.h>

#include "../../Common/Settings.h"

extern "C"
{
    #include <sys/lock.h>
    //#include <sys/mutex.h>

	#include <sys/proc.h>
    #include <sys/sysent.h>
    #include <sys/syscall.h>
    #include <sys/ptrace.h>
    #include <sys/uio.h>
    #include <sys/mman.h>
    //#include <sys/pcpu.h>
    #include <vm/vm.h>
    #include <vm/pmap.h>
    #include <vm/vm_map.h>

    #include <errno.h>
    #include <sys/elf_common.h>
    #include <sys/elf64.h>
    #include <sys/eventhandler.h>
    #include <wait.h>
    #include <machine/reg.h>
    #include <fcntl.h>
    #include <time.h>
    #include <dirent.h>

    #include <sys/stat.h>
    #include "sys/mount.h"
}

#include "Util/Util.hpp"
#include "Resolver/Resolver.hpp"

#define SPRX_PATH "/data/Orbis Toolbox/Orbis Toolbox.sprx"