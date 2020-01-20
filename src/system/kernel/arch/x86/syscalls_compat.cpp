/*
 * Copyright 2018, Jérôme Duval, jerome.duval@gmail.com.
 * Copyright 2008, Ingo Weinhold, ingo_weinhold@gmx.de.
 * Copyright 2004-2010, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


/*!	Big case statement for dispatching syscalls, as well as the generic
	syscall interface.
*/


#include <syscalls.h>

#include <arch_config.h>
#include <arch/system_info.h>
#include <cpu.h>
#include <debug.h>
#include <disk_device_manager/ddm_userland_interface.h>
#include <elf.h>
#include <frame_buffer_console.h>
#include <fs/fd.h>
#include <fs/node_monitor.h>
#include <generic_syscall.h>
#include <int.h>
#include <kernel.h>
#include <kimage.h>
#include <ksignal.h>
#include <ksyscalls.h>
#include <ksystem_info.h>
#include <messaging.h>
#include <port.h>
#include <posix/realtime_sem.h>
#include <posix/xsi_message_queue.h>
#include <posix/xsi_semaphore.h>
#include <real_time_clock.h>
#include <safemode.h>
#include <sem.h>
#include <sys/resource.h>
#include <system_profiler.h>
#include <thread.h>
#include <tracing.h>
#include <user_atomic.h>
#include <user_mutex.h>
#include <usergroup.h>
#include <UserTimer.h>
#include <util/AutoLock.h>
#include <vfs.h>
#include <vm/vm.h>
#include <wait_for_objects.h>


status_t _user_generic_syscall(const char* userSubsystem, uint32 function,
	void* buffer, size_t bufferSize);
int _user_is_computer_on(void);


#include "syscall_numbers.h"

#define kSyscallCount kSyscallCompatCount
#define kSyscallInfos kSyscallCompatInfos
#define kExtendedSyscallInfos kExtendedSyscallCompatInfos

extern const int kSyscallCount;
extern const syscall_info kSyscallInfos[];
extern const extended_syscall_info kExtendedSyscallInfos[];

/*
 * kSyscallCount and kSyscallInfos here
 */
// generated by gensyscalls
#include "syscall_table.h"
