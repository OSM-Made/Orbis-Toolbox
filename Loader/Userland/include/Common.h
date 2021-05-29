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

#include "../../Common/Settings.h"
#include "Resolver/Resolver.h"
#include "Resolver/Patches.h"
#include "syscall.h"
#include "ELF.h"
#include "Util/Utils.h"

extern uint8_t* gKernelBase;