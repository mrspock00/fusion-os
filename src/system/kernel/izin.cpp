

#include <Roster.h>

#include <string.h>

#include <FindDirectory.h>
#include <OS.h>
#include "../boot/loader/zarar.h"
#include "izin.h"
#include "../boot/loader/loader.h"
// bootloader_h'dan alinacak olan booldan kontrol edilecek
#define A_IZIN 1
#define N_IZIN 0

void izinsis(int b)
{
    int iz_a;
	if(iz.izin_onyuk == true)
    {
        iz_a = iz.izin_onyuk;
	b = 0; // b sonradan alinacak
	if(iz_a == failed) 
	{
		dprintf("loading kernel failed number is: 1");
	} else {
		failed = 1;
		b = 1; // sistem onyuklenmez
	}	
    }
}

void
izin_class::izin_shutdown(izin* izs, bool f)
{
	


} 
void 
izin_class::izin_onyuk(izin* izs, bool ef)
{	

}




