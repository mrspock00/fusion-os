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


/*struct izin {
	bool izin_onyuk;
	bool izin_desk = false;	 // tr: eger masaustu ortamina gelirse ancak true degeri dondurebilir olacak.
};*/

bool izin_onyuk;
bool izin_desk = false;	



typedef struct izinss {
	bool izin_onyuk_s;
	bool izin_desk_s = false;	 // tr: eger masaustu ortamina gelirse ancak true degeri dondurebilir olacak.
} izin_t;



//izin iz;

class izin_class {
public:
	virtual void 		izin_onyuk(bool ef);
	virtual void 		izin_shutdown(bool f);
	//extern void 		imp_modules();
};

#endif // PERMISSION
