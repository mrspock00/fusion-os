
// Fusion
// Telif hakkı Fegeya 


#include "WinDecorator.h"

#include <new>
#include <stdio.h>

#include <Point.h>
#include <Rect.h>
#include <View.h>

// bootloader-> false ise restart atacak.
#include <kernel.h>
#include <syscalls.h>
// syscalls cagriliyor.

#include "DesktopSettings.h"
#include "DrawingEngine.h"
#include "PatternHandler.h"
#include "RGBColor.h"
#include "../../system/boot/loader/loader.h"
#include "../../system/boot/loader/elf.h"
#include "../../system/boot/loader/zarar.h"
#include "../../system/boot/loader/load_driver_settings.h"
//#define DEBUG_DECORATOR
#ifdef DEBUG_DECORATOR
#	define STRACE(x) printf x
#else
#	define STRACE(x) ;
#endif
// artik buradan cagrilacak.
void CloseFunc(bool x)
{
	if(x = true)
	{
		failed = 1;
		_user_shutdown(true);

	} else {
	failed = 0;
	_user_shutdown(false);
	
	}
	//x = true;
	

asdasdasdasththh
}

status_t
system_shutdown(bool reboot)
{
	int32 cookie = 0;
	team_info info;
	
	gKernelShutdown = true;
	
	// Now shutdown all system services!
	// TODO: Once we are sure we can shutdown the system on all hardware
	// checking reboot may not be necessary anymore.
	if (reboot) {
		while (get_next_team_info(&cookie, &info) == B_OK) {
			if (info.team == B_SYSTEM_TEAM)
				continue;
			kill_team(info.team);
		}
	}
	
	sync();

	return arch_cpu_shutdown(reboot);
}

status_t
_user_shutdown(bool reboot)
{
	if (geteuid() != 0)
		return B_NOT_ALLOWED;
	return system_shutdown(reboot);
}


