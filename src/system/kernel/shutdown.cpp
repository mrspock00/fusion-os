/* Telif hakkı 2020 Fegeya */


#include <kernel.h>
#include <syscalls.h>


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


//	#pragma mark -


status_t
_user_shutdown(bool reboot)
{
	if (geteuid() != 0)
		return B_NOT_ALLOWED;
	return system_shutdown(reboot);
}


status_t
_user_recovery_mode(bool reboot)
{

	if(reboot == true)
	{
	system_shutdown(reboot);

	}
	return system_shutdown(reboot);
}