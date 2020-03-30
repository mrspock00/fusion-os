

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
#define IZIN_SYS_D 2048
#define IZIN_SYS_N 1024

	


void izin_sis_enab(bool izin_ss_x)
{
	izin_ss_x = izin_onyuk;
}
//	izinsis();
void 
izinsis()
{
 /*   int iz_a;
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
    }*
	*/	


	//const char	*ERR_S = "ERR: YOUR SYSTEM IS NOT SECURE! PLEASE DON'T CLOSE IZINSIS!";
	const char  *GOOD_S = "GOOD: YOUR SYSTEM IS SECURE";
	const char	*BAD_S = "BAD: IZINSIS'S SOME SECURE SYSTEM IS CLOSED IN YOUR SYSTEM.";
	//const char 	*EXC_S = "EXCELLENT: IZINSIS LOOKS GOOD. YOUR SYSTEM'S SECURE SYSTEM IS OPEN";
	uint16_t number_desk;
	uint16_t number;
	bool izin_tt = izin_onyuk;
	bool izin_desktop = izin_desk; 
	if(izin_tt == true)
	{
		number = 1;
		izin_sis_enab(izin_tt);	
		dprintf(GOOD_S);
	} else {
		number = 0;
		dprintf(BAD_S);
		izin_sis_enab(izin_desktop);
	}

	if(izin_desktop == true)
	{
		number_desk = 2;
		izin_sis_enab(izin_desktop);
	} else {
		number_desk = 3;
		izin_sis_enab(izin_tt);
	}
    /*
	--
	--
	--			 ------- -------- ----    -------- -----
	--			 ------- --	   -- -- --   --       --  --
	--			 --   -- -- -- -- --  --  ------  --   --
	------------ --   -- --    -- -- --   --	   ----- 
	------------ ------- --    -- ----    -------- --   --


	-------	 ----------  -----     --------   ONLY IZINSIS CODE:D 
	-------  ----------  --   --   --
	--		 --		 --  --     -- ------
 	--		 --		 --  --    --  --
	-------	 ----------  --   --   --------
	-------  ----------  ------    

	*/
	
	// tr=eklenecek.
	// TODO: tr= izinsis masaustu ortamina gelindigi taktirde true degeri dondurmesi
}

/*
struct izin {
	bool izin_onyuk;
	bool izin_desk;	
};
*/
void
izin_class::izin_shutdown(bool f)
{
	


} 
void 
izin_class::izin_onyuk(bool ef)
{	

}




