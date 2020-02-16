/*
** Telif hakki fegeya
** FegeyaOS 2020
*/
#ifndef ZARAR_H
#define ZARAR_H

#include <OS.h>

#include <boot/vfs.h>
int failed = 0;

// bugs: adding typedef struct func


typedef struct stage3 {
	int32 stagenumber;
} stage3_t;
// stage3_t ile al.

stage3_t n;
class Bootloader_s {
public:
		void BootLoader_stage3(BootVolume& volume, stage3_t* n,
					bool boolean);
		void BootLoader_stage3_comp(stage3_t* x,
					bool xb);

};


#endif	
