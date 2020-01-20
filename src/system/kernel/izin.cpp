


#include <string.h>

#include <FindDirectory.h>
#include <OS.h>
#include "../boot/loader/zarar.h"
#include "izin.h"

// bootloader_h'dan alinacak olan booldan kontrol edilecek
#define A_IZIN 1
#define N_IZIN 0

void izinsis()
{
    int iz_a;
	if(iz.izin_onyuk == true)
    {
        iz_a = iz.izin_onyuk;
        
	if(iz_a == failed) 
	{
		dprintf("loading kernel failed number is: 1");
	} else {
		failed = 1; // sistem onyuklenmez
	}	
	
    }





}
