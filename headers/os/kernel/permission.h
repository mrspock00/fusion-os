/*
 * Copyright 2020, Ferhat Gecdogan. All Rights Reserved.
 * Distributed under the terms of the GPLv3 License.
 *
 * Authors:
 *		Ferhat Gecdogan <ferhatgectao@gmail.com>
 */


#ifndef PERMISSION_H
#define PERMISSION_H


#include <debug.h>
#include <DPC.h>
#include <elf.h>
#include <find_directory_private.h>
#include <fs/devfs.h>
#include <fs/KPath.h>
#include <int.h>
#include <kdevice_manager.h>
#include <kdriver_settings.h>
#include <kernel_daemon.h>
#include <kmodule.h>
#include <kscheduler.h>
#include <ksyscalls.h>
#include <ksystem_info.h>
#include <lock.h>
#include <low_resource_manager.h>
#include <messaging.h>
#include <Notifications.h>
#include <port.h>
#include <posix/realtime_sem.h>
#include <posix/xsi_message_queue.h>
#include <posix/xsi_semaphore.h>
#include <real_time_clock.h>
#include <sem.h>
#include <smp.h>    
#include <system_profiler.h>
#include <team.h>
#include <timer.h>
#include <user_debugger.h>
#include <user_mutex.h>
#include <vfs.h>
#include <vm/vm.h>
#include <boot/kernel_args.h>
#include <boot/vfs.h>
#include <OS.h>

typedef char fchar;

class FPermission {
public:
    FPermission();
    ~FPermission();
    void        GetPermission(fchar, fchar); // I will add variable.. 
private:
    uinteger8   permission_value;
    bool    desktop_permission_value;

};

#endif // PERMISSION_H
