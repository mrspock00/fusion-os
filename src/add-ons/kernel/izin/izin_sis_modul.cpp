/*
 Telif Hakkı Fegeya 2020
 	Fusion
 */


#include "launch_speedup.h"

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


