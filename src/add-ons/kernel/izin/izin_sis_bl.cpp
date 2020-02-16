/*
- Telif hakkı fegeya 2020
 */


#include <KernelExport.h>
#include <Node.h>

#include <util/kernel_cpp.h>
#include <util/AutoLock.h>
#include <thread.h>
#include <team.h>
#include <file_cache.h>
#include <generic_syscall.h>
#include <syscalls.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>


#define TRACE_CACHE_MODULE
#ifdef TRACE_CACHE_MODULE
#	define TRACE(x) dprintf x
#else
#	define TRACE(x) ;
#endif


